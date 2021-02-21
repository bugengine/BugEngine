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

from be_typing import TYPE_CHECKING


def p_simple_type_specifier(p):
    # type: (YaccProduction) -> None
    """
        simple-type-specifier : simple-type-modifier? id-expression simple-type-modifier?
                              | simple-type-modifier? KW_CHAR simple-type-modifier?
                              | simple-type-modifier? KW_CHAR16_T simple-type-modifier?
                              | simple-type-modifier? KW_CHAR32_T simple-type-modifier?
                              | simple-type-modifier? KW_WCHAR_T simple-type-modifier?
                              | simple-type-modifier? KW_BOOL simple-type-modifier?
                              | simple-type-modifier KW_INT? simple-type-modifier?
                              | simple-type-modifier? KW_FLOAT simple-type-modifier?
                              | simple-type-modifier? KW_DOUBLE simple-type-modifier?
                              | simple-type-modifier? KW_VOID simple-type-modifier?
                              | simple-type-modifier? KW_AUTO simple-type-modifier?
                              | simple-type-modifier? decltype-specifier simple-type-modifier?
    """


def p_simple_type_modifier(p):
    # type: (YaccProduction) -> None
    """
        simple-type-modifier : simple-type-modifier? KW_SHORT
                             | simple-type-modifier? KW_LONG
                             | simple-type-modifier? KW_SIGNED
                             | simple-type-modifier? KW_UNSIGNED
    """


#def p_type_name(p):
#    # type: (YaccProduction) -> None
#    """
#        type-name : IDENTIFIER
#                  | simple-template-id
#    """


def p_decltype_specifier(p):
    # type: (YaccProduction) -> None
    """
        decltype-specifier : KW_DECLTYPE LPAREN expression RPAREN
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction