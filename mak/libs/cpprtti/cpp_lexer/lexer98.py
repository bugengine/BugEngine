from .cpp_lexer import CppLexer
from .keyword import keywords_98
from ply.lex import TOKEN
import decimal
from be_typing import TYPE_CHECKING

decimal.getcontext().prec = 24

_nonzero_digit = '[1-9]'
_digit = '[0-9]'
_octal_digit = '[0-7]'
_hexadecimal_digit = '[0-9a-fA-F]'
_binary_digit = '[01]'

_decimal_literal = '(%s%s*)' % (_nonzero_digit, _digit)
_octal_literal = '(0%s*)' % _octal_digit
_hexadecimal_literal = """(0(x|X)%s+)""" % _hexadecimal_digit
_binary_literal = """(0(b|B)%s+)""" % _binary_digit

_unsigned_suffix = '(u|U)'
_long_suffix = '(l|L)'
_long_long_suffix = '((ll)|(LL))'

_integer_suffix = '((%s%s?)|(%s%s?)|(%s%s?)|(%s%s?))' % (
    _unsigned_suffix, _long_long_suffix, _unsigned_suffix, _long_suffix, _long_suffix, _unsigned_suffix,
    _long_long_suffix, _unsigned_suffix
)

integer_literal = '(%s%s)|(%s%s)|(%s%s)|(%s%s)' % (
    _decimal_literal, _integer_suffix, _octal_literal, _integer_suffix, _hexadecimal_literal, _integer_suffix,
    _binary_literal, _integer_suffix
)

_digit_sequence = '(%s+)' % _digit
_sign = '(\\+|-)'

_fractional_constant = '((%s?\\.%s)|(%s\\.))' % (_digit_sequence, _digit_sequence, _digit_sequence)
_exponent_part = '((e|E)%s?%s)' % (_sign, _digit_sequence)

_floating_suffix = '(f|l|F|L)'

floating_literal = '(%s%s?%s?)|(%s%s%s?)' % (
    _fractional_constant, _exponent_part, _floating_suffix, _digit_sequence, _exponent_part, _floating_suffix
)


class CppLexer98(CppLexer):
    keywords = CppLexer.keywords + keywords_98
    tokens = CppLexer.tokens + (
        'IDENTIFIER',
        'FLOATING_LITERAL',
        'INTEGER_LITERAL',
        'STRING_LITERAL',
        'OP_PLUS',
        'OP_MINUS',
        'OP_TIMES',
        'OP_DIVIDE',
        'OP_MOD',
        'OP_OR',
        'OP_AND',
        'OP_NOT',
        'OP_XOR',
        'OP_LSHIFT',
        'OP_RSHIFT',
        'OP_LOR',
        'OP_LAND',
        'OP_LNOT',
        'OP_LT',
        'OP_GT',
        'OP_LE',
        'OP_GE',
        'OP_EQ',
        'OP_NE',
        'OP_EQUALS',
        'OP_TIMESEQUAL',
        'OP_DIVEQUAL',
        'OP_MODEQUAL',
        'OP_PLUSEQUAL',
        'OP_MINUSEQUAL',
        'OP_LSHIFTEQUAL',
        'OP_RSHIFTEQUAL',
        'OP_ANDEQUAL',
        'OP_OREQUAL',
        'OP_XOREQUAL',
        'OP_PLUSPLUS',
        'OP_MINUSMINUS',
        'OP_ARROW',
        'OP_COND',
        'OP_SCOPE',
        'OP_ARROWMEMBER',
        'OP_PERIODMEMBER',
        'LBRACE',
        'RBRACE',
        'LPAREN',
        'RPAREN',
        'LBRACKET',
        'RBRACKET',
        'LANGLE',
        'RANGLE',
        'COMMA',
        'PERIOD',
        'SEMI',
        'COLON',
        'ELLIPSIS',
    ) + tuple('KW_' + kw.upper() for kw in keywords_98)

    t_ignore = ' \t'

    t_OP_PLUS = r'\+'
    t_OP_MINUS = r'-'
    t_OP_TIMES = r'\*'
    t_OP_DIVIDE = r'/'
    t_OP_MOD = r'%'
    t_OP_OR = r'\|'
    t_OP_AND = r'&'
    t_OP_NOT = r'~'
    t_OP_XOR = r'\^'
    t_OP_LSHIFT = r'<<'
    t_OP_RSHIFT = r'>>'
    t_OP_LOR = r'\|\|'
    t_OP_LAND = r'&&'
    t_OP_LNOT = r'!'
    t_OP_LT = r'<'
    t_OP_GT = r'>'
    t_OP_LE = r'<='
    t_OP_GE = r'>='
    t_OP_EQ = r'=='
    t_OP_NE = r'!='

    # Assignment operators
    t_OP_EQUALS = r'='
    t_OP_TIMESEQUAL = r'\*='
    t_OP_DIVEQUAL = r'/='
    t_OP_MODEQUAL = r'%='
    t_OP_PLUSEQUAL = r'\+='
    t_OP_MINUSEQUAL = r'-='
    t_OP_LSHIFTEQUAL = r'<<='
    t_OP_RSHIFTEQUAL = r'>>='
    t_OP_ANDEQUAL = r'&='
    t_OP_OREQUAL = r'\|='
    t_OP_XOREQUAL = r'\^='

    # Increment/decrement
    t_OP_PLUSPLUS = r'\+\+'
    t_OP_MINUSMINUS = r'--'

    # ->, ->*, .*
    t_OP_ARROW = r'->'
    t_OP_ARROWMEMBER = r'->\*'
    t_OP_PERIODMEMBER = r'\.\*'

    # ?
    t_OP_COND = r'\?'

    # Scope
    t_OP_SCOPE = r'::'

    # Punctuation
    t_COMMA = r','
    t_PERIOD = r'\.'
    t_SEMI = r';'
    t_COLON = r':'
    t_ELLIPSIS = r'\.\.\.'
    t_LBRACKET = r'\['
    t_LBRACE = r'\{'
    t_LPAREN = r'\('
    t_RBRACKET = r'\]'
    t_RBRACE = r'\}'
    t_RPAREN = r'\)'

    def t_preprocessor(self, t):
        # type: (LexToken) -> Optional[LexToken]
        r'\#([^\\\n]|(\\.)|(\\\n))*'
        self._lineno += t.value.count('\n')
        #if t.value.find('include') != -1:
        #    t.lexer.includes.append(t.value)
        return None

    def t_comment_block(self, t):
        # type: (LexToken) -> Optional[LexToken]
        r'/\*(.|\n)*?\*/'
        self._lineno += t.value.count('\n')
        return None

    def t_comment_line(self, t):
        # type: (LexToken) -> Optional[LexToken]
        r'\//([^\\\n]|(\\.)|(\\\n))*'
        self._lineno += t.value.count('\n')
        return None

    def t_NEWLINE(self, t):
        # type: (LexToken) -> Optional[LexToken]
        r'\n+'
        self._lineno += t.value.count("\n")
        return None

    def t_IDENTIFIER(self, t):
        # type: (LexToken) -> Optional[LexToken]
        r'[a-zA-Z_][a-zA-Z_0-9]*'
        if t.value in self.keywords:
            t.type = 'KW_' + t.value.upper()
        return t

    def t_error(self, t):
        # type: (LexToken) -> Optional[LexToken]
        self._lexer.skip(1)
        return None

    @TOKEN(floating_literal)
    def t_FLOATING_LITERAL(self, t):
        # type: (LexToken) -> Optional[LexToken]
        if t.value[-1] in 'fFdD':
            setattr(t, 'constant_value', decimal.Decimal(t.value[:-1]))
        else:
            setattr(t, 'constant_value', decimal.Decimal(t.value))
        return t

    @TOKEN(integer_literal)
    def t_INTEGER_LITERAL(self, t):
        # type: (LexToken) -> Optional[LexToken]
        return t


if TYPE_CHECKING:
    from typing import Optional, Tuple
    from ply.lex import LexToken