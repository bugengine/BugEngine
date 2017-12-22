import sys

def p_error(p):
    if p:
        sys.stderr.write("%s:%d: error: Syntax error near unexpected token %s (%s)\n" %
                         (p.lexer.filename, p.lineno, p.value, p.type))
        p.lexer.error_count += 1
    else:
        sys.stderr.write("error: unexpected EOF\n")
