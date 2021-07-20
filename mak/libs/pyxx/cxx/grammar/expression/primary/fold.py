"""
fold-expression:
    ( cast-expression fold-operator ... )
    ( ... fold-operator cast-expression )
    ( cast-expression fold-operator ... fold-operator cast-expression )

fold-operator: one of
+   -   *   /   %   ^   &   |   <<  >>
+=  -=  *=  /=  %=  ^=  &=  |=  <<= >>= =
==  !=  <   >   <=  >=  &&  ||  ,   .*  ->*
"""

import glrp
from ....parser import cxx98
from be_typing import TYPE_CHECKING


@glrp.rule('fold-expression : [split]"(" cast-expression fold-operator "..." ")"')
@glrp.rule('fold-expression : [split]"(" "..." fold-operator cast-expression ")"')
@glrp.rule('fold-expression : [split]"(" cast-expression fold-operator "..." fold-operator cast-expression ")"')
@cxx98
def fold_expression(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('fold-operator : [split]"+"')
@glrp.rule('fold-operator : [split]"-"')
@glrp.rule('fold-operator : [split]"*"')
@glrp.rule('fold-operator : [split]"/"')
@glrp.rule('fold-operator : [split]"%"')
@glrp.rule('fold-operator : [split]"^"')
@glrp.rule('fold-operator : [split]"&"')
@glrp.rule('fold-operator : [split]"|"')
@glrp.rule('fold-operator : [split]"<<"')
@glrp.rule('fold-operator : [split]">>"')
@glrp.rule('fold-operator : [split]"+="')
@glrp.rule('fold-operator : [split]"-="')
@glrp.rule('fold-operator : [split]"*="')
@glrp.rule('fold-operator : [split]"/="')
@glrp.rule('fold-operator : [split]"%="')
@glrp.rule('fold-operator : [split]"^="')
@glrp.rule('fold-operator : [split]"&="')
@glrp.rule('fold-operator : [split]"|="')
@glrp.rule('fold-operator : [split]"<<="')
@glrp.rule('fold-operator : [split]">>="')
@glrp.rule('fold-operator : [split]"="')
@glrp.rule('fold-operator : [split]"=="')
@glrp.rule('fold-operator : [split]"!="')
@glrp.rule('fold-operator : [split]"<"')
@glrp.rule('fold-operator : [split]">"')
@glrp.rule('fold-operator : [split]"<="')
@glrp.rule('fold-operator : [split]">="')
@glrp.rule('fold-operator : [split]"&&"')
@glrp.rule('fold-operator : [split]"||"')
@glrp.rule('fold-operator : [split]","')
@glrp.rule('fold-operator : [split]".*"')
@glrp.rule('fold-operator : [split]"->*"')
@cxx98
def fold_operator(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ....parser import CxxParser
