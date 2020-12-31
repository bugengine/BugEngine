from ircc.ir_position import IrPosition
from cpprtti.cpp_position import CppPosition
from typing import Any, List, Optional, Tuple, Union, TYPE_CHECKING
from typing_extensions import Protocol
from .lex import LexToken


class Lexer(Protocol):
    def token(self) -> Optional[LexToken]:
        ...


class YaccSymbol:
    position: IrPosition
    cpp_position: CppPosition


class YaccProduction:
    lexer: 'ir_lexer.IrLexer'
    cpp_lexer: 'cpp_lexer.CppLexer'
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

    def set_position_absolute(self, s1: int, position: IrPosition) -> None:
        ...

    def position(self, s: int) -> IrPosition:
        ...

    def lexspan(self, s: int) -> Tuple[int, int]:
        ...

    def lineno(self, s: int) -> int:
        ...


class Parser:
    ir_parser: 'ir_parser.IrParser'
    cpp_parser: 'cpp_parser.CppParser'

    def errok(self) -> None:
        ...

    def parse(
        self,
        input: Optional[str] = ...,
        lexer: Optional[Lexer] = ...,
        debug: bool = ...,
        tracking: bool = ...,
        tokenfunc: Optional[Any] = ...
    ):
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
) -> Parser:
    ...


if TYPE_CHECKING:
    from ircc import ir_lexer, ir_parser
    from cpprtti import cpp_lexer, cpp_parser