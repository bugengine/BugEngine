"""
simple-type-specifier:
    nested-name-specifier? type-name
    nested-name-specifier template simple-template-id
    decltype-specifier
    placeholder-type-specifier
    nested-name-specifier? template-name
    char
    char8_t
    char16_t
    char32_t
    wchar_t
    bool
    short
    int
    long
    signed
    unsigned
    float
    double
    void

type-name:
    class-name
    enum-name
    typedef-name
"""

import glrp
from .....parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('simple-type-specifier : nested-name-specifier? type-name')
@glrp.rule('simple-type-specifier : nested-name-specifier "template" simple-template-id')
@glrp.rule('simple-type-specifier : decltype-specifier')
@glrp.rule('simple-type-specifier : placeholder-type-specifier')
@glrp.rule('simple-type-specifier : nested-name-specifier? template-name')
@glrp.rule('simple-type-specifier : "char"')
@glrp.rule('simple-type-specifier : "char8_t"')
@glrp.rule('simple-type-specifier : "char16_t"')
@glrp.rule('simple-type-specifier : "char32_t"')
@glrp.rule('simple-type-specifier : "wchar_t"')
@glrp.rule('simple-type-specifier : "bool"')
@glrp.rule('simple-type-specifier : "short"')
@glrp.rule('simple-type-specifier : "int"')
@glrp.rule('simple-type-specifier : "long"')
@glrp.rule('simple-type-specifier : "signed"')
@glrp.rule('simple-type-specifier : "unsigned"')
@glrp.rule('simple-type-specifier : "float"')
@glrp.rule('simple-type-specifier : "double"')
@glrp.rule('simple-type-specifier : "void"')
@cxx98
def simple_type_specifier(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('type-name : class-name')
@glrp.rule('type-name : enum-name')
@glrp.rule('type-name : typedef-name')
@cxx98
def type_name(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from .....parser import CxxParser