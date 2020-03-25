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
        simple-type-specifier : OP_SCOPE nested-name-specifier-opt type-name                    %prec OP_SCOPE
                              | nested-name-specifier type-name                                 %prec OP_SCOPE
                              | type-name                                                       %prec SCOPE_REDUCTION
                              | OP_SCOPE nested-name-specifier unknown-template-id
                              | nested-name-specifier unknown-template-id
                              | KW_CHAR
                              | KW_CHAR16_T
                              | KW_CHAR32_T
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
                              | KW_AUTO
                              | decltype-specifier                                              %prec SCOPE_REDUCTION
    """


def p_type_name(p):
    # type: (YaccProduction) -> None
    """
        type-name : class-name
                  | enum-name
                  | typedef-name
                  | class-template-id
    """


def p_decltype_specifier(p):
    # type: (YaccProduction) -> None
    """
        decltype-specifier : KW_DECLTYPE LPAREN expression RPAREN
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction