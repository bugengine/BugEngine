from ..cl_ast.ast_types import Struct
from typing import TYPE_CHECKING


def p_struct_keyword(p):
    # type: (YaccProduction) -> None
    """
        struct_keyword : STRUCT
                       | UNION
                       | CLASS
    """
    p[0] = p[1]
    p.set_position(0, 1)


def p_struct_declaration(p):
    # type: (YaccProduction) -> None
    """
        struct_header : struct_keyword type_name verify_template_stack_1
                      | struct_keyword object_name verify_template_stack_1
                      | struct_keyword namespace_name verify_template_stack_1
    """
    name = p[2][1]
    if name.template_bindings and name.template:
        name.template_bindings.template.bind(name.template)
    struct_type = p[1]
    object_type = name.get_type()
    if object_type != 'ID' and (name.is_qualified() or not name.is_shadow()):
        if object_type == 'STRUCT_ID':
            if name.target.struct_type != struct_type:
                p.lexer.warning(
                    "'%s' declared as %s here, but first declared as %s" % (name, p[1], name.target.struct_type),
                    p.position(1)
                )
                p.lexer.note('previously declared here', name.target.position)
            #if name.template:
            #    p.lexer.error("redefinition of '%s' as a different kind of symbol" % name, name.position)
            #    p.lexer.note('previous definition is here', name.target.position)
            #    p[0] = (None, Struct(p.lexer, name.position, struct_type, name.name))
            p[0] = (name.target, None)
        else:
            if name.is_qualified():
                p.lexer.error('qualified name %s does not name a struct' % name, name.position)
            else:
                p.lexer.error('name %s does not name a struct' % name, name.position)
            p.lexer.note('previously declared here', name.target.position)
            p[0] = (None, Struct(p.lexer, name.position, struct_type, name.name))
    elif name.target and object_type == 'STRUCT_ID':
        # optimistically use provided declaration, but if definition, it will be overriden
        p[0] = (name.target, Struct(p.lexer, name.position, struct_type, name.name))
    else:  # new structure or new specialization
        assert object_type == 'ID' or (name.is_shadow() and object_type != 'STRUCT_ID')
        t = name.template
        if not t or name.is_shadow():
            p[0] = (None, Struct(p.lexer, name.position, struct_type, name.name))
        else:
            if name.target:
                p[0] = (name.target, None)
            elif name.arguments:
                p[0] = (Struct(p.lexer, name.position, struct_type, name.name), None)
                t.back_link.create_specialization(name.arguments, p[0][0])
            else:
                p[0] = (None, Struct(p.lexer, name.position, struct_type, name.name))
    p.set_position_absolute(0, name.position)


def p_struct_push(p):
    # type: (YaccProduction) -> None
    """
        struct_push :
    """
    p[0] = p[-1][1]
    if not p[0]:
        p[0] = p[-1][0]
    else:
        p[0].register()


def p_struct_pop(p):
    # type: (YaccProduction) -> None
    """
        struct_pop :
    """
    struct = p[-3]
    struct.pop_scope_recursive()


def p_struct_parent_visibility(p):
    # type: (YaccProduction) -> None
    """
        struct_parent_visibility_opt : PUBLIC
                                     | PROTECTED
                                     | PRIVATE
                                     |
    """
    if len(p) > 1:
        p[0] = p[1]


def p_struct_parent(p):
    # type: (YaccProduction) -> None
    """
        struct_parent_opt : COLON struct_parent_visibility_opt type_name
                          |
    """
    if len(p) > 1:
        p[0] = (p[2], p[3][0].target.get_type())
    else:
        p[0] = (None, None)


def p_struct_parent_error(p):
    # type: (YaccProduction) -> None
    """
        struct_parent_opt : COLON struct_parent_visibility_opt object_name
    """
    name = p[3][0]
    p.lexer.error('expected class name', name.position)


def p_struct_define(p):
    # type: (YaccProduction) -> None
    """
        struct_define :
    """
    p[-2].define(p[-1][0], p[-1][1], p.position(-3))


def p_struct_begin(p):
    # type: (YaccProduction) -> None
    """
        struct_begin : struct_header struct_push struct_parent_opt struct_define LBRACE BRACE_MARKER
    """
    p[0] = p[2]
    p.set_position(0, 1)


def p_struct_definition(p):
    # type: (YaccProduction) -> None
    """
        struct_definition : struct_begin struct_declaration_list RBRACE struct_pop
    """
    p[0] = p[1]
    p.set_position(0, 1)


def p_struct_header_anonymous(p):
    # type: (YaccProduction) -> None
    """
        struct_header_anonymous : struct_keyword LBRACE BRACE_MARKER verify_template_stack_0
    """
    p[0] = Struct(p.lexer, p.position(1), p[1], None)
    p[0].register()
    p[0].define(None, p.position(2))
    p.set_position(0, 1)


def p_struct_definition_anonymous(p):
    # type: (YaccProduction) -> None
    """
        struct_definition : struct_header_anonymous struct_declaration_list RBRACE struct_pop
    """
    p[0] = p[1]
    p.set_position(0, 1)


def p_type_struct_declaration(p):
    # type: (YaccProduction) -> None
    """
        typedecl : struct_header
    """
    p[0] = p[1][0]
    if not p[0]:
        p[0] = p[1][1]
        p[0].register()
    elif p[1][1]:
        if p[1][0].struct_type != p[1][1].struct_type:
            p.lexer.warning(
                "'%s' declared as %s here, but first declared as %s" %
                (p[1][1].name, p[1][1].struct_type, p[1][0].struct_type), p[1][1].position
            )
            p.lexer.note('previously declared here', p[1][0].position)
    p.set_position(0, 1)


def p_type_struct_definition(p):
    # type: (YaccProduction) -> None
    """
        type_definition : struct_definition
    """
    p[0] = p[1]
    p.set_position(0, 1)


def p_struct_declaration_list(p):
    # type: (YaccProduction) -> None
    """
        struct_declaration_list : external_declaration struct_declaration_list
                                | struct_declaration struct_declaration_list
    """
    pass


def p_struct_declaration_list_end(p):
    # type: (YaccProduction) -> None
    """
        struct_declaration_list :
    """
    pass


def p_struct_declaration_scope(p):
    # type: (YaccProduction) -> None
    """
        struct_declaration : PUBLISHED COLON
                           | PUBLIC COLON
                           | PROTECTED COLON
                           | PRIVATE COLON
    """
    pass


def p_struct_declaration_friend_template(p):
    # type: (YaccProduction) -> None
    """
        struct_declaration : template_specifier_opt FRIEND declaration_specifier_list struct_keyword type_name SEMI
                           | template_specifier_opt FRIEND declaration_specifier_list struct_keyword object_name SEMI
    """
    p.lexer.finalize_template_stack()
    for t in p[1][::-1]:
        p.lexer.pop_scope(t.scope)


def p_struct_declaration_friend_template_method(p):
    # type: (YaccProduction) -> None
    """
        struct_declaration : template_specifier_opt FRIEND method_declaration SEMI
    """
    for t in p[1][::-1]:
        p.lexer.pop_scope(t.scope)


if TYPE_CHECKING:
    from ply.yacc import YaccProduction