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

from ....cpp_parser import cpp98, cpp11, disabled
from be_typing import TYPE_CHECKING


@cpp98
def p_simple_type_specifier(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        simple-type-specifier : id-expression
                              | builtin-type-specifier
    """


@cpp98
def p_builtin_type_specifier(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        builtin-type-specifier : KW_CHAR
                               | KW_WCHAR_T
                               | KW_BOOL
                               | KW_SHORT
                               | KW_INT
                               | KW_LONG
                               | KW_SIGNED
                               | KW_UNSIGNED
                               | KW_FLOAT
                               | KW_DOUBLE
                               | KW_VOID
    """


@cpp11
def p_simple_type_specifier_cpp11(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        simple-type-specifier : KW_AUTO
                              | decltype-specifier
    """


@disabled
def p_type_name(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        type-name : class-name
                  | enum-name
                  | typedef-name
                  | simple-template-id
    """


@cpp11
def p_decltype_specifier(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        decltype-specifier : KW_DECLTYPE LPAREN expression RPAREN
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ....cpp_parser import CppParser