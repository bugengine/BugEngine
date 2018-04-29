import sys
import traceback

def p_error(p):
    if p:
        p.lexer._error("syntax error near token %s (%s)" % (p.value, p.type),
                       (p.lineno, p.lexpos, p.endlexpos))
        p.lexer.parser.errok()
    else:
        sys.stderr.write("error: unexpected EOF\n")
