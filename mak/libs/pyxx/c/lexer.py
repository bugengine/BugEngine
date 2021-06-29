import glrp
import decimal
from be_typing import TYPE_CHECKING

decimal.getcontext().prec = 24

_nonzero_digit = '[1-9]'
_digit = '[0-9]'
_octal_digit = '[0-7]'
_hexadecimal_digit = '[0-9a-fA-F]'
_hexadecimal_prefix = '(?:0[xX])'

_decimal_constant = '(?:%(_nonzero_digit)s%(_digit)s*)' % locals()
_octal_constant = '(?:0%(_octal_digit)s*)' % locals()
_hexadecimal_constant = '(?:%(_hexadecimal_prefix)s%(_hexadecimal_digit)s+)' % locals()
_unsigned_suffix = '[uU]'
_long_suffix = '[lL]'
_long_long_suffix = '(?:(?:ll)|(?:LL))'
_integer_suffix = '(?:(?:%(_unsigned_suffix)s%(_long_suffix)s?)|(?:%(_unsigned_suffix)s%(_long_long_suffix)s)|(?:%(_long_suffix)s%(_unsigned_suffix)s?)|(?:%(_long_long_suffix)s%(_unsigned_suffix)s?))' % locals(
)
_integer_constant = '(?:%(_decimal_constant)s|%(_octal_constant)s|%(_hexadecimal_constant)s)%(_integer_suffix)s?' % locals(
)

_floating_suffix = '(?:f|l|F|L|(?:df)|(?:dd)|(?:dl)|(?:DF)|(?:DD)|(?:DL))'

_fractional_constant = r'(?:(?:[0-9]*\.[0-9]+)|(?:[0-9]+\.))'
_exponent_part = r'(?:[eE][+\-]?[0-9]+)'
_decimal_floating_constant = '(?:(?:%(_fractional_constant)s%(_exponent_part)s?%(_floating_suffix)s?)|(?:[0-9]+%(_exponent_part)s%(_floating_suffix)s?))' % locals(
)

_hexadecimal_fractional_constant = '(?:(?:%(_hexadecimal_digit)s*\\.%(_hexadecimal_digit)s+)|(?:%(_hexadecimal_digit)s+\\.))' % locals(
)
_binary_exponent_part = r'(?:[pP][+\-]?[0-9]+)'
_hexadecimal_floating_constant = '(?:(?:%(_hexadecimal_prefix)s%(_hexadecimal_fractional_constant)s)|(?:%(_binary_exponent_part)s%(_floating_suffix)s?)|(?:%(_hexadecimal_prefix)s%(_hexadecimal_digit)s+)|(?:%(_binary_exponent_part)s%(_floating_suffix)s))' % locals(
)

_floating_constant = '%(_decimal_floating_constant)s|%(_hexadecimal_floating_constant)s' % locals()

_identifier = r'[a-zA-Z_][a-zA-Z_0-9]*'

_encoding_prefix = '(?:(?:u8?)|U|L)'
_simple_escape_sequence = r'(?:\\[\'"\?\\abfnrtv])'
_octal_escape_sequence = '(?:\\\\%(_octal_digit)s%(_octal_digit)s?%(_octal_digit)s?)' % locals()
_hexadecimal_escape_sequence = '(?:\\\\x%(_hexadecimal_digit)s+)' % locals()
_hexadecimal_quad = '%(_hexadecimal_digit)s%(_hexadecimal_digit)s%(_hexadecimal_digit)s%(_hexadecimal_digit)s' % locals(
)
_universal_character_name = '(?:(?:\\\\u%(_hexadecimal_quad)s)|(?:\\\\U%(_hexadecimal_quad)s%(_hexadecimal_quad)s))' % locals(
)
_escape_sequence = '(?:%(_simple_escape_sequence)s|%(_octal_escape_sequence)s|%(_hexadecimal_escape_sequence)s|%(_universal_character_name)s)' % locals(
)
_c_char = '[^\\\'\\\\\\n]|%(_escape_sequence)s' % locals()
_c_char_sequence = '%(_c_char)s+' % locals()
_character_constant = '%(_encoding_prefix)s?\'%(_c_char_sequence)s\'' % locals()

_s_char = '(?:[^"\\n]|%(_escape_sequence)s)' % locals()
_string_literal = '(?:%(_encoding_prefix)s"%(_s_char)s*")' % locals()

_keywords = (
    'auto',
    'break',
    'case',
    'char',
    'const',
    'continue',
    'default',
    'do',
    'double',
    'else',
    'enum',
    'extern',
    'float',
    'for',
    'goto',
    'if',
    'inline',
    'int',
    'long',
    'register',
    'restrict',
    'return',
    'short',
    'signed',
    'sizeof',
    'static',
    'struct',
    'switch',
    'typedef',
    'union',
    'unsigned',
    'void',
    'volatile',
    'while',
    '_Alignas',
    '_Alignof',
    '_Atomic',
    '_Bool',
    '_Complex',
    '_Decimal128',
    '_Decimal32',
    '_Decimal64',
    '_Generic',
    '_Imaginary',
    '_Noreturn',
    '_Static_assert',
    '_Thread_local',
)                      # type: Tuple[str,...]


class C89Lexer(glrp.Lexer):
    tokens = ('enumeration-constant', ) + _keywords
    keywords = _keywords

    def __init__(self):
        # type: () -> None
        glrp.Lexer.__init__(self)
        self._enum_constants = set([]) # type: Set[str]

    @glrp.token(r"\[", "[")
    @glrp.token(r"\]", "]")
    @glrp.token(r"\(", "(")
    @glrp.token(r"\)", ")")
    @glrp.token(r"\{", "{")
    @glrp.token(r"\}", "}")
    @glrp.token(r"\.", ".")
    @glrp.token(r"->", "->")
    @glrp.token(r"\+\+", "++")
    @glrp.token(r"--", "--")
    @glrp.token(r"&", "&")
    @glrp.token(r"\*", "*")
    @glrp.token(r"\+", "+")
    @glrp.token(r"-", "-")
    @glrp.token(r"~", "~")
    @glrp.token(r"!", "!")
    @glrp.token(r"/", "/")
    @glrp.token(r"%", "%")
    @glrp.token(r"<<", "<<")
    @glrp.token(r">>", ">>")
    @glrp.token(r"<", "<")
    @glrp.token(r">", ">")
    @glrp.token(r"<=", "<=")
    @glrp.token(r">=", ">=")
    @glrp.token(r"==", "==")
    @glrp.token(r"!=", "!=")
    @glrp.token(r"\^", "^")
    @glrp.token(r"\|", "|")
    @glrp.token(r"&&", "&&")
    @glrp.token(r"\|\|", "||")
    @glrp.token(r"\?", "?")
    @glrp.token(r":", ":")
    @glrp.token(r"::", "::")
    @glrp.token(r";", ";")
    @glrp.token(r"...", "...")
    @glrp.token(r"=", "=")
    @glrp.token(r"\*=", "*=")
    @glrp.token(r"/=", "/=")
    @glrp.token(r"%=", "%=")
    @glrp.token(r"\+=", "+=")
    @glrp.token(r"-=", "-=")
    @glrp.token(r"<<=", "<<=")
    @glrp.token(r">>=", ">>=")
    @glrp.token(r"&=", "&=")
    @glrp.token(r"^=", "^=")
    @glrp.token(r"|=", "|=")
    @glrp.token(r",", ",")
    def punctuation(self, token):
        # type: (glrp.Token) -> glrp.Token
        return token

    @glrp.token(r'\#([^\\\n]|(?:\\.)|(?:\\\n))*', 'preprocessor')
    def preprocessor(self, t):
        # type: (glrp.Token) -> Optional[glrp.Token]
        #if t.value.find('include') != -1:
        #    t.lexer.includes.append(t.value)
        return None

    @glrp.token(r'[ \t\n]+', 'whitespace')
    def skip(self, t):
        # type: (glrp.Token) -> Optional[glrp.Token]
        return None

    @glrp.token(r'/\*(?:.|\n)*?\*/', 'block-comment')
    @glrp.token(r'\//(?:[^\\\n]|(?:\\.)|(?:\\\n))*', 'line-comment')
    def comment(self, t):
        # type: (glrp.Token) -> Optional[glrp.Token]
        comment = t.text()
        return None
        #if comment[1] == '/' and comment[2] in ('/', '!') or comment[1] == '*' and comment[2] in ('*', '!'):
        #    self.set_token_type(t, 'doxycomment')
        #    return t
        #else:
        #    return None

    @glrp.token(_identifier, 'identifier')
    def identifier(self, t):
        # type: (glrp.Token) -> Optional[glrp.Token]
        t.value = t.text()
        if t.value in self.keywords:
            self.set_token_type(t, t.value)
        if t.value in self._enum_constants:
            self.set_token_type(t, 'enumeration-constant')
        return t

    @glrp.token(_floating_constant, 'floating-constant')
    def floating_constant(self, t):
        # type: (glrp.Token) -> Optional[glrp.Token]
        if t.text()[-1] in 'fFdD':
            t.value = decimal.Decimal(t.text()[:-1])
        else:
            t.value = decimal.Decimal(t.text())
        return t

    @glrp.token(_integer_constant, 'integer-constant')
    def integer_constant(self, t):
        # type: (glrp.Token) -> Optional[glrp.Token]
        return t

    @glrp.token(_character_constant, 'character-constant')
    def character_constant(self, t):
        # type: (glrp.Token) -> Optional[glrp.Token]
        return t

    @glrp.token(_string_literal, 'string-literal')
    def string_literal(self, t):
        # type: (glrp.Token) -> Optional[glrp.Token]
        t.value = t.text()[1:-1]
        return t


class C99Lexer(C89Lexer):
    pass


class C11Lexer(C99Lexer):
    pass


class C17Lexer(C11Lexer):
    pass


if TYPE_CHECKING:
    from typing import Optional, Set, Tuple