"""
floating-literal:
  	fractional-constant exponent-partopt floating-suffixopt
  	digit-sequence exponent-part floating-suffixopt
fractional-constant:
  	digit-sequenceopt . digit-sequence
  	digit-sequence .
exponent-part:
  	e signopt digit-sequence
  	E signopt digit-sequence
sign:
  	+
  	-
digit-sequence:
  	digit
  	digit-sequence digit
floating-suffix: 
  	f
  	l
  	F
  	L
"""

_digit = '[0-9]'
_digit_sequence = '(%s+)' % _digit
_sign = '(\\+|-)'

_fractional_constant = '((%s?\\.%s)|(%s\\.))' % (_digit_sequence, _digit_sequence, _digit_sequence)
_exponent_part = '((e|E)%s?%s)' % (_sign, _digit_sequence)

_floating_suffix = '(f|l|F|L)'

floating_literal = '(%s%s?%s?)|(%s%s%s?)' % (
    _fractional_constant, _exponent_part, _floating_suffix, _digit_sequence, _exponent_part, _floating_suffix
)