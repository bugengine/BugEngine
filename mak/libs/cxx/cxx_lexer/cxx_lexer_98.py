from .cxx_lexer import CxxLexer
from .keyword import keywords_98
import glrp
import decimal
from be_typing import TYPE_CHECKING

decimal.getcontext().prec = 24

_nonzero_digit = '[1-9]'
_digit = '[0-9]'
_octal_digit = '[0-7]'
_hexadecimal_digit = '[0-9a-fA-F]'
_binary_digit = '[01]'

_decimal_literal = '(?:%s%s*)' % (_nonzero_digit, _digit)
_octal_literal = '(?:0%s*)' % _octal_digit
_hexadecimal_literal = """(?:0(?:x|X)%s+)""" % _hexadecimal_digit
_binary_literal = """(?:0(?:b|B)%s+)""" % _binary_digit

_unsigned_suffix = '(?:u|U)'
_long_suffix = '(?:l|L)'
_long_long_suffix = '(?:(?:ll)|(?:LL))'

_integer_suffix = '(?:(?:%s%s?)|(?:%s%s?)|(?:%s%s?)|(?:%s%s?))' % (
    _unsigned_suffix, _long_long_suffix, _unsigned_suffix, _long_suffix, _long_suffix, _unsigned_suffix,
    _long_long_suffix, _unsigned_suffix
)

integer_literal = '(?:%s%s)|(?:%s%s)|(?:%s%s)|(?:%s%s)' % (
    _decimal_literal, _integer_suffix, _octal_literal, _integer_suffix, _hexadecimal_literal, _integer_suffix,
    _binary_literal, _integer_suffix
)

_digit_sequence = '(%s+)' % _digit
_sign = '(\\+|-)'

_fractional_constant = '(?:(?:%s?\\.%s)|(?:%s\\.))' % (_digit_sequence, _digit_sequence, _digit_sequence)
_exponent_part = '(?:(?:e|E)%s?%s)' % (_sign, _digit_sequence)

_floating_suffix = '(?:f|l|F|L)'

floating_literal = '(?:%s%s?%s?)|(?:%s%s%s?)' % (
    _fractional_constant, _exponent_part, _floating_suffix, _digit_sequence, _exponent_part, _floating_suffix
)

_simple_escape = r"""([a-zA-Z\\?'"])"""
_octal_escape = r"""([0-7]{1,3})"""
_hex_escape = r"""(x[0-9a-fA-F]+)"""

_escape_sequence = r"""(\\(""" + _simple_escape + '|' + _octal_escape + '|' + _hex_escape + '))'
_cconst_char = r"""([^'\\\n]|""" + _escape_sequence + ')'
character_literal = "'" + _cconst_char + "'"
wide_character_literal = 'L' + character_literal

# String literals (K&R2: A.2.6)
_string_char = r"""([^"\\\n]|""" + _escape_sequence + ')'
string_literal = '"' + _string_char + '*"'
wide_string_literal = 'L' + string_literal


class CxxLexer98(CxxLexer):
    keywords = CxxLexer.keywords + keywords_98
    tokens = CxxLexer.tokens + keywords_98

    # arithmetic operators
    @glrp.token(r'\+', '+')
    @glrp.token(r'-')
    @glrp.token(r'\*', '*')
    @glrp.token(r'/')
    @glrp.token(r'%')
    @glrp.token(r'\|', '|')
    @glrp.token(r'&')
    @glrp.token(r'~')
    @glrp.token(r'\^', '^')
    @glrp.token(r'<<')
    @glrp.token(r'>>')
    # logic operators
    @glrp.token(r'\|\|', '||')
    @glrp.token(r'&&')
    @glrp.token(r'!')
    @glrp.token(r'<')
    @glrp.token(r'>')
    @glrp.token(r'<=')
    @glrp.token(r'>=')
    @glrp.token(r'==')
    @glrp.token(r'!=')
    # assignment operators
    @glrp.token(r'=')
    @glrp.token(r'\*=', '*=')
    @glrp.token(r'/=')
    @glrp.token(r'%=')
    @glrp.token(r'\+=', '+=')
    @glrp.token(r'-=')
    @glrp.token(r'<<=')
    @glrp.token(r'>>=')
    @glrp.token(r'&=')
    @glrp.token(r'\|=', '|=')
    @glrp.token(r'\^=', '^=')
    @glrp.token(r'\+\+', '++')
    @glrp.token(r'--')
    # member access operators
    @glrp.token(r'->')
    @glrp.token(r'->\*', '->*')
    @glrp.token(r'\.\*', '.*')
    # conditional operator
    @glrp.token(r'\?', '?')
    # scope operator
    @glrp.token(r'::')
    # punctuation
    @glrp.token(r',')
    @glrp.token(r'\.', '.')
    @glrp.token(r';')
    @glrp.token(r':')
    @glrp.token(r'\.\.\.', '...')
    @glrp.token(r'\[', '[')
    @glrp.token(r'\{', '{')
    @glrp.token(r'\(', '(')
    @glrp.token(r'\]', ']')
    @glrp.token(r'\}', '}')
    @glrp.token(r'\)', ')')
    def tok(self, token):
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

    @glrp.token(r'/\*(.|\n)*?\*/', 'block-comment')
    @glrp.token(r'\//(?:[^\\\n]|(?:\\.)|(?:\\\n))*', 'line-comment')
    def comment(self, t):
        # type: (glrp.Token) -> Optional[glrp.Token]
        comment = t.text()
        if comment[1] == '/' and comment[2] in ('/', '!') or comment[1] == '*' and comment[2] in ('*', '!'):
            self.set_token_type(t, 'doxycomment')
            return t
        else:
            return None

    @glrp.token(r'[a-zA-Z_][a-zA-Z_0-9]*', 'identifier')
    def identifier(self, t):
        # type: (glrp.Token) -> Optional[glrp.Token]
        t.value = t.text()
        if t.value in self.keywords:
            self.set_token_type(t, t.value)
        return t

    @glrp.token(floating_literal, 'floating-literal')
    def floating_literal(self, t):
        # type: (glrp.Token) -> Optional[glrp.Token]
        if t.text()[-1] in 'fFdD':
            t.value = decimal.Decimal(t.text()[:-1])
        else:
            t.value = decimal.Decimal(t.text())
        return t

    @glrp.token(integer_literal, 'integer-literal')
    def integer_literal(self, t):
        # type: (glrp.Token) -> Optional[glrp.Token]
        return t

    @glrp.token(string_literal, 'string-literal')
    def string_literal(self, t):
        # type: (glrp.Token) -> Optional[glrp.Token]
        t.value = t.text()[1:-1]
        return t


if TYPE_CHECKING:
    from typing import Optional, Tuple
