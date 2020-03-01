from ply import lex
from clt import cl_ast
import copy
from be_typing import cast, TYPE_CHECKING


class ClLexer:
    class UnknownScope(cl_ast.scope.Scope):
        def __init__(self):
            # type: () -> None
            pass

        def find(self, name, position, source_context, is_current_scope):
            # type: (str, Position, cl_ast.cppobject.CppObject, bool) -> None
            return None

    class TemplateBind:
        def __init__(self, previous_bind, bind_index, template):
            # type: (Optional[ClLexer.TemplateBind], int, Template) -> None
            self.previous_bind = previous_bind
            self.template = template
            self.next_bind = bind_index + 1
            self.parameter_binds = previous_bind and previous_bind.parameter_binds or {} \
                # type: Dict[BaseTemplateParameter, Tuple[int, BaseTemplateObject]]

        def temporary_bind(self, template):
            # type: (Template) -> ClLexer.TemplateBind
            result = ClLexer.TemplateBind(self.previous_bind, self.next_bind - 1, self.template)
            parameter_binds = {}
            for p, b in result.parameter_binds.items():
                parameter_binds[p] = b
            for i, p in enumerate(cast('Template.Scope', result.template.scope).parameters):
                parameter_binds[p] = (i, template)
            result.parameter_binds = parameter_binds
            return result

    class TemplateStack:
        def __init__(self, template_list):
            # type: (List[Template]) -> None
            self.template_list = template_list[:]

        def bind(self, template, current_bind):
            # type: (Optional[BaseTemplateObject], Optional[ClLexer.TemplateBind]) -> Optional[ClLexer.TemplateBind]
            bind_index = current_bind and current_bind.next_bind or 0
            if bind_index >= len(self.template_list):
                return None

            result = ClLexer.TemplateBind(current_bind, bind_index, self.template_list[bind_index])
            if template is not None:
                for i, p in enumerate(cast('Template.Scope', result.template.scope).parameters):
                    result.parameter_binds[p] = (i, template)
            return result

        def bind_last(self, template, current_bind):
            # type: (Optional[Template], Optional[ClLexer.TemplateBind]) -> Optional[ClLexer.TemplateBind]
            next_index = current_bind and current_bind.next_bind or 0
            if current_bind:
                if current_bind.next_bind >= len(self.template_list):
                    return None
                else:
                    assert template
                    return self.bind(template, current_bind)
            elif len(self.template_list) == 0:
                return None
            else:
                return self.bind(template or self.template_list[next_index], current_bind)

        def __getitem__(self, index):
            # type: (int) -> Template
            return self.template_list[index]

        def pop(self):
            # type: () -> Optional[Template]
            try:
                return self.template_list.pop(-1)
            except IndexError:
                return None

    def __init__(self, filename, logger):
        # type: (str, Logger) -> None
        self._lexer = lex.lex(object=self)
        self.filename = filename
        self.logger = logger
        self.last_token = None         # type: Optional[lex.LexToken]
        self.scopes = []               # type: List[Scope]
        self.owner_scopes = []         # type: List[Scope]
        self.current_scope = None      # type: Optional[Scope]
        self.pp_line = None            # type: Optional[str]
        self.pp_filename = None        # type: Optional[str]
        self.template_stack = None     # type: Optional[ClLexer.TemplateStack]
        self._template_stack = None    # type: Optional[ClLexer.TemplateStack]
        self._template_stack_count = 0
        self.base_types = {}           # type: Dict[str, cl_ast.TypeRef]
        p = cl_ast.Position('<built-in>', 0, 0, 0, '')
        for t in ('bool', 'float', 'double', 'i8', 'u8', 'i16', 'u16', 'i32', 'u32', 'i64', 'u64'):
            type = cl_ast.TypeRef(self, p, cl_ast.ast_types.BuiltIn(self, p, t))
            type.qualifiers.add('const')
            self.base_types[t] = type

    def log_cpperror(self, e):
        # type: (CppError) -> None
        if e.inner_error:
            self.log_cpperror(e.inner_error)
        e.error(e.position, e.arguments)

    def push_scope(self, scope, owner_scope=True):
        # type: (Scope, bool) -> None
        # print('>%s %s' % (' '*len(self.scopes), str(scope.owner)))
        assert isinstance(scope, cl_ast.scope.Scope)
        self.scopes.append(scope)
        if owner_scope:
            self.owner_scopes.append(scope)

    def pop_scope(self, scope):
        # type: (Scope) -> None
        # print('<%s %s' % (' '*(len(self.scopes) - 1), str(scope.owner)))
        assert self.scopes[-1] == scope, "asymetric scope push/pop: %s/%s" % (scope.owner, self.scopes[-1].owner)
        self.scopes[-1].seal()
        self.scopes.pop(-1)
        if self.owner_scopes and self.owner_scopes[-1] == scope:
            self.owner_scopes.pop(-1)

    def set_search_scope(self, obj):
        # type: (CppObject) -> None
        # print('new search scope: %s' % obj.name)
        self.current_scope = obj.scope

    def clear_search_scope(self):
        # type: () -> None
        self.current_scope = None

    def push_template_stack(self, templates):
        # type: (List[Template]) -> None
        assert self.template_stack is None, self.template_stack
        self.template_stack = ClLexer.TemplateStack(templates)

    def finalize_template_stack(self):
        # type: () -> None
        assert self.template_stack is not None
        self.template_stack = None

    def disable_template_stack(self):
        # type: () -> None
        if self._template_stack_count == 0:
            assert self._template_stack is None
            self._template_stack = self.template_stack
            self.template_stack = None
        self._template_stack_count += 1

    def enable_template_stack(self):
        # type: () -> None
        self._template_stack_count -= 1
        if self._template_stack_count == 0:
            self.template_stack = self._template_stack
            self._template_stack = None

    def lookup_by_name(self, name, position):
        # type: (str, Position) -> Tuple[Optional[Scope], Optional[CppObject]]
        if self.current_scope:
            if getattr(self.last_token, 'type') == 'NOT':
                name = '~' + name
            try:
                return (None, self.current_scope.find(name, position, self.scopes[-1].owner, True))
            except cl_ast.CppError as e:
                self.log_cpperror(e)
                return (None, None)
        else:
            for s in self.scopes[::-1]:
                try:
                    obj = s.find(name, position, self.scopes[-1].owner, False)
                except cl_ast.CppError as e:
                    self.log_cpperror(e)
                    return (None, None)
                else:
                    if obj:
                        return (s, obj)
            return (None, None)

    def lookup(self, token):
        # type: (lex.LexToken) -> None
        scope, obj = self.lookup_by_name(token.value, self._position(token))
        if obj:
            token.found_object = obj
            token.type = obj.get_token_type()
            if scope:
                for s in self.scopes[::-1]:
                    if isinstance(s, cl_ast.ast_templates.Template.Scope):
                        continue
                    if scope == s:
                        break
                    token.type += '_SHADOW'
                    return

    def input(self, text):
        # type: (str) -> None
        self._lexer.input(text)

    def lexdata(self):
        # type: () -> str
        return self._lexer.lexdata

    def token(self):
        # type: () -> Optional[lex.LexToken]
        new_token = None           # type: Optional[lex.LexToken]
        if self.last_token and self.last_token.type == 'SCOPE':
            new_token = copy.copy(self.last_token)
            new_token.type = 'SCOPE_MARKER'
        elif self.last_token and self.last_token.type == 'LBRACE':
            new_token = copy.copy(self.last_token)
            new_token.type = 'BRACE_MARKER'
        else:
            new_token = self._lexer.token()
            if new_token:
                new_token.lexer = self
                new_token.filename = self.filename
                new_token.endlexpos = new_token.lexpos + len(new_token.value)
                scope_breaks = (
                    'TEMPLATE',
                    'OPERATOR',
                )
                if new_token.type == 'ID':
                    new_token.found_object = None
                if new_token.type == 'ID' and (not self.last_token or self.last_token.type not in ('TEMPLATE', )):
                    self.lookup(new_token)
                    self.clear_search_scope()
                if self.last_token and self.last_token.type != 'SCOPE_MARKER':
                    self.clear_search_scope()
                elif new_token.type in scope_breaks:
                    self.clear_search_scope()
        self.last_token = new_token
                                   # print(new_token)
        return new_token

    def _execute_pragma(self, pragma):
        # type: (List[lex.LexToken]) -> None
        print(pragma)

    def _position(self, token):
        # type: (lex.LexToken) -> Position
        return cl_ast.Position(
            self.filename, token.lineno, token.lexpos, token.lexpos + len(token.value), self.lexdata()
        )

    # Lexer rules ##
    from .cl_grammar import keywords, tokens

    keyword_map = {}
    for keyword in keywords:
        keyword_map[keyword] = keyword.upper()

    identifier = r'[a-zA-Z_][0-9a-zA-Z_]*'

    hex_prefix = '0[xX]'
    hex_digits = '[0-9a-fA-F]+'
    bin_prefix = '0[bB]'
    bin_digits = '[01]+'

    # integer constants (K&R2: A.2.5.1)
    integer_suffix_opt = r'(([uU]ll)|([uU]LL)|(ll[uU]?)|(LL[uU]?)|([uU][lL])|([lL][uU]?)|[uU])?'
    decimal_constant = '(0' + integer_suffix_opt + ')|([1-9][0-9]*' + integer_suffix_opt + ')'
    octal_constant = '0[0-7]*' + integer_suffix_opt
    hex_constant = hex_prefix + hex_digits + integer_suffix_opt
    bin_constant = bin_prefix + bin_digits + integer_suffix_opt

    bad_octal_constant = '0[0-7]*[89]'

    # character constants (K&R2: A.2.5.2)
    # Note: a-zA-Z and '.-~^_!=&;,' are allowed as escape chars to support #line
    # directives with Windows paths as filenames (..\..\dir\file)
    # For the same reason, decimal_escape allows all digit sequences. We want to
    # parse all correct code, even if it means to sometimes parse incorrect
    # code.
    #
    simple_escape = r"""([a-zA-Z._~!=&\^\-\\?'"])"""
    decimal_escape = r"""(\d+)"""
    hex_escape = r"""(x[0-9a-fA-F]+)"""
    bad_escape = r"""([\\][^a-zA-Z._~^!=&\^\-\\?'"x0-7])"""

    escape_sequence = r"""(\\(""" + simple_escape + '|' + decimal_escape + '|' + hex_escape + '))'
    cconst_char = r"""([^'\\\n]|""" + escape_sequence + ')'
    char_const = "'" + cconst_char + "'"
    wchar_const = 'L' + char_const
    unmatched_quote = "('" + cconst_char + "*\\n)|('" + cconst_char + "*$)"
    bad_char_const = r"""('""" + cconst_char + """[^'\n]+')|('')|('""" + bad_escape + r"""[^'\n]*')"""

    # string literals (K&R2: A.2.6)
    string_char = r"""([^"\\\n]|""" + escape_sequence + ')'
    string_literal = '"' + string_char + '*"'
    wstring_literal = 'L' + string_literal
    bad_string_literal = '"' + string_char + '*?' + bad_escape + string_char + '*"'

    # floating constants (K&R2: A.2.5.3)
    exponent_part = r"""([eE][-+]?[0-9]+)"""
    fractional_constant = r"""([0-9]*\.[0-9]+)|([0-9]+\.)"""
    floating_constant = '((((%s)%s?)|([0-9]+%s))[FfLl]?)' % (fractional_constant, exponent_part, exponent_part)
    binary_exponent_part = r'''([pP][+-]?[0-9]+)'''
    hex_fractional_constant = r'(((%s)?\.%s)|(%s\.))' % (hex_digits, hex_digits, hex_digits)
    hex_floating_constant = '(%s(%s|%s)%s[FfLl]?)' % (
        hex_prefix, hex_digits, hex_fractional_constant, binary_exponent_part
    )

    states = (
        ('ppline', 'exclusive'),       # preprocessor line directives
        ('ppident', 'exclusive'),      # preprocessor ident directives
        ('pppragma', 'exclusive'),     # preprocessor pragma directives
    )

    # Rules for the normal state
    t_ignore = ' \t'

    # Newlines
    def t_NEWLINE(self, t):
        # type: (lex.LexToken) -> Optional[lex.LexToken]
        r'\n+'
        self._lexer.lineno += t.value.count("\n")
        return None

    # Rules for the ppident state
    @lex.TOKEN(r'\#[ \t]*ident')
    def t_PREPROC_IDENT(self, t):
        # type: (lex.LexToken) -> Optional[lex.LexToken]
        self._lexer.begin('ppident')
        return None

    t_ppident_ignore = ' \t'

    def t_ppident_IDENT(self, t):
        # type: (lex.LexToken) -> Optional[lex.LexToken]
        r'[^\n]+'
        return None

    def t_ppident_NEWLINE(self, t):
        # type: (lex.LexToken) -> Optional[lex.LexToken]
        r'\n'
        self._lexer.begin('INITIAL')
        return None

    def t_ppident_error(self, t):
        # type: (lex.LexToken) -> Optional[lex.LexToken]
        self.logger.C0003(self._position(t), str(t))
        self._lexer.skip(1)
        return None

    # Rules for the pppragma state
    @lex.TOKEN(r'\#[ \t]*pragma')
    def t_PREPROC_PRAGMA(self, t):
        # type: (lex.LexToken) -> Optional[lex.LexToken]
        self._lexer.begin('pppragma')
        self._pragma = []  # type: List[lex.LexToken]
        return None

    t_pppragma_ignore = ' \t'

    @lex.TOKEN(identifier)
    def t_pppragma_id(self, t):
        # type: (lex.LexToken) -> Optional[lex.LexToken]
        self._pragma.append(t)
        return None

    @lex.TOKEN(decimal_constant)
    def t_pppragma_nmber(self, t):
        # type: (lex.LexToken) -> Optional[lex.LexToken]
        self._pragma.append(t)
        return None

    def t_pppragma_whitespace(self, t):
        # type: (lex.LexToken) -> Optional[lex.LexToken]
        r'[ \t]+'
        return None

    def t_pppragma_else(self, t):
        # type: (lex.LexToken) -> Optional[lex.LexToken]
        r'[^\n]'
        self._pragma.append(t)
        return None

    def t_pppragma_NEWLINE(self, t):
        # type: (lex.LexToken) -> Optional[lex.LexToken]
        r'\n'
        self._lexer.begin('INITIAL')
        self._execute_pragma(self._pragma)
        self._pragma = []
        return None

    def t_pppragma_error(self, t):
        # type: (lex.LexToken) -> Optional[lex.LexToken]
        self.logger.C0004(self._position(t), str(t))
        self._lexer.skip(1)
        return None

    # Rules for the ppline state
    @lex.TOKEN(r'\#')
    def t_PREPROC_LINE(self, t):
        # type: (lex.LexToken) -> Optional[lex.LexToken]
        self._lexer.begin('ppline')
        return None

    @lex.TOKEN(string_literal)
    def t_ppline_FILENAME(self, t):
        # type: (lex.LexToken) -> Optional[lex.LexToken]
        if self.pp_line is None:
            self.logger.C0005(self._position(t))
            self._lexer.skip(1)
        else:
            self.pp_filename = t.value.lstrip('"').rstrip('"')
        return None

    @lex.TOKEN(decimal_constant)
    def t_ppline_LINE_NUMBER(self, t):
        # type: (lex.LexToken) -> Optional[lex.LexToken]
        if self.pp_line is None:
            self.pp_line = t.value
        else:
            # Ignore: GCC's cpp sometimes inserts a numeric flag
            # after the file name
            pass
        return None

    def t_ppline_NEWLINE(self, t):
        # type: (lex.LexToken) -> Optional[lex.LexToken]
        r'\n'
        if self.pp_line is None:
            self.logger.C0006(self._position(t))
            # self._lexer.skip(1)
        else:
            self._lexer.lineno = int(self.pp_line)
            if self.pp_filename is not None:
                self.filename = self.pp_filename
        self.pp_line = None
        self._lexer.begin('INITIAL')
        return None

    def t_ppline_PPLINE(self, t):
        # type: (lex.LexToken) -> Optional[lex.LexToken]
        r'line'
        pass

    t_ppline_ignore = ' \t'

    def t_ppline_error(self, t):
        # type: (lex.LexToken) -> Optional[lex.LexToken]
        self.logger.C0007(self._position(t))
        self._lexer.skip(1)
        return None

    # Operators
    t_PLUS = r'\+'
    t_MINUS = r'-'
    t_TIMES = r'\*'
    t_DIVIDE = r'/'
    t_MOD = r'%'
    t_OR = r'\|'
    t_AND = r'&'
    t_NOT = r'~'
    t_XOR = r'\^'
    t_LSHIFT = r'<<'
    t_RSHIFT = r'>>'
    t_LOR = r'\|\|'
    t_LAND = r'&&'
    t_LNOT = r'!'
    t_LT = r'<'
    t_GT = r'>'
    t_LE = r'<='
    t_GE = r'>='
    t_EQ = r'=='
    t_NE = r'!='

    # Assignment operators
    t_EQUALS = r'='
    t_TIMESEQUAL = r'\*='
    t_DIVEQUAL = r'/='
    t_MODEQUAL = r'%='
    t_PLUSEQUAL = r'\+='
    t_MINUSEQUAL = r'-='
    t_LSHIFTEQUAL = r'<<='
    t_RSHIFTEQUAL = r'>>='
    t_ANDEQUAL = r'&='
    t_OREQUAL = r'\|='
    t_XOREQUAL = r'\^='

    # Increment/decrement
    t_PLUSPLUS = r'\+\+'
    t_MINUSMINUS = r'--'

    # ->
    t_ARROW = r'->'

    # ?
    t_CONDOP = r'\?'

    # Scope
    t_SCOPE = r'::'

    # Delimeters
    t_LBRACE = r'{'
    t_RBRACE = r'}'
    t_LPAREN = r'\('
    t_RPAREN = r'\)'
    t_LBRACKET = r'\['
    t_RBRACKET = r'\]'
    t_COMMA = r','
    t_PERIOD = r'\.'
    t_SEMI = r';'
    t_COLON = r':'
    # t_ELLIPSIS          = r'\.\.\.'

    t_STRING_LITERAL = string_literal

    # The following floating and integer constants are defined as
    # functions to impose a strict order (otherwise, decimal
    # is placed before the others because its regex is longer,
    # and this is bad)
    @lex.TOKEN(floating_constant)
    def t_FLOAT_CONST(self, t):
        # type: (lex.LexToken) -> Optional[lex.LexToken]
        if t.value[-1] in 'fF':
            setattr(t, 'constant_value', float(t.value[:-1]))
            setattr(t, 'constant_type', self.base_types['float'])
        elif t.value[-1] in 'dD':
            setattr(t, 'constant_value', float(t.value[:-1]))
            setattr(t, 'constant_type', self.base_types['double'])
        else:
            assert t.value[-1] in '0123456789.'
            setattr(t, 'constant_value', float(t.value[:-1]))
            setattr(t, 'constant_type', self.base_types['float'])
        return t

    @lex.TOKEN(hex_floating_constant)
    def t_HEX_FLOAT_CONST(self, t):
        # type: (lex.LexToken) -> Optional[lex.LexToken]
        assert False

    def _make_int_const(self, t, base):
        # type: (lex.LexToken, int) -> lex.LexToken
        constant_type = 'i32'
        while t.value[-1] in 'lLuU':
            if t.value[-1] in 'lL':
                constant_type = constant_type[0] + '64'
            else:
                constant_type = 'u' + constant_type[1:]
            t.value = t.value[:-1]
        setattr(t, 'constant_type', self.base_types[constant_type])
        setattr(t, 'constant_value', int(t.value, base))
        return t

    @lex.TOKEN(hex_constant)
    def t_INT_CONST_HEX(self, t):
        # type: (lex.LexToken) -> Optional[lex.LexToken]
        return self._make_int_const(t, 16)

    @lex.TOKEN(bin_constant)
    def t_INT_CONST_BIN(self, t):
        # type: (lex.LexToken) -> Optional[lex.LexToken]
        return self._make_int_const(t, 2)

    @lex.TOKEN(bad_octal_constant)
    def t_BAD_CONST_OCT(self, t):
        # type: (lex.LexToken) -> Optional[lex.LexToken]
        self.logger.C0008(self._position(t), 'octal')
        self._lexer.skip(1)
        return None

    @lex.TOKEN(octal_constant)
    def t_INT_CONST_OCT(self, t):
        # type: (lex.LexToken) -> Optional[lex.LexToken]
        return self._make_int_const(t, 8)

    @lex.TOKEN(decimal_constant)
    def t_INT_CONST_DEC(self, t):
        # type: (lex.LexToken) -> Optional[lex.LexToken]
        return self._make_int_const(t, 10)

    # Must come before bad_char_const, to prevent it from
    # catching valid char constants as invalid
    #
    @lex.TOKEN(char_const)
    def t_CHAR_CONST(self, t):
        # type: (lex.LexToken) -> Optional[lex.LexToken]
        l = {}     # type: Dict[str, unicode]
        exec("x = u%s" % t.value, globals(), l)
        x = l['x']
        if ord(x) > 255:
            self.logger.C0008(self._position(t), 'character')
        setattr(t, 'constant_value', ord(x) % 256)
        setattr(t, 'constant_type', self.base_types['u8'])
        return t

    @lex.TOKEN(wchar_const)
    def t_WCHAR_CONST(self, t):
        # type: (lex.LexToken) -> Optional[lex.LexToken]
        l = {}     # type: Dict[str, unicode]
        exec("x = u%s" % t.value[1:], globals(), l)
        x = l['x']
        setattr(t, 'constant_value', ord(x))
        setattr(t, 'constant_type', self.base_types['u32'])
        return t

    @lex.TOKEN(unmatched_quote)
    def t_UNMATCHED_QUOTE(self, t):
        # type: (lex.LexToken) -> Optional[lex.LexToken]
        self.logger.C0009(self._position(t))
        self._lexer.skip(1)
        return None

    @lex.TOKEN(bad_char_const)
    def t_BAD_CHAR_CONST(self, t):
        # type: (lex.LexToken) -> Optional[lex.LexToken]
        self.logger.C0008(self._position(t), 'char')
        self._lexer.skip(1)
        return None

    @lex.TOKEN(wstring_literal)
    def t_WSTRING_LITERAL(self, t):
        # type: (lex.LexToken) -> Optional[lex.LexToken]
        # TODO
        return t

    # unmatched string literals are caught by the preprocessor

    @lex.TOKEN(bad_string_literal)
    def t_BAD_STRING_LITERAL(self, t):
        # type: (lex.LexToken) -> Optional[lex.LexToken]
        self.logger.C0010(self._position(t))
        self._lexer.skip(1)
        return t

    @lex.TOKEN(identifier)
    def t_ID(self, t):
        # type: (lex.LexToken) -> Optional[lex.LexToken]
        t.type = self.keyword_map.get(t.value, "ID")
        return t

    def t_error(self, t):
        # type: (lex.LexToken) -> Optional[lex.LexToken]
        self.logger.C0000(self._position(t), repr(t.value[0]))
        self._lexer.skip(1)
        return None


if TYPE_CHECKING:
    from typing import Dict, List, Optional, Tuple
    from clt.cl_ast.cppobject import CppObject
    from clt.cl_ast.scope import Scope
    from clt.cl_ast.position import Position
    from clt.cl_ast.ast_templates import BaseTemplateParameter, BaseTemplateObject, Template
    from clt.cl_ast.error import CppError
    from clt.cl_parser import ClParser
    from clt.cl_messages import Logger
