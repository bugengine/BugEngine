from typing import Any, Callable, Optional, Union, TYPE_CHECKING


class PlyLogger:
    ...


class LexToken:
    type: str
    value: str
    parsed_value: Union[None, int, str]
    filename: str
    lineno: int
    lexpos: int
    endlexpos: int
    position: 'IrPosition'
    lexer: 'Lexer'
    ir_lexer: 'ir_lexer.IrLexer'


class Lexer:
    lexdata: str
    lineno: int
    ir_lexer: 'ir_lexer.IrLexer'

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
    from ircc import ir_ast, ir_lexer
    from ircc.ir_position import IrPosition
