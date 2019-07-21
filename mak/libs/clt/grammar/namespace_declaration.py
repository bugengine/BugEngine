from ..cl_ast.namespaces import Namespace, AnonymousNamespace


def p_new_namespace_name(p):
    """
        new_namespace_name : ID
                           | STRUCT_ID_SHADOW
                           | TYPENAME_ID_SHADOW
                           | NAMESPACE_ID_SHADOW
                           | METHOD_ID_SHADOW
                           | VARIABLE_ID_SHADOW
                           | TEMPLATE_STRUCT_ID_SHADOW
                           | TEMPLATE_METHOD_ID_SHADOW
                           | TEMPLATE_TYPENAME_ID_SHADOW
    """
    p[0] = p[1]
    p.set_position(0, 1)


def p_existing_namespace_name(p):
    """
        existing_namespace_name : NAMESPACE_ID
    """
    p[0] = p.slice[1].found_object
    p.set_position(0, 1)


def p_new_namespace_name_invalid(p):
    """
        new_namespace_name : STRUCT_ID
                           | TYPENAME_ID
                           | METHOD_ID
                           | VARIABLE_ID
                           | TEMPLATE_STRUCT_ID
                           | TEMPLATE_METHOD_ID
                           | TEMPLATE_TYPENAME_ID
    """
    p[0] = p[1]
    p.set_position(0, 1)
    p._lexer.error('redefinition of %s as a namespace' % p[0])
    p.slice[1].found_object._note('previously declared here')
    p[0].push_scope(p.position(1))


def p_namespace_declaration_new(p):
    """
        namespace_declaration_new : NAMESPACE new_namespace_name
    """
    p[0] = Namespace(p.lexer, p.position(2), p[2])
    p[0].push_scope(p.position(2))


def p_namespace_declaration_existing(p):
    """
        namespace_declaration_existing : NAMESPACE existing_namespace_name
    """
    p[0] = p[2]
    p[0].push_scope(p.position(2))


def p_namespace_declaration_anonymous(p):
    """
        namespace_declaration_anonymous : NAMESPACE
    """
    p[0] = AnonymousNamespace(p.lexer, p[1].position)
    p[0].push_scope(p[1].position)


def p_namespace_pop(p):
    """
        namespace_pop :
    """
    namespace = p[-4]
    if namespace:
        p.lexer.pop_scope(namespace.scope)


def p_namespace_declaration(p):
    """
        namespace_declaration : namespace_declaration_new LBRACE external_declarations RBRACE namespace_pop
                              | namespace_declaration_existing LBRACE external_declarations RBRACE namespace_pop
                              | namespace_declaration_anonymous LBRACE external_declarations RBRACE namespace_pop
    """
