from ..ply import lex
from . import cl_ast
import sys
import copy

color_list = {
    'BOLD':     '\x1b[01;1m',
    'BLACK':    '\x1b[30m',
    'RED':      '\x1b[31m',
    'GREEN':    '\x1b[32m',
    'YELLOW':   '\x1b[33m',
    'BLUE':     '\x1b[34m',
    'PINK':     '\x1b[35m',
    'CYAN':     '\x1b[36m',
    'WHITE':    '\x1b[37m',
    'BBLACK':   '\x1b[01;30m',
    'BRED':     '\x1b[01;31m',
    'BGREEN':   '\x1b[01;32m',
    'BYELLOW':  '\x1b[01;33m',
    'BBLUE':    '\x1b[01;34m',
    'BPINK':    '\x1b[01;35m',
    'BCYAN':    '\x1b[01;36m',
    'BWHITE':   '\x1b[01;37m',
    'NORMAL':   '\x1b[0m',
}

default_color_pattern = (color_list['BWHITE'], color_list['BWHITE'],
                         color_list['NORMAL'], color_list['BGREEN'], color_list['NORMAL'])
color_pattern = {
    'note':     (color_list['BBLACK'], color_list['BWHITE'],
                 color_list['NORMAL'], color_list['BGREEN'], color_list['NORMAL']),
    'info':     (color_list['BWHITE'], color_list['BWHITE'],
                 color_list['NORMAL'], color_list['BGREEN'], color_list['NORMAL']),
    'warning':  (color_list['BYELLOW'], color_list['BWHITE'],
                 color_list['NORMAL'], color_list['BGREEN'], color_list['NORMAL']),
    'error':    (color_list['BRED'], color_list['BWHITE'],
                 color_list['BWHITE'], color_list['BGREEN'], color_list['NORMAL']),
}
ide_format = {
    'msvc':     '%(f)s(%(l)d,%(c)d) :',
    'unix':     '%(f)s:%(l)d:%(c)d:',
}

class ClLexer:
    class UnknownScope:
        def find(self, name, position, source_context, is_current_scope):
            return None

    class TemplateBind:
        def __init__(self, previous_bind, bind_index, template):
            self.previous_bind = previous_bind
            self.template = template
            self.next_bind = bind_index + 1
            self.parameter_binds = previous_bind and previous_bind.parameter_binds or { }

        def temporary_bind(self, template):
            result = ClLexer.TemplateBind(self.previous_bind, self.next_bind-1, self.template)
            parameter_binds = { }
            for p, b in result.parameter_binds.items():
                parameter_binds[p] = b
            for i, p in enumerate(result.template.scope.parameters):
                parameter_binds[p] = (i, template)
            result.parameter_binds = parameter_binds
            return result

    class TemplateStack:
        def __init__(self, template_list):
            self.template_list = template_list[:]

        def bind(self, template, current_bind):
            bind_index = current_bind and current_bind.next_bind or 0
            if bind_index >= len(self.template_list):
                return None
            
            result = ClLexer.TemplateBind(current_bind, bind_index, self.template_list[bind_index])
            for i, p in enumerate(result.template.scope.parameters):
                result.parameter_binds[p] = (i, template)
            return result

        def bind_last(self, template, current_bind):
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
            return self.template_list[index]

        def pop(self):
            try:
                return self.template_list.pop(-1)
            except IndexError:
                return None

    def __init__(self, filename, error_format):
        self.lexer = lex.lex(object=self)
        self.filename = filename
        self.error_count = 0
        self.last_token = None
        self.scopes = []
        self.owner_scopes=[]
        self.current_scope = None
        self.pp_line = None
        self.pp_filename = None
        self.template_stack = None
        self._template_stack = None
        self._template_stack_count = 0
        try:
            self.error_format = ide_format[error_format]
            self.error_color = error_format == 'unix' and sys.stderr.isatty()
        except KeyError:
            self.error_format = ide_format['unix']
            self.error_color = False
        self.base_types = { }
        p = ('<built-in>', 0, 0, 0)
        for t in ('bool', 'float', 'double',
                    'i8', 'u8', 'i16', 'u16', 'i32', 'u32', 'i64', 'u64'):
            type = cl_ast.types.TypeRef(self, p, cl_ast.types.BuiltIn(self, p, t))
            type.qualifiers.add('const')
            self.base_types[t] = type

    def note(self, msg, pos):
        self._msg('note', msg, pos)

    def info(self, msg, pos):
        self._msg('info', msg, pos)

    def warning(self, msg, pos):
        self._msg('warning', msg, pos)

    def error(self, msg, pos):
        self.error_count += 1
        self._msg('error', msg, pos)

    def log_cpperror(self, e):
        if e.inner_error:
            self.log_cpperror(e.inner_error)
            self.note(e.message, e.position)
        else:
            self.error(e.message, e.position)

    def push_scope(self, scope, owner_scope=True):
        #print('>%s %s' % (' '*len(self.scopes), str(scope.owner)))
        assert isinstance(scope, cl_ast.scope.Scope)
        self.scopes.append(scope)
        if owner_scope:
            self.owner_scopes.append(scope)

    def pop_scope(self, scope):
        #print('<%s %s' % (' '*(len(self.scopes) - 1), str(scope.owner)))
        assert self.scopes[-1] == scope,"asymetric scope push/pop: %s/%s" % (scope.owner, self.scopes[-1].owner)
        self.scopes[-1].seal()
        self.scopes.pop(-1)
        if self.owner_scopes and self.owner_scopes[-1] == scope:
            self.owner_scopes.pop(-1)

    def set_search_scope(self, obj):
        #print('new search scope: %s' % obj.name)
        self.current_scope = obj.scope

    def clear_search_scope(self):
        self.current_scope = None

    def push_template_stack(self, templates):
        assert self.template_stack is None, self.template_stack
        self.template_stack = ClLexer.TemplateStack(templates)

    def finalize_template_stack(self):
        assert self.template_stack is not None
        self.template_stack = None
    
    def disable_template_stack(self):
        if self._template_stack_count == 0:
            assert self._template_stack is None
            self._template_stack = self.template_stack
            self.template_stack = None
        self._template_stack_count += 1

    def enable_template_stack(self):
        self._template_stack_count -= 1
        if self._template_stack_count == 0:
            self.template_stack = self._template_stack
            self._template_stack = None

    def show_error_stack(self, cpp_error):
        if cpp_error.inner_error:
            self.show_error_stack(cpp_error.inner_error)
            self.note(cpp_error.message, cpp_error.position)
        else:
            self.error(cpp_error.message, cpp_error.position)

    def lookup_by_name(self, name, position):
        if self.current_scope:
            if self.last_token.type == 'NOT':
                name = '~'+name
            try:
                return (None, self.current_scope.find(name, position, self.scopes[-1].owner, True))
            except cl_ast.error.CppError as e:
                self.show_error_stack(e)
                return (None, None)
        else:
            for s in self.scopes[::-1]:
                try:
                    obj = s.find(name, position, self.scopes[-1].owner, False)
                except cl_ast.error.CppError as e:
                    self.show_error_stack(e)
                    return (None, None)
                else:
                    if obj:
                        return (s, obj)
            return (None, None)

    def lookup(self, token):
        scope, obj = self.lookup_by_name(token.value, self._position(token))
        if obj:
            token.found_object = obj
            token.type = obj.get_token_type()
            if scope:
                for s in self.scopes[::-1]:
                    if isinstance(s, cl_ast.templates.TemplateScope):
                        continue
                    if scope == s:
                        break
                    token.type += '_SHADOW'
                    return

    def input(self, text):
        self.lexer.input(text)

    def token(self):
        if self.last_token and self.last_token.type == 'SCOPE':
            new_token = copy.copy(self.last_token)
            new_token.type = 'SCOPE_MARKER'
        elif self.last_token and self.last_token.type == 'LBRACE':
            new_token = copy.copy(self.last_token)
            new_token.type = 'BRACE_MARKER'
        else:
            new_token = self.lexer.token()
            if new_token:
                new_token.lexer = self
                new_token.filename = self.filename
                new_token.endlexpos = new_token.lexpos + len(new_token.value)
                scope_breaks = ('TEMPLATE', 'OPERATOR', )
                if new_token.type == 'ID':
                    new_token.found_object = None
                if new_token.type == 'ID' and (not self.last_token or self.last_token.type not in ('TEMPLATE',)):
                    self.lookup(new_token)
                    self.clear_search_scope()
                if self.last_token and self.last_token.type != 'SCOPE_MARKER':
                    self.clear_search_scope()
                elif new_token.type in scope_breaks:
                    self.clear_search_scope()
        self.last_token = new_token
        #print(new_token)
        return new_token

    def _execute_pragma(self, pragma):
        print(pragma)

    def _position(self, token):
        return (self.filename, token.lineno, token.lexpos, token.lexpos + len(token.value))

    def _msg(self, error_type, msg, pos):
        if self.error_color:
            color_error_type, color_filename, color_msg, color_caret, color_off = color_pattern.get(error_type, default_color_pattern)
        else:
            color_error_type = ''
            color_filename = ''
            color_msg = ''
            color_caret = ''
            color_off = ''
        filename, lineno, offset, end = pos
        while offset>0 and self.lexer.lexdata[offset-1] != '\n':
            offset -= 1
        while end < len(self.lexer.lexdata) and self.lexer.lexdata[end] != '\n':
            end += 1
        location = self.error_format % { 'f': filename, 'l': lineno, 'c': pos[2] - offset + 1 }
        sys.stderr.write('%s%s%s%s %s:%s %s%s%s\n' % (color_filename, location, color_off,
                                                      color_error_type, error_type, color_off,
                                                      color_msg, msg, color_off))
        sys.stderr.write(self.lexer.lexdata[offset:end+1])
        sys.stderr.write('%s%s%s%s\n' % (' '*(pos[2] - offset),
                                         color_caret,
                                         '^'*(pos[3]-pos[2]),
                                         color_off))

    # Lexer rules ##
    keywords = (
        # raise warnings when encountered
        'signed', 'char', 'short', 'long', 'unsigned', 'uchar', 'ushort', 'uint', 'ulong',
        # use these types instead
        'i8', 'u8', 'i16', 'u16', 'i32', 'u32', 'i64', 'u64', 'float', 'half', 'double',
        'bool', 'size_t', 'ptrdiff_t', 'intptr_t', 'uintptr_t', 'void', 'int',

        'const', '__global', '__local', '__constant', '__private',
        'restrict', '__restrict', 'volatile', 'static', 'inline', 'explicit', 'virtual', 'return',
        'do', 'while', 'for', 'switch', 'case', 'default', 'break', 'continue', 'if', 'else',
        'enum', 'struct', 'union', 'typedef', 'sizeof', 'true', 'false', 'new', 'delete',

        # BugEngine additions
        'template', 'namespace', 'typename', 'class', 'using', 'friend', 'this',
        'published', 'public', 'protected', 'private', 'operator',
        'static_cast', 'dynamic_cast', 'reinterpret_cast', 'const_cast',
    ) + tuple('%s%d' % (t, n) for t in ('byte', 'ubyte', 'short', 'ushort',
                                        'int', 'uint', 'bigint', 'biguint',
                                        'float', 'double')
                              for n in (2, 3, 4, 8, 16))

    keyword_map = {}
    for keyword in keywords:
        keyword_map[keyword] = keyword.upper()

    tokens = tuple(kw.upper() for kw in keywords) + (
        'ID',
        'STRUCT_ID',
        'TYPENAME_ID',
        'NAMESPACE_ID',
        'METHOD_ID',
        'VARIABLE_ID',
        'TEMPLATE_STRUCT_ID',
        'TEMPLATE_METHOD_ID',
        'TEMPLATE_TYPENAME_ID',
        'STRUCT_ID_SHADOW',
        'TYPENAME_ID_SHADOW',
        'NAMESPACE_ID_SHADOW',
        'METHOD_ID_SHADOW',
        'VARIABLE_ID_SHADOW',
        'TEMPLATE_STRUCT_ID_SHADOW',
        'TEMPLATE_METHOD_ID_SHADOW',
        'TEMPLATE_TYPENAME_ID_SHADOW',
        'SPECIAL_METHOD_ID',

        # constants
        'INT_CONST_DEC', 'INT_CONST_OCT', 'INT_CONST_HEX', 'INT_CONST_BIN',
        'FLOAT_CONST', 'HEX_FLOAT_CONST',
        'CHAR_CONST',
        'WCHAR_CONST',

        # String literals
        'STRING_LITERAL',
        'WSTRING_LITERAL',

        # Operators
        'PLUS', 'MINUS', 'TIMES', 'DIVIDE', 'MOD',
        'OR', 'AND', 'NOT', 'XOR', 'LSHIFT', 'RSHIFT',
        'LOR', 'LAND', 'LNOT',
        'LT', 'LE', 'GT', 'GE', 'EQ', 'NE',

        # Assignment
        'EQUALS', 'TIMESEQUAL', 'DIVEQUAL', 'MODEQUAL',
        'PLUSEQUAL', 'MINUSEQUAL',
        'LSHIFTEQUAL','RSHIFTEQUAL', 'ANDEQUAL', 'XOREQUAL',
        'OREQUAL',

        # Increment/decrement
        'PLUSPLUS', 'MINUSMINUS',

        # Structure dereference (->)
        'ARROW',

        # Conditional operator (?)
        'CONDOP',

        # C++ scope operator (::)
        'SCOPE', 'SCOPE_MARKER',

        # Delimeters
        'LPAREN', 'RPAREN',         # ( )
        'LBRACKET', 'RBRACKET',     # [ ]
        'LBRACE', 'RBRACE',         # { }
        'COMMA', 'PERIOD',          # . ,
        'SEMI', 'COLON',            # ; :
        'BRACE_MARKER',

        # Ellipsis (...) is not supported
        # 'ELLIPSIS',
    )

    identifier = r'[a-zA-Z_][0-9a-zA-Z_]*'

    hex_prefix = '0[xX]'
    hex_digits = '[0-9a-fA-F]+'
    bin_prefix = '0[bB]'
    bin_digits = '[01]+'

    # integer constants (K&R2: A.2.5.1)
    integer_suffix_opt = r'(([uU]ll)|([uU]LL)|(ll[uU]?)|(LL[uU]?)|([uU][lL])|([lL][uU]?)|[uU])?'
    decimal_constant = '(0'+integer_suffix_opt+')|([1-9][0-9]*'+integer_suffix_opt+')'
    octal_constant = '0[0-7]*'+integer_suffix_opt
    hex_constant = hex_prefix+hex_digits+integer_suffix_opt
    bin_constant = bin_prefix+bin_digits+integer_suffix_opt

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

    escape_sequence = r"""(\\("""+simple_escape+'|'+decimal_escape+'|'+hex_escape+'))'
    cconst_char = r"""([^'\\\n]|"""+escape_sequence+')'
    char_const = "'"+cconst_char+"'"
    wchar_const = 'L'+char_const
    unmatched_quote = "('"+cconst_char+"*\\n)|('"+cconst_char+"*$)"
    bad_char_const = r"""('"""+cconst_char+"""[^'\n]+')|('')|('"""+bad_escape+r"""[^'\n]*')"""

    # string literals (K&R2: A.2.6)
    string_char = r"""([^"\\\n]|"""+escape_sequence+')'
    string_literal = '"'+string_char+'*"'
    wstring_literal = 'L'+string_literal
    bad_string_literal = '"'+string_char+'*?'+bad_escape+string_char+'*"'

    # floating constants (K&R2: A.2.5.3)
    exponent_part = r"""([eE][-+]?[0-9]+)"""
    fractional_constant = r"""([0-9]*\.[0-9]+)|([0-9]+\.)"""
    floating_constant = '(((('+fractional_constant+')'+exponent_part+'?)|([0-9]+'+exponent_part+'))[FfLl]?)'
    binary_exponent_part = r'''([pP][+-]?[0-9]+)'''
    hex_fractional_constant = '((('+hex_digits+r""")?\."""+hex_digits+')|('+hex_digits+r"""\.))"""
    hex_floating_constant = '('+hex_prefix+'('+hex_digits+'|'+hex_fractional_constant+')'+binary_exponent_part+'[FfLl]?)'

    states = (
        # ppline: preprocessor line directives
        ('ppline', 'exclusive'),
        # ppident: preprocessor ident directives
        ('ppident', 'exclusive'),
        # pppragma: preprocessor pragma directives
        ('pppragma', 'exclusive'),
    )

    ##
    ## Rules for the normal state
    ##
    t_ignore = ' \t'

    # Newlines
    def t_NEWLINE(self, t):
        r'\n+'
        t.lexer.lineno += t.value.count("\n")

    ##
    ## Rules for the ppident state
    ##
    @lex.TOKEN(r'\#[ \t]*ident')
    def t_PREPROC_IDENT(self, t):
        self.lexer.begin('ppident')

    t_ppident_ignore = ' \t'

    def t_ppident_IDENT(self, t):
        r'[^\n]+'
        pass

    def t_ppident_NEWLINE(self, t):
        r'\n'
        t.lexer.begin('INITIAL')

    def t_ppident_error(self, t):
        self.error('invalid #ident directive', self._position(t))
        self.lexer.skip(1)


    ##
    ## Rules for the pppragma state
    ##
    @lex.TOKEN(r'\#[ \t]*pragma')
    def t_PREPROC_PRAGMA(self, t):
        self.lexer.begin('pppragma')
        self._pragma = []

    t_pppragma_ignore = ' \t'

    @lex.TOKEN(identifier)
    def t_pppragma_id(self, t):
        self._pragma.append(t)

    @lex.TOKEN(decimal_constant)
    def t_pppragma_nmber(self, t):
        self._pragma.append(t)

    def t_pppragma_whitespace(self, t):
        r'[ \t]+'

    def t_pppragma_else(self, t):
        r'[^\n]'
        self._pragma.append(t)

    def t_pppragma_NEWLINE(self, t):
        r'\n'
        self.lexer.begin('INITIAL')
        self._execute_pragma(self._pragma)
        self._pragma = []

    def t_pppragma_error(self, t):
        self.error('invalid #pragma directive', self._position(t))
        self.lexer.skip(1)


    ##
    ## Rules for the ppline state
    ##
    @lex.TOKEN(r'\#')
    def t_PREPROC_LINE(self, t):
        self.lexer.begin('ppline')

    @lex.TOKEN(string_literal)
    def t_ppline_FILENAME(self, t):
        if self.pp_line is None:
            self.error('filename before line number in #line', self._position(t))
            self.lexer.skip(1)
        else:
            self.pp_filename = t.value.lstrip('"').rstrip('"')

    @lex.TOKEN(decimal_constant)
    def t_ppline_LINE_NUMBER(self, t):
        if self.pp_line is None:
            self.pp_line = t.value
        else:
            # Ignore: GCC's cpp sometimes inserts a numeric flag
            # after the file name
            pass

    def t_ppline_NEWLINE(self, t):
        r'\n'
        if self.pp_line is None:
            self.error('line number missing in #line', self._position(t))
            #self.lexer.skip(1)
        else:
            self.lexer.lineno = int(self.pp_line)
            if self.pp_filename is not None:
                self.filename = self.pp_filename
        self.pp_line = None
        t.lexer.begin('INITIAL')

    def t_ppline_PPLINE(self, t):
        r'line'
        pass

    t_ppline_ignore = ' \t'

    def t_ppline_error(self, t):
        self.error('invalid #line directive', self._position(t))
        self.lexer.skip(1)


    # Operators
    t_PLUS              = r'\+'
    t_MINUS             = r'-'
    t_TIMES             = r'\*'
    t_DIVIDE            = r'/'
    t_MOD               = r'%'
    t_OR                = r'\|'
    t_AND               = r'&'
    t_NOT               = r'~'
    t_XOR               = r'\^'
    t_LSHIFT            = r'<<'
    t_RSHIFT            = r'>>'
    t_LOR               = r'\|\|'
    t_LAND              = r'&&'
    t_LNOT              = r'!'
    t_LT                = r'<'
    t_GT                = r'>'
    t_LE                = r'<='
    t_GE                = r'>='
    t_EQ                = r'=='
    t_NE                = r'!='

    # Assignment operators
    t_EQUALS            = r'='
    t_TIMESEQUAL        = r'\*='
    t_DIVEQUAL          = r'/='
    t_MODEQUAL          = r'%='
    t_PLUSEQUAL         = r'\+='
    t_MINUSEQUAL        = r'-='
    t_LSHIFTEQUAL       = r'<<='
    t_RSHIFTEQUAL       = r'>>='
    t_ANDEQUAL          = r'&='
    t_OREQUAL           = r'\|='
    t_XOREQUAL          = r'\^='

    # Increment/decrement
    t_PLUSPLUS          = r'\+\+'
    t_MINUSMINUS        = r'--'

    # ->
    t_ARROW             = r'->'

    # ?
    t_CONDOP            = r'\?'

    # Scope
    t_SCOPE             = r'::'

    # Delimeters
    t_LBRACE            = r'{'
    t_RBRACE            = r'}'
    t_LPAREN            = r'\('
    t_RPAREN            = r'\)'
    t_LBRACKET          = r'\['
    t_RBRACKET          = r'\]'
    t_COMMA             = r','
    t_PERIOD            = r'\.'
    t_SEMI              = r';'
    t_COLON             = r':'
    #t_ELLIPSIS          = r'\.\.\.'

    t_STRING_LITERAL = string_literal

    # The following floating and integer constants are defined as
    # functions to impose a strict order (otherwise, decimal
    # is placed before the others because its regex is longer,
    # and this is bad)
    @lex.TOKEN(floating_constant)
    def t_FLOAT_CONST(self, t):
        if t.value[-1] in 'fF':
            t.constant_value = float(t.value[:-1])
            t.constant_type = self.base_types['float']
        elif t.value[-1] in 'dD':
            t.constant_value = float(t.value[:-1])
            t.constant_type = self.base_types['double']
        else:
            assert t.value[-1] in '0123456789.'
            t.constant_value = float(t.value[:-1])
            t.constant_type = self.base_types['float']
        return t

    @lex.TOKEN(hex_floating_constant)
    def t_HEX_FLOAT_CONST(self, t):
        assert False
        return t

    @lex.TOKEN(hex_constant)
    def t_INT_CONST_HEX(self, t):
        t.constant_type = 'i32'
        while t.value[-1] in 'lLuU':
            if t.value[-1] in 'lL':
                t.constant_type = t.constant_type[0]+'64'
            else:
                t.constant_type = 'u' + t.constant_type[1:]
            t.value = t.value[:-1]
        t.constant_type = self.base_types[t.constant_type]
        t.constant_value = int(t.value, 16)
        return t

    @lex.TOKEN(bin_constant)
    def t_INT_CONST_BIN(self, t):
        t.constant_type = 'i32'
        while t.value[-1] in 'lLuU':
            if t.value[-1] in 'lL':
                t.constant_type = t.constant_type[0]+'64'
            else:
                t.constant_type = 'u' + t.constant_type[1:]
            t.value = t.value[:-1]
        t.constant_type = self.base_types[t.constant_type]
        t.constant_value = int(t.value, 2)
        return t

    @lex.TOKEN(bad_octal_constant)
    def t_BAD_CONST_OCT(self, t):
        msg = "Invalid octal constant"
        self.error(msg, self._position(t))
        self.lexer.skip(1)

    @lex.TOKEN(octal_constant)
    def t_INT_CONST_OCT(self, t):
        t.constant_type = 'i32'
        while t.value[-1] in 'lLuU':
            if t.value[-1] in 'lL':
                t.constant_type = t.constant_type[0]+'64'
            else:
                t.constant_type = 'u' + t.constant_type[1:]
            t.value = t.value[:-1]
        t.constant_type = self.base_types[t.constant_type]
        t.constant_value = int(t.value, 8)
        return t

    @lex.TOKEN(decimal_constant)
    def t_INT_CONST_DEC(self, t):
        t.constant_type = 'i32'
        while t.value[-1] in 'lLuU':
            if t.value[-1] in 'lL':
                t.constant_type = t.constant_type[0]+'64'
            else:
                t.constant_type = 'u' + t.constant_type[1:]
            t.value = t.value[:-1]
        t.constant_type = self.base_types[t.constant_type]
        t.constant_value = int(t.value, 10)
        return t

    # Must come before bad_char_const, to prevent it from
    # catching valid char constants as invalid
    #
    @lex.TOKEN(char_const)
    def t_CHAR_CONST(self, t):
        exec("x=u%s"%t.value)
        t.constant_value = ord(x)
        if t.constant_value > 255:
            self.error("invalid character constant", self._position(t))
        t.constant_type = self.base_types['u8']
        return t

    @lex.TOKEN(wchar_const)
    def t_WCHAR_CONST(self, t):
        exec("x=u%s"%t.value[1:])
        t.constant_value = ord(x)
        t.constant_type = self.base_types['u32']
        return t

    @lex.TOKEN(unmatched_quote)
    def t_UNMATCHED_QUOTE(self, t):
        msg = "Unmatched '"
        self.error(msg, self._position(t))
        self.lexer.skip(1)

    @lex.TOKEN(bad_char_const)
    def t_BAD_CHAR_CONST(self, t):
        msg = "Invalid char constant %s" % t.value
        self.error(msg, self._position(t))
        self.lexer.skip(1)

    @lex.TOKEN(wstring_literal)
    def t_WSTRING_LITERAL(self, t):
        return t

    # unmatched string literals are caught by the preprocessor

    @lex.TOKEN(bad_string_literal)
    def t_BAD_STRING_LITERAL(self, t):
        msg = "String contains invalid escape code"
        self.error(msg, self._position(t))
        self.lexer.skip(1)

    @lex.TOKEN(identifier)
    def t_ID(self, t):
        t.type = self.keyword_map.get(t.value, "ID")
        return t

    def t_error(self, t):
        self.error('Illegal character %s' % repr(t.value[0]), self._position(t))
        self.lexer.skip(1)
