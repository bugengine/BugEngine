"""
module-declaration:
    export-keyword? module-keyword module-name module-partition? attribute-specifier-seq? ;

module-name:
    module-name-qualifier? identifier

module-partition:
    : module-name-qualifier? identifier

module-name-qualifier:
    identifier .
    module-name-qualifier identifier .
"""

import glrp
from ...parser import cxx98
from be_typing import TYPE_CHECKING
from . import module_export
from . import module_import
from . import global_fragment
from . import private_fragment


@glrp.rule('module-declaration : "export"? "module" module-name module-partition? attribute-specifier-seq? ";"')
@cxx98
def module_declaration(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('module-name : module-name-qualifier? "identifier"')
@cxx98
def module_name(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('module-partition : ":" module-name-qualifier? "identifier"')
@cxx98
def module_partition(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('module-name-qualifier : "identifier" "."')
@glrp.rule('module-name-qualifier : module-name-qualifier "identifier" "."')
@cxx98
def module_name_qualifier(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...parser import CxxParser