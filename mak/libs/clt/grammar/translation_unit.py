from .. import cl_ast


def p_translation_unit_start(p):
    """
        translation_unit_start :
    """
    p[0] = cl_ast.namespaces.Root(p)
    p.lexer.push_scope(p[0])


def p_translation_unit_end(p):
    """
        translation_unit_end :
    """
    p.lexer.pop_scope()


def p_translation_unit(p):
    """
        translation_unit_or_empty : translation_unit_start external_declarations translation_unit_end
    """
    p[0] = p[1]
