import os
from ply import yacc
from . import ir_lexer
from .ir_position import IrPosition


def set_position(p, s1, s2):
    # type: (yacc.YaccProduction, int, int) -> None
    p.slice[s1].position = position(p, s2)


def set_position_absolute(p, s1, position):
    # type: (yacc.YaccProduction, int, IrPosition) -> None
    p.slice[s1].position = position


def position(p, s):
    # type: (yacc.YaccProduction, int) -> IrPosition
    if s >= 0:
        return p.slice[s].position
    else:
        return p.stack[s].position


def p_error(self, p):
    # type: (IrParser, lex.LexToken) -> None
    """
        error :
    """
    if p:
        self.lexer.logger.C0003(p.position, p.value, p.type)
    else:
        self.lexer.logger.C0004(self.lexer.eof_position())


setattr(yacc.YaccProduction, 'set_position', set_position)
setattr(yacc.YaccProduction, 'set_position_absolute', set_position_absolute)
setattr(yacc.YaccProduction, 'position', position)


class IrParser:
    from . import ir_grammar

    def __init__(self, tmp_dir):
        # type: (str)->None
        self.parser = yacc.yacc(
            module=self.ir_grammar,
            start='translation-unit',
            picklefile=os.path.join(tmp_dir, 'ir_grammar.pickle'),
            debugfile=os.path.join(tmp_dir, 'ir_grammar.debug'),
            debug=True,
        )
        setattr(self.parser, 'errorfunc', lambda p: p_error(self, p))

    def parse(self, logger, filename):
        # type: (Logger, str) -> IrObject
        with open(filename, 'r') as input:
            self.lexer = ir_lexer.IrLexer(filename, logger)
            result = self.parser.parse(input.read(), lexer=self.lexer)
        if result:
            # result.debug_dump()
            result.error_count = logger._error_count
            pass
        return result


from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from .ir_messages import Logger
    from .ir_ast.ir_object import IrObject
    from ply import lex, yacc