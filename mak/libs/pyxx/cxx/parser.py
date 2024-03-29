import glrp
from . import lexer
from .. import tables
import os
from be_typing import Callable, TYPE_CHECKING, cast


class CxxParser(glrp.Parser):
    Lexer = glrp.Lexer

    def __init__(self, tmp_dir, mode=glrp.LOAD_CACHE):
        # type: (str, int)->None
        self.lexer = self.__class__.Lexer()
        out_dir = os.path.dirname(tables.__file__)
        glrp.Parser.__init__(self, self.lexer, 'translation-unit', tmp_dir, out_dir, mode)


class Cxx98Parser(CxxParser):
    Lexer = lexer.Cxx98Lexer


class Cxx03Parser(Cxx98Parser):
    Lexer = lexer.Cxx03Lexer


class Cxx11Parser(Cxx03Parser):
    Lexer = lexer.Cxx11Lexer


class Cxx14Parser(Cxx11Parser):
    Lexer = lexer.Cxx14Lexer


class Cxx17Parser(Cxx14Parser):
    Lexer = lexer.Cxx17Lexer


class Cxx20Parser(Cxx17Parser):
    Lexer = lexer.Cxx20Lexer


class Cxx23Parser(Cxx20Parser):
    Lexer = lexer.Cxx23Lexer


def _empty_rule(parser, production):
    # type: (glrp.Parser, glrp.Production) -> None
    pass


def cxx98(func):
    # type: (Callable[[CxxParser, glrp.Production], None]) -> Callable[[glrp.Parser, glrp.Production], None]
    setattr(Cxx98Parser, func.__name__, func)
    return cast(Callable[[glrp.Parser, glrp.Production], None], func)


def cxx03(func):
    # type: (Callable[[CxxParser, glrp.Production], None]) -> Callable[[glrp.Parser, glrp.Production], None]
    setattr(Cxx03Parser, func.__name__, func)
    return cast(Callable[[glrp.Parser, glrp.Production], None], func)


def cxx11(func):
    # type: (Callable[[CxxParser, glrp.Production], None]) -> Callable[[glrp.Parser, glrp.Production], None]
    setattr(Cxx11Parser, func.__name__, func)
    return cast(Callable[[glrp.Parser, glrp.Production], None], func)


def cxx14(func):
    # type: (Callable[[CxxParser, glrp.Production], None]) -> Callable[[glrp.Parser, glrp.Production], None]
    setattr(Cxx14Parser, func.__name__, func)
    return cast(Callable[[glrp.Parser, glrp.Production], None], func)


def cxx17(func):
    # type: (Callable[[CxxParser, glrp.Production], None]) -> Callable[[glrp.Parser, glrp.Production], None]
    setattr(Cxx17Parser, func.__name__, func)
    return cast(Callable[[glrp.Parser, glrp.Production], None], func)


def cxx20(func):
    # type: (Callable[[CxxParser, glrp.Production], None]) -> Callable[[glrp.Parser, glrp.Production], None]
    setattr(Cxx20Parser, func.__name__, func)
    return cast(Callable[[glrp.Parser, glrp.Production], None], func)


def cxx23(func):
    # type: (Callable[[CxxParser, glrp.Production], None]) -> Callable[[glrp.Parser, glrp.Production], None]
    setattr(Cxx23Parser, func.__name__, func)
    return cast(Callable[[glrp.Parser, glrp.Production], None], func)


from . import grammar
