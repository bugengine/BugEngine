from . import cl_lexer
from ..ply import yacc


def set_position(p, s1, s2):
    p.slice[s1].lineno = p.lineno(s2)
    p.slice[s1].lexpos, p.slice[s1].endlexpos = p.lexspan(s2)


def position(p, s):
    return (p.lineno(s),) + p.lexspan(s)


yacc.YaccProduction.set_position = set_position
yacc.YaccProduction.position = position


class ClParser:
    from . import grammar
    grammar.tokens = cl_lexer.ClLexer.tokens

    def __init__(self, pickle_file):
        self.parser = yacc.yacc(module = self.grammar,
                                start = 'translation_unit_or_empty',
                                picklefile = pickle_file,
                                debug = 1)

    def parse(self, error_format, filename):
        with open(filename, 'r') as input:
            self.lexer = cl_lexer.ClLexer(filename, error_format)
            result = self.parser.parse(input.read(), lexer=self.lexer)

        if result:
            result.error_count = self.lexer.error_count
        return result

