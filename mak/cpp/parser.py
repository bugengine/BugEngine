from cpp.tokens import tokens
from cpp import lexer
from cpp.grammar import *
from cpp.ply import lex, yacc
import sys
try:
    from cStringIO import StringIO
except ImportError:
    try:
        from StringIO import StringIO
    except ImportError:
        from io import StringIO

def p_error(p):
    sys.stderr.write("%s:%d: error: Syntax error near unexpected token %s\n" %
            (p.lexer.source, p.lineno, p.type.lower()))
    p.lexer.error_count += 1

def parse(filename, class_output, instance_output, doc_output, pickle_file,
          macro_files, precompiled_header, module_name):
    for f in macro_files or []:
        with open(f, 'r') as macro_defs:
            for m in macro_defs.readlines():
                lexer.lexer.define_macro(m)
    with open(filename, 'r') as input:
        lexer.lexer.source = filename
        lexer.lexer.classes = StringIO()
        lexer.lexer.instances = StringIO()
        lexer.lexer.docs = StringIO()
        parser = yacc.yacc(start='unit', picklefile=pickle_file)
        parser.source = filename
        parser.classes = lexer.lexer.classes
        parser.instances = lexer.lexer.instances
        parser.docs = lexer.lexer.docs
        parser.root_namespace = module_name

        if precompiled_header:
            parser.classes.write('#include <%s>\n' % precompiled_header)
            parser.instances.write('#include <%s>\n' % precompiled_header)
        parser.classes.write('#include <%s>\n' % filename)
        parser.instances.write('#include <%s>\n' % filename)
        parser.parse(input.read(), lexer=lexer.lexer)

    with open(class_output, 'w') as classes:
        classes.write(parser.classes.getvalue())
    with open(instance_output, 'w') as instances:
        instances.write(parser.instances.getvalue())
    with open(doc_output, 'w') as docs:
        docs.write(parser.docs.getvalue())
    return lexer.lexer.error_count
