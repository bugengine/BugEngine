from ply.lex import TOKEN
from .integer import *
from .character import *
from .floating import *
from .string import *
from .bool import *
from .pointer import *
from .user import *

import decimal
from be_typing import TYPE_CHECKING

decimal.getcontext().prec = 24


@TOKEN(floating_literal)
def t_FLOATING_LITERAL(t):
    # type: (LexToken) -> Optional[LexToken]
    setattr(t, 'constant_value', decimal.Decimal(t.value[:-1]))
    #if t.value[-1] in 'fF':
    #    setattr(t, 'constant_type', self._base_types['float'])
    #elif t.value[-1] in 'dD':
    #    setattr(t, 'constant_type', self._base_types['double'])
    #else:
    #    assert t.value[-1] in '0123456789.'
    #    setattr(t, 'constant_type', self._base_types['float'])
    return t


@TOKEN(integer_literal)
def t_INTEGER_LITERAL(t):
    # type: (LexToken) -> Optional[LexToken]
    return t


"""@lex.TOKEN(hex_floating_constant)
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
"""

if TYPE_CHECKING:
    from typing import Optional
    from ply.lex import LexToken
    from ....cpp_lexer import CppLexer