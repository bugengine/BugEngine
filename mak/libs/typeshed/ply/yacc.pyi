from clt.cl_ast.position import Position
from typing import Any, List, Optional, Tuple, Union, TYPE_CHECKING
from .lex import LexToken


class YaccSymbol:
    position: Position


class YaccProduction:
    lexer: 'cl_lexer.ClLexer'
    parser: 'Parser'
    slice: List[Union[YaccSymbol, LexToken]]
    stack: List[Union[YaccSymbol, LexToken]]

    def __len__(self) -> int:
        ...

    def __getitem__(self, n: int) -> Any:
        ...

    def __setitem__(self, n: int, value: Any) -> None:
        ...

    def set_position(self, s1: int, s2: int) -> None:
        ...

    def set_position_absolute(self, s1: int, position: Position) -> None:
        ...

    def position(self, s: int) -> Position:
        ...

    def lexspan(self, s: int) -> Tuple[int, int]:
        ...

    def lineno(self, s: int) -> int:
        ...


class Parser:
    def errok(self) -> None:
        ...


def yacc(
    method: str = ...,
    debug: bool = ...,
    module: Optional[Any] = ...,
    tabmodule: str = ...,
    start: Optional[str] = ...,
    check_recursion: bool = ...,
    optimize: bool = ...,
    write_tables: bool = ...,
    debugfile: str = ...,
    outputdir: Optional[str] = ...,
    debuglog: Optional[str] = ...,
    errorlog: Optional[str] = ...,
    picklefile: Optional[str] = ...
):
    ...


if TYPE_CHECKING:
    from clt import cl_lexer