from mak.libs.cpp import parser as parser_cpp
from mak.libs.ply import yacc

def build(build_context):
    parser_cpp.build_parser(build_context.bldnode.parent.make_node('cpp_grammar.pickle').abspath())
