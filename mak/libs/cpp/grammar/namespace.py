from ..tree import Namespace, AnonymousNamespace


def p_namespace_header(p):
    """
        namespace_header : NAMESPACE ID LEFT_BRACE
                         | NAMESPACE LEFT_BRACE
    """
    if len(p) == 4:
        p[0] = Namespace(p.parser.root_namespace, p.parser.stack[-1].name + [p[2]], p.parser.root_alias)
    else:
        p[0] = AnonymousNamespace()
    p.parser.stack.append(p[0])


def p_namespace(p):
    """
        namespace : namespace_header exprs RIGHT_BRACE
    """
    p[0] = p.parser.stack.pop()
    if not p[1].anonymous:
        p.parser.stack[-1].add_object(p[1])


def p_namespace_error(p):
    """
        namespace : namespace_header error RIGHT_BRACE
    """
    p[0] = p.parser.stack.pop()
    if not p[1].anonymous:
        p.parser.stack[-1].add_object(p[1])


def p_namespace_expr(p):
    """
        expr : namespace
    """
