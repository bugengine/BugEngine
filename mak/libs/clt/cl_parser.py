import os
from ply import yacc
from . import cl_lexer
from .cl_ast import namespaces
from .cl_ast.position import Position


def set_position(p, s1, s2):
    # type: (yacc.YaccProduction, int, int) -> None
    p.slice[s1].position = position(p, s2)


def set_position_absolute(p, s1, position):
    # type: (yacc.YaccProduction, int, Position) -> None
    p.slice[s1].position = position


def position(p, s):
    # type: (yacc.YaccProduction, int) -> Position
    if s >= 0:
        return Position(getattr(p.slice[s], 'filename', ''), p.lineno(s), *p.lexspan(s), lexdata=p.lexer.lexdata())
    else:
        production = p.stack[s]
        try:
            return production.position
        except AttributeError:
            startpos = getattr(production, 'lexpos', 0)
            endpos = getattr(production, 'endlexpos', startpos)
            return Position(
                getattr(production, 'filename', ''), getattr(production, 'lineno', 0), startpos, endpos,
                p.lexer.lexdata()
            )


setattr(yacc.YaccProduction, 'set_position', set_position)
setattr(yacc.YaccProduction, 'set_position_absolute', set_position_absolute)
setattr(yacc.YaccProduction, 'position', position)


class ClParser:
    from . import cl_grammar

    def __init__(self, tmp_dir):
        # type: (str)->None
        self.parser = yacc.yacc(
            module=self.cl_grammar,
            start='translation_unit_or_empty',
            picklefile=os.path.join(tmp_dir, 'cl_grammar.pickle'),
            debugfile=os.path.join(tmp_dir, 'cl_grammar.debug'),
            debug=True
        )

    def parse(self, logger, filename):
        # type: (Logger, str)->namespaces.RootNamespace
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