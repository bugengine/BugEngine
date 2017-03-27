from mak.cpp import parser
from mak.cpp.ply import yacc

def build(build_context):
    parser.build_parser(build_context.bldnode.parent.make_node('cpp_grammar.pickle').abspath())
