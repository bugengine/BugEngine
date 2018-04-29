from mak.libs.cpp import parser as parser_cpp
from mak.libs.clt.cl_parser import ClParser

def build(build_context):
    parser_cpp.build_parser(build_context.bldnode.make_node('cpp_grammar.pickle').abspath())
    ClParser(build_context.bldnode.abspath())
