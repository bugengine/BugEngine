from cpp import parser as parser_cpp
from cxx import cxx_parser
from ircc import ir_parser


def build(build_context):
    parser_cpp.build_parser(build_context.bldnode.parent.parent.make_node('cpp_grammar.pickle').abspath())
    cxx_parser.CxxParser98(build_context.bldnode.parent.parent.abspath())
    cxx_parser.CxxParser11(build_context.bldnode.parent.parent.abspath())
    ir_parser.IrParser(build_context.bldnode.parent.parent.abspath())
