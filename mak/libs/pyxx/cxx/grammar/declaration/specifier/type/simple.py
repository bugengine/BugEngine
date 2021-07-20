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
@glrp.rule('simple-type-specifier[split] : nested-name-specifier? template-name')
@glrp.rule('simple-type-specifier : [split]"char"')
@glrp.rule('simple-type-specifier : [split]"char8_t"')
@glrp.rule('simple-type-specifier : [split]"char16_t"')
@glrp.rule('simple-type-specifier : [split]"char32_t"')
@glrp.rule('simple-type-specifier : [split]"wchar_t"')
@glrp.rule('simple-type-specifier : [split]"bool"')
@glrp.rule('simple-type-specifier : [split]"short"')
@glrp.rule('simple-type-specifier : [split]"int"')
@glrp.rule('simple-type-specifier : [split]"long"')
@glrp.rule('simple-type-specifier : [split]"signed"')
@glrp.rule('simple-type-specifier : [split]"unsigned"')
@glrp.rule('simple-type-specifier : [split]"float"')
@glrp.rule('simple-type-specifier : [split]"double"')
@glrp.rule('simple-type-specifier : [split]"void"')
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