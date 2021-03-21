import os
from ply import yacc
from . import cpp_lexer
from .cpp_position import CppPosition


def _set_position(p, s1, s2):
    # type: (yacc.YaccProduction, int, int) -> None
    p.slice[s1].cpp_position = _position(p, s2)


def _set_position_absolute(p, s1, position):
    # type: (yacc.YaccProduction, int, CppPosition) -> None
    p.slice[s1].cpp_position = position


def _position(p, s):
    # type: (yacc.YaccProduction, int) -> CppPosition
    if s >= 0:
        return p.slice[s].cpp_position
    else:
        return p.stack[s].cpp_position


setattr(yacc.YaccProduction, 'set_position', _set_position)
setattr(yacc.YaccProduction, 'set_position_absolute', _set_position_absolute)
setattr(yacc.YaccProduction, 'position', _position)


class CppParser(object):
    Lexer = cpp_lexer.CppLexer

    precedence = (
        ('nonassoc', 'EMPTY'),
        ('left', 'SCOPE_REDUCTION'),
        ('left', 'ARRAY_BRACKET'),
        ('left', 'OPERATOR_BRACKET'),
        ('left', 'IDENTIFIER'),
        ('left', 'OP_SCOPE'),
        ('left', 'DECL_SPEC_GREEDY'),
    )

    def __init__(self, tmp_dir):
        # type: (str)->None
        self.tokens = self.__class__.Lexer.tokens
        self.parser = yacc.yacc(
            module=self,
            start='simple-declaration',
            picklefile=os.path.join(tmp_dir, 'cpprtti_grammar_%s.pickle' % self.__class__.__name__[-2:]),
            debugfile=os.path.join(tmp_dir, 'cpprtti_grammar_%s.debug' % self.__class__.__name__[-2:]),
            debug=True
        )

    def parse(self, logger, filename):
        # type: (Logger, str) -> CppObject
        with open(filename, 'r') as input:
            self.lexer = self.__class__.Lexer(filename, logger)
            result = self.parser.parse(input.read(), lexer=self.lexer)
        if result:
            # result.debug_dump()
            result.error_count = logger._error_count
            pass
        return result


class CppParser98(CppParser):
    Lexer = cpp_lexer.CppLexer98


class CppParser03(CppParser98):
    Lexer = cpp_lexer.CppLexer03


class CppParser11(CppParser03):
    Lexer = cpp_lexer.CppLexer11


class CppParser14(CppParser11):
    Lexer = cpp_lexer.CppLexer14


class CppParser17(CppParser14):
    Lexer = cpp_lexer.CppLexer17


class CppParser20(CppParser17):
    Lexer = cpp_lexer.CppLexer20


class CppParser23(CppParser20):
    Lexer = cpp_lexer.CppLexer23


def _empty_rule(self, p):
    # type: (CppParser, yacc.YaccProduction) -> None
    """ """
    pass


def cpp98(func):
    setattr(CppParser98, func.__name__, func)
    return func


def cpp03(func):
    setattr(CppParser03, func.__name__, func)
    return func


def cpp11(func):
    setattr(CppParser11, func.__name__, func)
    return func


def cpp14(func):
    setattr(CppParser14, func.__name__, func)
    return func


def cpp17(func):
    setattr(CppParser17, func.__name__, func)
    return func


def cpp20(func):
    setattr(CppParser20, func.__name__, func)
    return func


def cpp23(func):
    setattr(CppParser23, func.__name__, func)
    return func


def disabled(func):
    setattr(CppParser98, func.__name__, _empty_rule)
    return func


def deprecate_cpp03(func):
    setattr(CppParser03, func.__name__, _empty_rule)
    return func


def deprecate_cpp11(func):
    setattr(CppParser11, func.__name__, _empty_rule)
    return func


def deprecate_cpp14(func):
    setattr(CppParser14, func.__name__, _empty_rule)
    return func


def deprecate_cpp17(func):
    setattr(CppParser17, func.__name__, _empty_rule)
    return func


def deprecate_cpp20(func):
    setattr(CppParser20, func.__name__, _empty_rule)
    return func


def deprecate_cpp23(func):
    setattr(CppParser23, func.__name__, _empty_rule)
    return func


@cpp98
def p_error(self, p):
    # type: (CppParser, lex.LexToken) -> None
    """
        error :
    """
    if p:
        self.lexer.logger.C0011(self.lexer.position(p), p.value, p.type)
    else:
        self.lexer.logger.C0012(self.lexer.eof_position())


_parsers = {
    'c++98': CppParser98,
    'c++03': CppParser03,
    'c++11': CppParser11,
    'c++14': CppParser14,
    'c++17': CppParser17,
    'c++20': CppParser20,
    'c++23': CppParser23,
}


def parser(std):
    # type: (str) -> Type[CppParser]
    return _parsers[std]


from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from ply import lex
    from typing import Type
    from types import ModuleType
    from .cpp_messages import Logger
    from .cpp_ast.cppobject import CppObject