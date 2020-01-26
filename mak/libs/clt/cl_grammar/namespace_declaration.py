from ..cl_ast import Namespace, AnonymousNamespace
from be_typing import TYPE_CHECKING


def p_namespace_declaration_new(p):
    # type: (YaccProduction) -> None
    """
        namespace_declaration : NAMESPACE namespace_name
                              | NAMESPACE type_name
                              | NAMESPACE object_name
    """
    name = p[2][1]
    if name.get_type() != 'ID' and (name.is_qualified() or not name.is_shadow()):
        if name.get_type() == 'NAMESPACE_ID':
            p[0] = name.target
            p[0].push_scope(name.position)
        else:
            p.lexer.error("redefinition of '%s' as a different kind of symbol" % name, p.position(1))
            p.lexer.note('previous definition is here', name.target.position)
            p[0] = Namespace(p.lexer, p.position(2), name.name)
            p[0].push_scope(name.position)
    else:
        p[0] = Namespace(p.lexer, name.position, name.name)
        p[0].push_scope(name.position)


def p_namespace_declaration_anonymous(p):
    # type: (YaccProduction) -> None
    """
        namespace_declaration_anonymous : NAMESPACE
    """
    p[0] = AnonymousNamespace(p.lexer, p.position(1))
    p[0].push_scope(p.position(1))


def p_namespace_pop(p):
    # type: (YaccProduction) -> None
    """
        namespace_pop :
    """
    namespace = p[-5]
    if namespace:
        p.lexer.pop_scope(namespace.scope)


def p_namespace_declaration(p):
    # type: (YaccProduction) -> None
    """
        namespace_declaration : namespace_declaration LBRACE BRACE_MARKER external_declarations RBRACE namespace_pop
                              | namespace_declaration_anonymous LBRACE BRACE_MARKER external_declarations RBRACE namespace_pop
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction