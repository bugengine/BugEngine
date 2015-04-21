from cpp.tokens import tokens
from cpp import lexer
from cpp.grammar import *
from cpp.ply import lex, yacc
import sys

def p_error(p):
    sys.stderr.write("%s:%d: error: Syntax error near unexpected token %s\n" %
            (p.lexer.source, p.lineno, p.type.lower()))
    p.lexer.error_count += 1

def parse(filename, pickle_file, macro_files, module_name):
    for f in macro_files or []:
        with open(f, 'r') as macro_defs:
            for m in macro_defs.readlines():
                lexer.lexer.define_macro(m)
    with open(filename, 'r') as input:
        lexer.lexer.source = filename
        parser = yacc.yacc(start='unit', picklefile=pickle_file)
        parser.source = filename
        parser.root_namespace = module_name

        result = parser.parse(input.read(), lexer=lexer.lexer)

    result.error_count = lexer.lexer.error_count
    result.includes = lexer.lexer.includes
    return result
