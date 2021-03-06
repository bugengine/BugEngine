from .tokens import tokens
from . import lexer
from .grammar import *
from ply import lex, yacc
import sys


def build_parser(pickle_file):
    return yacc.yacc(start='unit', picklefile=pickle_file, debugfile=pickle_file + '.debug', debug=1)


def p_error(p):
    sys.stderr.write("%s:%d: error: Syntax error near unexpected token %s\n" % (p.lexer.source, p.lineno, p.value))
    p.lexer.error_count += 1


def parse(filename, pickle_file, macro_files, module_name, root_alias):
    for f in macro_files or []:
        with open(f, 'r') as macro_defs:
            for m in macro_defs.readlines():
                lexer.lexer.define_macro(m)
    with open(filename, 'r') as input:
        lexer.lexer.source = filename
        parser = build_parser(pickle_file)
        parser.source = filename
        parser.root_namespace = module_name
        parser.root_alias = root_alias

        result = parser.parse(input.read(), lexer=lexer.lexer)

    if result:
        result.error_count = lexer.lexer.error_count
        result.includes = lexer.lexer.includes
        result.usings = lexer.lexer.usings
    return result
