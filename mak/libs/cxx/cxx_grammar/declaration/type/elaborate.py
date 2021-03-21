"""
elaborated-type-specifier:
      class-key attribute-specifier-seqopt ::opt nested-name-specifieropt identifier
      class-key ::opt nested-name-specifieropt templateopt simple-template-id
      enum ::opt nested-name-specifieropt identifier
"""

from ....cxx_parser import cxx98
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("elaborated-type-specifier : class-key attribute-specifier-seq? id-type")
@glrp.rule("elaborated-type-specifier : enum-key id-type")
@cxx98
def p_elaborate_type_specifier(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ....cxx_parser import CxxParser