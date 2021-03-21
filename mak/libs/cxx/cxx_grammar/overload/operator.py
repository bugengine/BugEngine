"""
operator-function-id: 	See C++ Standard Core Language Issue n. 189
      operator overloadable-operator
      operator overloadable-operator < template-argument-listopt >
overloadable-operator: 	See C++ Standard Core Language Issue n. 189
      new
      delete
      new [ ]
      delete [ ]
      +
      -
      *
      /
      %
      ^
      &
      |
      ~
      !
      =
      <
      >
      +=
      -=
      *=
      /=
      %=
      ^=
      &=
      |=
      <<
      >>
      >>=
      <<=
      ==
      !=
      <=
      >=
      &&
      ||
      ++
      --
      ,
      ->*
      ->
      ()
      []
"""

from ...cxx_parser import cxx98
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("operator-function-id : 'operator' overloadable-operator")
@cxx98
def p_operator_function_id(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass
    # removed template args. Looks duplicated


@cxx98
@glrp.rule("overloadable-operator : 'new'")
@glrp.rule("overloadable-operator : 'delete'")
@glrp.rule("overloadable-operator : 'new' '[' ']'")
@glrp.rule("overloadable-operator : 'delete' '[' ']'")
@glrp.rule("overloadable-operator : '+'")
@glrp.rule("overloadable-operator : '-'")
@glrp.rule("overloadable-operator : '*'")
@glrp.rule("overloadable-operator : '/'")
@glrp.rule("overloadable-operator : '%'")
@glrp.rule("overloadable-operator : '^'")
@glrp.rule("overloadable-operator : '&'")
@glrp.rule("overloadable-operator : '|'")
@glrp.rule("overloadable-operator : '~'")
@glrp.rule("overloadable-operator : '!'")
@glrp.rule("overloadable-operator : '='")
@glrp.rule("overloadable-operator : '<'")
@glrp.rule("overloadable-operator : '>'")
@glrp.rule("overloadable-operator : '+='")
@glrp.rule("overloadable-operator : '-='")
@glrp.rule("overloadable-operator : '*='")
@glrp.rule("overloadable-operator : '/='")
@glrp.rule("overloadable-operator : '%='")
@glrp.rule("overloadable-operator : '^='")
@glrp.rule("overloadable-operator : '&='")
@glrp.rule("overloadable-operator : '|='")
@glrp.rule("overloadable-operator : '<<'")
@glrp.rule("overloadable-operator : '>>'")
@glrp.rule("overloadable-operator : '<<='")
@glrp.rule("overloadable-operator : '>>='")
@glrp.rule("overloadable-operator : '=='")
@glrp.rule("overloadable-operator : '!='")
@glrp.rule("overloadable-operator : '<='")
@glrp.rule("overloadable-operator : '>='")
@glrp.rule("overloadable-operator : '&&'")
@glrp.rule("overloadable-operator : '||'")
@glrp.rule("overloadable-operator : '++'")
@glrp.rule("overloadable-operator : '--'")
@glrp.rule("overloadable-operator : ','")
@glrp.rule("overloadable-operator : '->*'")
@glrp.rule("overloadable-operator : '->'")
@glrp.rule("overloadable-operator : '(' ')'")
@glrp.rule("overloadable-operator : '[' ']'")
def p_overloadable_operator(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser