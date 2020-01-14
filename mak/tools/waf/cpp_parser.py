from cpp import parser as parser_cpp
from clt.cl_parser import ClParser


def build(build_context):
    parser_cpp.build_parser(build_context.bldnode.parent.parent.make_node('cpp_grammar.pickle').abspath())
    ClParser(build_context.bldnode.parent.parent.abspath())
