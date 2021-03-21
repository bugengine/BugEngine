"""
base-clause:
      : base-specifier-list
base-specifier-list:
      base-specifier ...opt     C++0x
      base-specifier-list , base-specifier ...opt     C++0x
base-specifier:
      attribute-specifier-seqopt base-type-specifier     C++0x
      attribute-specifier-seqopt virtual access-specifieropt base-type-specifier     C++0x
      attribute-specifier-seqopt access-specifier virtualopt base-type-specifier     C++0x
class-or-decltype:
      ::opt nested-name-specifieropt class-name     C++0x
      decltype-specifier     C++0x
base-type-specifier:
      class-or-decltype     C++0x
access-specifier:
      private
      protected
      public
"""

from ...cxx_parser import cxx98, cxx11, disabled
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("base-clause : ':' base-specifier-list")
@cxx98
def p_base_clause(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("base-specifier-list : base-specifier")
@glrp.rule("base-specifier-list : base-specifier-list ',' base-specifier")
@cxx98
def p_base_specifier_list(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("base-specifier-list : base-specifier '...'")
@glrp.rule("base-specifier-list : base-specifier-list ',' base-specifier '...'")
@cxx11
def p_base_specifier_list_ellipsis(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("base-specifier : attribute-specifier-seq? base-type-specifier")
@glrp.rule("base-specifier : attribute-specifier-seq? 'virtual' access-specifier? base-type-specifier")
@glrp.rule("base-specifier : attribute-specifier-seq? access-specifier 'virtual'? base-type-specifier")
@cxx98
def p_base_specifier(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("class-or-decltype : id-type")
@cxx98
def p_class_or_decltype(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("class-or-decltype : decltype-specifier")
@cxx11
def p_class_or_decltype_cxx11(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("base-type-specifier : class-or-decltype")
@cxx98
def p_base_type_specifier(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    """
        
    """


@glrp.rule("access-specifier : 'private' | 'protected' | 'public'")
@cxx98
def p_access_specifier(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser