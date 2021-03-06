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

from ...cpp_parser import cpp98, cpp11, disabled
from be_typing import TYPE_CHECKING


@disabled
def p_enum_name(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        enum-name : IDENTIFIER
    """


@cpp98
def p_enum_specifier(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        enum-specifier : enum-head LBRACE enumerator-list? RBRACE
                       | enum-head LBRACE enumerator-list COMMA RBRACE
    """


@cpp98
def p_enum_head(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        enum-head : enum-key attribute-specifier-seq? id-type? enum-base
    """


@cpp11
def p_opaque_enum_declaration(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        opaque-enum-declaration : enum-key attribute-specifier-seq? IDENTIFIER enum-base SEMI
    """


@cpp98
def p_enum_key(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        enum-key : KW_ENUM
    """


@cpp11
def p_enum_key_cpp11(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        enum-key : KW_ENUM KW_CLASS
                 | KW_ENUM KW_STRUCT
    """


@cpp98
def p_enum_base_empty(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        enum-base : empty
    """


@cpp11
def p_enum_base(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        enum-base : COLON type-specifier-seq
    """


@cpp98
def p_enumerator_list(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        enumerator-list : enumerator-definition
                        | enumerator-list COMMA enumerator-definition
    """


@cpp98
def p_enumerator_definition(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        enumerator-definition : enumerator
                              | enumerator OP_EQUALS constant-expression
    """


@cpp98
def p_enumerator(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        enumerator : IDENTIFIER
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser