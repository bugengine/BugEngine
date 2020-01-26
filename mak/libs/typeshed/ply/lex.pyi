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
    position: 'Position'
    lexer: 'cl_lexer.ClLexer'
    found_object: Optional['cl_ast.cppobject.CppObject']


class Lexer:
    lexdata: str
    lineno: int

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


_LexRule = Callable[['cl_lexer.ClLexer', LexToken], Optional[LexToken]]


def TOKEN(r: str) -> Callable[[_LexRule], _LexRule]:
    ...


if TYPE_CHECKING:
    from clt import cl_ast, cl_lexer
    from clt.cl_ast.position import Position
