"""
enum-name:
      identifier
enum-specifier:
      enum-head { enumerator-listopt }     C++0x
      enum-head { enumerator-list , }     C++0x
enum-head:
      enum-key attribute-specifier-seqopt identifieropt enum-baseopt     C++0x
      enum-key attribute-specifier-seqopt nested-name-specifier identifier enum-baseopt     C++0x
opaque-enum-declaration:
      enum-key attribute-specifier-seqopt identifier enum-baseopt ;     C++0x
enum-key:
      enum     C++0x
      enum class     C++0x
      enum struct     C++0x
enum-base:
      : type-specifier-seq     C++0x
enumerator-list:
      enumerator-definition     C++0x
      enumerator-list , enumerator-definition     C++0x
enumerator-definition:
      enumerator
      enumerator = constant-expression
enumerator:
      identifier
"""

from be_typing import TYPE_CHECKING


def p_enum_name(p):
    # type: (YaccProduction) -> None
    """
        enum-name : IDENTIFIER
    """


def p_enum_specifier(p):
    # type: (YaccProduction) -> None
    """
        enum-specifier : enum-head LBRACE enumerator-list? RBRACE
                       | enum-head LBRACE enumerator-list COMMA RBRACE
    """


def p_enum_head(p):
    # type: (YaccProduction) -> None
    """
        enum-head : enum-key attribute-specifier-seq? identifier? enum-base?
                  | enum-key attribute-specifier-seq? nested-name-specifier identifier enum-base?
    """


def p_opaque_enum_declaration(p):
    # type: (YaccProduction) -> None
    """
        opaque-enum-declaration : enum-key attribute-specifier-seq? identifier enum-base? SEMI
    """


def p_enum_key(p):
    # type: (YaccProduction) -> None
    """
        enum-key : KW_ENUM
                 | KW_ENUM KW_CLASS
                 | KW_ENUM KW_STRUCT
    """


def p_enum_base(p):
    # type: (YaccProduction) -> None
    """
        enum-base : COLON type-specifier-seq
    """


def p_enumerator_list(p):
    # type: (YaccProduction) -> None
    """
        enumerator-list : enumerator-definition
                        | enumerator-list COMMA enumerator-definition
    """


def p_enumerator_definition(p):
    # type: (YaccProduction) -> None
    """
        enumerator-definition : enumerator
                              | enumerator OP_EQUALS constant-expression
    """


def p_enumerator(p):
    # type: (YaccProduction) -> None
    """
        enumerator : identifier
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction