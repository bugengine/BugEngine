"""
simple-type-specifier:
      ::opt nested-name-specifieropt type-name
      ::opt nested-name-specifier template simple-template-id
      char
      char16_t     C++0x
      char32_t     C++0x
      wchar_t
      bool
      short
      int
      long
      signed
      unsigned
      float
      double
      void
      auto     C++0x
      decltype-specifier     C++0x
type-name:
      class-name
      enum-name
      typedef-name
      simple-template-id     C++0x
decltype-specifier:
      decltype ( expression )     C++0x
"""

from ....cxx_parser import cxx98, cxx11, disabled
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("simple-type-specifier : id-type [prec:nonassoc,0][split] | builtin-type-specifier")
@cxx98
def p_simple_type_specifier(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("builtin-type-specifier : 'char' | 'wchar_t' | 'bool' | 'short' | 'int' | 'long'")
@glrp.rule("builtin-type-specifier : 'signed' | 'unsigned'")
@glrp.rule("builtin-type-specifier : 'float' | 'double'")
@glrp.rule("builtin-type-specifier : 'void'")
@cxx98
def p_builtin_type_specifier(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("builtin-type-specifier : 'auto' | decltype-specifier")
@cxx11
def p_simple_type_specifier_cxx11(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@disabled
def p_type_name(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    """
        type-name : class-name
                  | enum-name
                  | typedef-name
                  | simple-template-id
    """


@glrp.rule("decltype-specifier : 'decltype' '(' expression ')'")
@cxx11
def p_decltype_specifier(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ....cxx_parser import CxxParser