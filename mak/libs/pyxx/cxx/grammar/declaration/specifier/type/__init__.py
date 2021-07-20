"""
type-specifier:
    simple-type-specifier
    elaborated-type-specifier
    typename-specifier
    cv-qualifier

type-specifier-seq:
    type-specifier attribute-specifier-seq?
    type-specifier type-specifier-seq

defining-type-specifier:
    type-specifier
    class-specifier
    enum-specifier

defining-type-specifier-seq:
    defining-type-specifier attribute-specifier-seq?
    defining-type-specifier defining-type-specifier-seq
"""

import glrp
from .....parser import cxx98
from be_typing import TYPE_CHECKING
from . import simple
from . import elaborated
from . import decltype
from . import placeholder


@glrp.rule('type-specifier[split] : simple-type-specifier')
@glrp.rule('type-specifier : elaborated-type-specifier')
@glrp.rule('type-specifier[split] : typename-specifier')
@glrp.rule('type-specifier : cv-qualifier')
@cxx98
def type_specifier(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


#@glrp.rule('type-specifier-seq : type-specifier attribute-specifier-seq?')
@glrp.rule('type-specifier-seq[split] : type-specifier')
@glrp.rule('type-specifier-seq : type-specifier attribute-specifier-seq')
@glrp.rule('type-specifier-seq : type-specifier type-specifier-seq')
@cxx98
def type_specifier_seq(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('defining-type-specifier[split] : type-specifier')
@glrp.rule('defining-type-specifier : class-specifier')
@glrp.rule('defining-type-specifier : enum-specifier')
@cxx98
def defining_type_specifier(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


#@glrp.rule('defining-type-specifier-seq : defining-type-specifier attribute-specifier-seq?')
@glrp.rule('defining-type-specifier-seq[split] : defining-type-specifier')
@glrp.rule('defining-type-specifier-seq : defining-type-specifier attribute-specifier-seq')
@glrp.rule('defining-type-specifier-seq : defining-type-specifier defining-type-specifier-seq')
@cxx98
def defining_type_specifier_seq(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from .....parser import CxxParser