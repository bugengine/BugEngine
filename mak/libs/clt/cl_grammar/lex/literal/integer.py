"""
integer-literal:
  	decimal-literal integer-suffixopt
  	octal-literal integer-suffixopt
  	hexadecimal-literal integer-suffixopt
decimal-literal:
  	nonzero-digit
  	decimal-literal digit
octal-literal:
  	0
  	octal-literal octal-digit
hexadecimal-literal:
  	0x hexadecimal-digit
  	0X hexadecimal-digit
  	hexadecimal-literal hexadecimal-digit
"""

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
