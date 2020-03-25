import os
from ply import yacc
from . import cl_lexer
from .cl_position import ClPosition


def set_position(p, s1, s2):
    # type: (yacc.YaccProduction, int, int) -> None
    p.slice[s1].position = position(p, s2)


def set_position_absolute(p, s1, position):
    # type: (yacc.YaccProduction, int, ClPosition) -> None
    p.slice[s1].position = position


def position(p, s):
    # type: (yacc.YaccProduction, int) -> ClPosition
    if s >= 0:
        return p.slice[s].position
    else:
        return p.stack[s].position


setattr(yacc.YaccProduction, 'set_position', set_position)
setattr(yacc.YaccProduction, 'set_position_absolute', set_position_absolute)
setattr(yacc.YaccProduction, 'position', position)


class ClParser:
    from . import cl_grammar

    def __init__(self, tmp_dir):
        # type: (str)->None
        self.parser = yacc.yacc(
            module=self.cl_grammar,
            start='translation-unit',
            picklefile=os.path.join(tmp_dir, 'cl_grammar.pickle'),
            debugfile=os.path.join(tmp_dir, 'cl_grammar.debug'),
            debug=True
        )

    def parse(self, logger, filename):
        # type: (Logger, str) -> CppObject
        with open(filename, 'r') as input:
            self.lexer = cl_lexer.ClLexer(filename, logger)
            result = self.parser.parse(input.read(), lexer=self.lexer)
        if result:
            # result.debug_dump()
            result.error_count = logger._error_count
            pass
        return result


from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from .cl_messages import Logger
    from .cl_ast.cppobject import CppObject