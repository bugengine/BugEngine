import os

from ..ply import yacc
from . import cl_lexer


def set_position(p, s1, s2):
    p.slice[s1].filename, p.slice[s1].lineno, p.slice[s1].lexpos, p.slice[s1].endlexpos = p.position(s2)

def set_position_absolute(p, s1, position):
    p.slice[s1].filename, p.slice[s1].lineno, p.slice[s1].lexpos, p.slice[s1].endlexpos = position

def position(p, s):
    if s >= 0:
        return (getattr(p.slice[s], 'filename', ''), p.lineno(s),) + p.lexspan(s)
    else:
        production = p.stack[s]
        startpos = getattr(production, 'lexpos', 0)
        endpos = getattr(production, 'endlexpos', startpos)
        return (getattr(production, 'filename', ''), getattr(production, 'lineno', 0), startpos, endpos)

yacc.YaccProduction.set_position = set_position
yacc.YaccProduction.set_position_absolute = set_position_absolute
yacc.YaccProduction.position = position


class ClParser:
    from . import grammar
    grammar.tokens = cl_lexer.ClLexer.tokens

    def __init__(self, tmp_dir):
        self.parser = yacc.yacc(module = self.grammar,
                                start = 'translation_unit_or_empty',
                                picklefile = os.path.join(tmp_dir, 'cl_grammar.pickle'),
                                debugfile = os.path.join(tmp_dir, 'cl_grammar.debug'),
                                debug = 1)

    def parse(self, error_format, filename):
        with open(filename, 'r') as input:
            self.lexer = cl_lexer.ClLexer(filename, error_format)
            self.lexer.parser = self.parser
            result = self.parser.parse(input.read(), lexer=self.lexer)

        if result:
            #result.debug_dump()
            result.error_count = self.lexer.error_count
        return result
