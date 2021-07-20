"""
class-name:
    identifier
    simple-template-id

class-specifier:
    class-head { member-specification? }

class-head:
    class-key attribute-specifier-seq? class-head-name class-virt-specifier? base-clause?
    class-key attribute-specifier-seq? base-clause?

class-head-name:
    nested-name-specifier? class-name

class-virt-specifier:
    final

class-key:
    class
    struct
    union
"""

import glrp
from ...parser import cxx98
from be_typing import TYPE_CHECKING
from . import member
from . import initializer
from . import derived
from . import conversion


@glrp.rule('class-name[split] : [split]"identifier"')
@glrp.rule('class-name[split] : [split]simple-template-id')
@cxx98
def class_name(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('class-specifier : class-head "{" member-specification? "}"')
@cxx98
def class_specifier(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('class-head : class-key attribute-specifier-seq? class-head-name class-virt-specifier? base-clause?')
@glrp.rule('class-head : class-key attribute-specifier-seq? base-clause?')
@cxx98
def class_head(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('class-head-name : nested-name-specifier? class-name')
@cxx98
def class_head_name(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('class-virt-specifier : "final"')
@cxx98
def class_virt_specifier(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('class-key : "class"')
@glrp.rule('class-key : "struct"')
@glrp.rule('class-key : "union"')
@cxx98
def class_key(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...parser import CxxParser