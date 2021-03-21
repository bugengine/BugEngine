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

from ...cxx_parser import cxx98, cxx11
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("class-specifier : class-head '{' member-specification? '}'")
@cxx98
def p_class_specifier(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("class-head : class-key attribute-specifier-seq? id-type class-virt-specifier-seq base-clause?")
@glrp.rule("class-head : class-key attribute-specifier-seq? base-clause?")
@cxx98
def p_class_head(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("class-virt-specifier-seq : ")
@cxx98
def p_class_virt_specifier_seq_empty(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("class-virt-specifier-seq : class-virt-specifier")
@glrp.rule("class-virt-specifier-seq : class-virt-specifier-seq class-virt-specifier")
@cxx11
def p_class_virt_specifier_seq(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("class-virt-specifier : 'final' | 'explicit'")
@cxx11
def p_class_virt_specifier(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("class-key : 'class' | 'struct' | 'union'")
@cxx98
def p_class_key(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser