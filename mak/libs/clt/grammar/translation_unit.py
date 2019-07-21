from ..cl_ast.namespaces import RootNamespace


def p_translation_unit_start(p):
    """
        translation_unit_start :
    """
    p[0] = RootNamespace(p.lexer)
    p[0].push_scope(None)


def p_translation_unit_end(p):
    """
        translation_unit_end :
    """
    tu = p[-2]
    p.lexer.pop_scope(tu.scope)
    assert not p.lexer.scopes, 'internal error: asymetric scope push/pop'


def p_translation_unit(p):
    """
        translation_unit_or_empty : translation_unit_start external_declarations translation_unit_end
    """
    p[0] = p[1]
