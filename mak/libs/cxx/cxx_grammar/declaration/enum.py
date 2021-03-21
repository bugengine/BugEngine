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

from ...cxx_parser import cxx98, cxx11, disabled
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("enum-specifier : enum-head '{' enumerator-list? '}'")
@glrp.rule("enum-specifier : enum-head '{' enumerator-list ',' '}'")
@cxx98
def p_enum_specifier(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("enum-head : enum-key attribute-specifier-seq? id-type? enum-base")
@cxx98
def p_enum_head(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("opaque-enum-declaration : enum-key attribute-specifier-seq? 'identifier' enum-base ';'")
@cxx11
def p_opaque_enum_declaration(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("enum-key : 'enum'")
@cxx98
def p_enum_key(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("enum-key : 'enum' 'class' | 'enum' 'struct'")
@cxx11
def p_enum_key_cxx11(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("enum-base :")
@cxx98
def p_enum_base_empty(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("enum-base : ':' type-specifier-seq")
@cxx11
def p_enum_base(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("enumerator-list : enumerator-definition | enumerator-list ',' enumerator-definition")
@cxx98
def p_enumerator_list(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("enumerator-definition : enumerator | enumerator '=' constant-expression")
@cxx98
def p_enumerator_definition(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("enumerator : 'identifier'")
@cxx98
def p_enumerator(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser