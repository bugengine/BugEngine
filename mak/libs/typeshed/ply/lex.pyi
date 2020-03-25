from typing import Any, Callable, Optional, Union, TYPE_CHECKING


class PlyLogger:
    ...


class LexToken:
    type: str
    value: str
    filename: str
    lineno: int
    lexpos: int
    endlexpos: int
    position: 'ClPosition'
    lexer: 'Lexer'
    cl_lexer: 'cl_lexer.ClLexer'
    found_object: Optional['cl_ast.cppobject.CppObject']


class Lexer:
    lexdata: str
    lineno: int
    cl_lexer: 'cl_lexer.ClLexer'

    def token(self) -> Optional[LexToken]:
        ...

    def input(self, s: str) -> None:
        ...

    def begin(self, state: str) -> None:
        ...

    def skip(self, skip_count: int) -> None:
        ...


def lex(
    module: Any = ...,
    object: Any = ...,
    debug: bool = ...,
    optimize: bool = ...,
    lextab: str = ...,
    reflags: int = ...,
    nowarn: bool = ...,
    outputdir: Optional[str] = ...,
    debuglog: Optional[PlyLogger] = ...,
    errorlog: Optional[PlyLogger] = ...
) -> Lexer:
    ...


_LexRule = Callable[..., Optional[LexToken]]


def TOKEN(r: str) -> Callable[[_LexRule], _LexRule]:
    ...


if TYPE_CHECKING:
    from clt import cl_ast, cl_lexer
    from clt.cl_position import ClPosition
