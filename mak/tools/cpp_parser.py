from cpp import parser as parser_cpp
from pyxx import cxx
from ircc import ir_parser


def build(build_context):
    parser_cpp.build_parser(build_context.bldnode.parent.parent.make_node('cpp_grammar.pickle').abspath())
    #cxx.Cxx98Parser(build_context.bldnode.parent.parent.abspath())
    #cxx.Cxx11Parser(build_context.bldnode.parent.parent.abspath())
    cxx.Cxx23Parser(build_context.bldnode.parent.parent.abspath())
    ir_parser.IrParser(build_context.bldnode.parent.parent.abspath())
