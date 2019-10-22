import sys
import traceback

def p_error(p):
    if p:
        p.lexer.error("syntax error near token %s (%s)" % (p.value, p.type),
                      (p.filename, p.lineno, p.lexpos, p.endlexpos))
    else:
        sys.stderr.write("error: unexpected EOF\n")
