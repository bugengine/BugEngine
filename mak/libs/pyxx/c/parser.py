import glrp
from . import lexer
from be_typing import Callable, TYPE_CHECKING, cast


class CParser(glrp.Parser):
    Lexer = glrp.Lexer

    def __init__(self, tmp_dir):
        # type: (str)->None
        self.lexer = self.__class__.Lexer()
        glrp.Parser.__init__(self, self.lexer, 'translation-unit', tmp_dir)
        #    self.tokens = self.__class__.Lexer.tokens
        #    self.parser = yacc.yacc(
        #        module=self,
        #        start='simple-declaration',
        #        picklefile=os.path.join(tmp_dir, 'cxxrtti_grammar_%s.pickle' % self.__class__.__name__[-2:]),
        #        debugfile=os.path.join(tmp_dir, 'cxxrtti_grammar_%s.debug' % self.__class__.__name__[-2:]),
        #        debug=True
        #    )


class C89Parser(CParser):
    Lexer = lexer.C89Lexer


class C99Parser(C89Parser):
    Lexer = lexer.C99Lexer


class C11Parser(C99Parser):
    Lexer = lexer.C11Lexer


class C17Parser(C11Parser):
    Lexer = lexer.C17Lexer


def c89(func):
    # type: (Callable[[CParser, glrp.Production], None]) -> Callable[[glrp.Parser, glrp.Production], None]
    setattr(C89Parser, func.__name__, func)
    return cast(Callable[[glrp.Parser, glrp.Production], None], func)


def c99(func):
    # type: (Callable[[CParser, glrp.Production], None]) -> Callable[[glrp.Parser, glrp.Production], None]
    setattr(C99Parser, func.__name__, func)
    return cast(Callable[[glrp.Parser, glrp.Production], None], func)


def c11(func):
    # type: (Callable[[CParser, glrp.Production], None]) -> Callable[[glrp.Parser, glrp.Production], None]
    setattr(C11Parser, func.__name__, func)
    return cast(Callable[[glrp.Parser, glrp.Production], None], func)


def c17(func):
    # type: (Callable[[CParser, glrp.Production], None]) -> Callable[[glrp.Parser, glrp.Production], None]
    setattr(C17Parser, func.__name__, func)
    return cast(Callable[[glrp.Parser, glrp.Production], None], func)


from . import grammar

from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from typing import Callable
