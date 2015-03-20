from cpp import tree


def p_unit_start(p):
    """
        unit_start :
    """
    p[0] = tree.Root(p.parser.root_namespace)
    p.parser.stack = [p[0]]


def p_unit(p):
    """
        unit : unit_start exprs
    """
    assert(len(p.parser.stack) == 1)
    p[0] = p[1]


