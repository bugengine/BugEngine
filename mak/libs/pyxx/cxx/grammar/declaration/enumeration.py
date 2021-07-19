"""
enum-name:
    identifier

enum-specifier:
    enum-head { enumerator-list? }
    enum-head { enumerator-list , }

enum-head:
    enum-key attribute-specifier-seq? enum-head-name? enum-base?
    
enum-head-name:
    nested-name-specifier? identifier

opaque-enum-declaration:
    enum-key attribute-specifier-seq? enum-head-name enum-base? ;

enum-key:
    enum
    enum class
    enum struct

enum-base:
    : type-specifier-seq

enumerator-list:
    enumerator-definition
    enumerator-list , enumerator-definition

enumerator-definition:
    enumerator
    enumerator = constant-expression

enumerator:
    identifier attribute-specifier-seq?
"""

import glrp
from ...parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('enum-name[split] : "identifier"')
@cxx98
def enum_name(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('enum-specifier : enum-head "{" enumerator-list? "}"')
@glrp.rule('enum-specifier : enum-head "{" enumerator-list "," "}"')
@cxx98
def enum_specifier(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('enum-head : enum-key attribute-specifier-seq? enum-head-name? enum-base?')
@cxx98
def enum_head(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('enum-head-name : nested-name-specifier? "identifier"')
@cxx98
def enum_head_name(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('opaque-enum-declaration : enum-key attribute-specifier-seq? enum-head-name enum-base? ";"')
@cxx98
def opaque_enum_declaration(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('enum-key : "enum"')
@glrp.rule('enum-key : "enum" "class"')
@glrp.rule('enum-key : "enum" "struct"')
@cxx98
def enum_key(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('enum-base : ":" type-specifier-seq')
@cxx98
def enum_base(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('enumerator-list : enumerator-definition')
@glrp.rule('enumerator-list : enumerator-list "," enumerator-definition')
@cxx98
def enumerator_list(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('enumerator-definition : enumerator')
@glrp.rule('enumerator-definition : enumerator "=" constant-expression')
@cxx98
def enumerator_definition(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('enumerator : identifier attribute-specifier-seq?')
@cxx98
def enumerator(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...parser import CxxParser