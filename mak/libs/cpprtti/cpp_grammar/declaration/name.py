"""
type-id:
      type-specifier-seq abstract-declaratoropt
abstract-declarator:
      ptr-abstract-declarator     C++0x
      noptr-abstract-declaratoropt parameters-and-qualifiers trailing-return-type     C++0x
      ...     C++0x
ptr-abstract-declarator:
      noptr-abstract-declarator     C++0x
      ptr-operator ptr-abstract-declaratoropt     C++0x
noptr-abstract-declarator:
      noptr-abstract-declaratoropt parameters-and-qualifiers     C++0x
      noptr-abstract-declaratoropt [ constant-expression ] attribute-specifier-seqopt     C++0x
      ( ptr-abstract-declarator )     C++0x
"""

from ...cpp_parser import cpp98, cpp11
from be_typing import TYPE_CHECKING


@cpp98
def p_type_id(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        type-id : type-specifier-seq abstract-declarator?
    """


@cpp98
def p_abstract_declarator(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        abstract-declarator : ptr-abstract-declarator
    """


@cpp11
def p_abstract_declarator_ellipsis(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        abstract-declarator : ELLIPSIS
    """
    # TODO: really necessary?


@cpp11
def p_abstract_declarator_trailing_type(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        abstract-declarator : noptr-abstract-declarator? parameters-and-qualifiers trailing-return-type
    """


@cpp98
def p_ptr_abstract_declarator(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        ptr-abstract-declarator : noptr-abstract-declarator
                                | ptr-operator ptr-abstract-declarator?
    """


@cpp98
def p_noptr_abstract_declarator(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        noptr-abstract-declarator : noptr-abstract-declarator? parameters-and-qualifiers
                                  | noptr-abstract-declarator? LBRACKET constant-expression RBRACKET attribute-specifier-seq?
                                  | empty LPAREN ptr-abstract-declarator RPAREN
    """
    # do not remove empty! it helps the parse shift/reduce algorithm


@cpp98
def p_noptr_abstract_declarator(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        noptr-abstract-declarator : noptr-abstract-declarator parameters-and-qualifiers
                                  | noptr-abstract-declarator LBRACKET constant-expression RBRACKET attribute-specifier-seq?
                                  | parameters-and-qualifiers
                                  | LBRACKET constant-expression RBRACKET attribute-specifier-seq?
                                  | LPAREN ptr-abstract-declarator RPAREN
    """
    # do not remove empty! it helps the parse shift/reduce algorithm


@cpp98
def p_noptr_abstract_declarator(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        noptr-abstract-declarator : noptr-abstract-declarator? parameters-and-qualifiers
                                  | noptr-abstract-declarator? LBRACKET RBRACKET attribute-specifier-seq?
                                  | empty LPAREN ptr-abstract-declarator RPAREN
    """
    # do not remove empty! it helps the parse shift/reduce algorithm


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser