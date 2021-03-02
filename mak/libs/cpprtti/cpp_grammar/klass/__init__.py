"""
class-name:
      identifier
      simple-template-id     C++0x
class-specifier:
      class-head { member-specificationopt }
class-head:
      class-key attribute-specifier-seqopt class-head-name class-virt-specifier-seqopt base-clauseopt     C++0x
      class-key attribute-specifier-seqopt base-clauseopt     C++0x
class-head-name:
      nested-name-specifieropt class-name     C++0x
class-virt-specifier-seq:
      class-virt-specifier     C++0x
      class-virt-specifier-seq class-virt-specifier     C++0x
class-virt-specifier:
      final     C++0x
      explicit     C++0x
class-key:
      class
      struct
      union
"""

from . import member
from . import base
from . import conversion
from . import base_init

from ...cpp_parser import cpp98, cpp11, disabled
from be_typing import TYPE_CHECKING


@disabled
def p_class_name(p):
    """
        class-name : identifier
                   | simple-template-id
    """


@cpp98
def p_class_specifier(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        class-specifier : class-head member-specification? RBRACE
    """


@cpp98
def p_class_head(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        class-head : class-key attribute-specifier-seq? id-expression class-virt-specifier-seq base-clause? LBRACE
                   | class-key attribute-specifier-seq? base-clause? LBRACE
    """
    pass


@disabled
def p_class_head_name(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        class-head-name : nested-name-specifier? IDENTIFIER
                        | nested-name-specifier? simple-template-id
    """


@cpp98
def p_class_virt_specifier_seq_empty(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        class-virt-specifier-seq : empty
    """


@cpp11
def p_class_virt_specifier_seq(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        class-virt-specifier-seq : class-virt-specifier
                                 | class-virt-specifier-seq class-virt-specifier
    """


@cpp11
def p_class_virt_specifier(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        class-virt-specifier : KW_FINAL
                             | KW_EXPLICIT
    """


@cpp98
def p_class_key(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        class-key : KW_CLASS
                  | KW_STRUCT
                  | KW_UNION
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser