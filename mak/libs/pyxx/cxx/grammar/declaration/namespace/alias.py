"""
namespace-alias:
    identifier

namespace-alias-definition:
    namespace identifier = qualified-namespace-specifier ;

qualified-namespace-specifier:
    nested-name-specifier? namespace-name
"""

import glrp
from ....parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('namespace-alias : "identifier"')
@cxx98
def namespace_alias(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('namespace-alias-definition : "namespace" "identifier" "=" qualified-namespace-specifier ";"')
@cxx98
def namespace_alias_definition(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('qualified-namespace-specifier : nested-name-specifier? namespace-name')
@cxx98
def qualified_namespace_specifier(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ....parser import CxxParser