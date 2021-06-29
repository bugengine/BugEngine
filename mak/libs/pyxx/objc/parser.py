import glrp
from ..c import C89Parser, C99Parser, C11Parser, C17Parser
from ..cxx import Cxx98Parser, Cxx03Parser, Cxx11Parser, Cxx14Parser, Cxx17Parser, Cxx20Parser, Cxx23Parser
from .c_lexer import ObjC89Lexer, ObjC99Lexer, ObjC11Lexer, ObjC17Lexer
from .cxx_lexer import ObjCxx98Lexer, ObjCxx03Lexer, ObjCxx11Lexer, ObjCxx14Lexer, ObjCxx17Lexer, ObjCxx20Lexer, ObjCxx23Lexer


class ObjCParser:
    pass


def objc(func):
    # type: (Callable[[ObjCParser, glrp.Production], None]) -> Callable[[ObjCParser, glrp.Production], None]
    setattr(ObjCParser, func.__name__, func)
    return func


class ObjC89Parser(C89Parser, ObjCParser):
    Lexer = ObjC89Lexer


class ObjC99Parser(C99Parser, ObjCParser):
    Lexer = ObjC99Lexer


class ObjC11Parser(C11Parser, ObjCParser):
    Lexer = ObjC11Lexer


class ObjC17Parser(C17Parser, ObjCParser):
    Lexer = ObjC17Lexer


class ObjCxx98Parser(Cxx98Parser):
    Lexer = ObjCxx98Lexer


class ObjCxx03Parser(Cxx03Parser):
    Lexer = ObjCxx03Lexer


class ObjCxx11Parser(Cxx11Parser):
    Lexer = ObjCxx11Lexer


class ObjCxx14Parser(Cxx14Parser):
    Lexer = ObjCxx14Lexer


class ObjCxx17Parser(Cxx17Parser):
    Lexer = ObjCxx17Lexer


class ObjCxx20Parser(Cxx20Parser):
    Lexer = ObjCxx20Lexer


class ObjCxx23Parser(Cxx23Parser):
    Lexer = ObjCxx23Lexer


from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from typing import Callable
