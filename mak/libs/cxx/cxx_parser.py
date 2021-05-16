import os
import glrp
from . import cxx_lexer, cxx_messages


class CxxParser(glrp.Parser):
    Lexer = cxx_lexer.CxxLexer

    def __init__(self, tmp_dir):
        # type: (str)->None
        self.lexer = self.__class__.Lexer()
        glrp.Parser.__init__(
            self, self.lexer, 'simple-declaration', os.path.join(tmp_dir, self.__class__.__name__ + '.tab')
        )
        #    self.tokens = self.__class__.Lexer.tokens
        #    self.parser = yacc.yacc(
        #        module=self,
        #        start='simple-declaration',
        #        picklefile=os.path.join(tmp_dir, 'cxxrtti_grammar_%s.pickle' % self.__class__.__name__[-2:]),
        #        debugfile=os.path.join(tmp_dir, 'cxxrtti_grammar_%s.debug' % self.__class__.__name__[-2:]),
        #        debug=True
        #    )


class CxxParser98(CxxParser):
    Lexer = cxx_lexer.CxxLexer98


class CxxParser03(CxxParser98):
    Lexer = cxx_lexer.CxxLexer03


class CxxParser11(CxxParser03):
    Lexer = cxx_lexer.CxxLexer11


class CxxParser14(CxxParser11):
    Lexer = cxx_lexer.CxxLexer14


class CxxParser17(CxxParser14):
    Lexer = cxx_lexer.CxxLexer17


class CxxParser20(CxxParser17):
    Lexer = cxx_lexer.CxxLexer20


class CxxParser23(CxxParser20):
    Lexer = cxx_lexer.CxxLexer23


def _empty_rule(parser, production):
    # type: (CxxParser, glrp.Production) -> None
    pass


def cxx98(func):
    # type: (Callable[[CxxParser, glrp.Production], None]) -> Callable[[CxxParser, glrp.Production], None]
    setattr(CxxParser98, func.__name__, func)
    return func


def cxx03(func):
    # type: (Callable[[CxxParser, glrp.Production], None]) -> Callable[[CxxParser, glrp.Production], None]
    setattr(CxxParser03, func.__name__, func)
    return func


def cxx11(func):
    # type: (Callable[[CxxParser, glrp.Production], None]) -> Callable[[CxxParser, glrp.Production], None]
    setattr(CxxParser11, func.__name__, func)
    return func


def cxx14(func):
    # type: (Callable[[CxxParser, glrp.Production], None]) -> Callable[[CxxParser, glrp.Production], None]
    setattr(CxxParser14, func.__name__, func)
    return func


def cxx17(func):
    # type: (Callable[[CxxParser, glrp.Production], None]) -> Callable[[CxxParser, glrp.Production], None]
    setattr(CxxParser17, func.__name__, func)
    return func


def cxx20(func):
    # type: (Callable[[CxxParser, glrp.Production], None]) -> Callable[[CxxParser, glrp.Production], None]
    setattr(CxxParser20, func.__name__, func)
    return func


def cxx23(func):
    # type: (Callable[[CxxParser, glrp.Production], None]) -> Callable[[CxxParser, glrp.Production], None]
    setattr(CxxParser23, func.__name__, func)
    return func


def disabled(func):
    # type: (Callable[[CxxParser, glrp.Production], None]) -> Callable[[CxxParser, glrp.Production], None]
    setattr(CxxParser98, func.__name__, _empty_rule)
    return func


def deprecate_cxx03(func):
    # type: (Callable[[CxxParser, glrp.Production], None]) -> Callable[[CxxParser, glrp.Production], None]
    setattr(CxxParser03, func.__name__, _empty_rule)
    return func


def deprecate_cxx11(func):
    # type: (Callable[[CxxParser, glrp.Production], None]) -> Callable[[CxxParser, glrp.Production], None]
    setattr(CxxParser11, func.__name__, _empty_rule)
    return func


def deprecate_cxx14(func):
    # type: (Callable[[CxxParser, glrp.Production], None]) -> Callable[[CxxParser, glrp.Production], None]
    setattr(CxxParser14, func.__name__, _empty_rule)
    return func


def deprecate_cxx17(func):
    # type: (Callable[[CxxParser, glrp.Production], None]) -> Callable[[CxxParser, glrp.Production], None]
    setattr(CxxParser17, func.__name__, _empty_rule)
    return func


def deprecate_cxx20(func):
    # type: (Callable[[CxxParser, glrp.Production], None]) -> Callable[[CxxParser, glrp.Production], None]
    setattr(CxxParser20, func.__name__, _empty_rule)
    return func


def deprecate_cxx23(func):
    # type: (Callable[[CxxParser, glrp.Production], None]) -> Callable[[CxxParser, glrp.Production], None]
    setattr(CxxParser23, func.__name__, _empty_rule)
    return func


#@cxx98
#def p_error(self, p):
#    # type: (CxxParser, glrp.Production) -> None
#    """
#        error :
#    """
#    self.lexer.logger.C0011(p.position, p.value, p.type)

_parsers = {
    'c++98': CxxParser98,
    'c++03': CxxParser03,
    'c++11': CxxParser11,
    'c++14': CxxParser14,
    'c++17': CxxParser17,
    'c++20': CxxParser20,
    'c++23': CxxParser23,
}


def parser(std):
    # type: (str) -> Type[CxxParser]
    return _parsers[std]


from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from typing import Callable, Type
    from types import ModuleType
    from .cxx_messages import Logger