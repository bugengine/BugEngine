from ..cl_ast.ast_types import Enum, EnumItem
from typing import TYPE_CHECKING
from ply.lex import LexToken


def p_enum_declaration(p):
    # type: (YaccProduction) -> None
    """
        enum_header : ENUM type_name verify_template_stack_1
                    | ENUM object_name verify_template_stack_1
                    | ENUM namespace_name verify_template_stack_1
    """
    name = p[2][1]
    struct_type = p[1]
    object_type = name.get_type()
    if object_type != 'ID' and (name.is_qualified() or not name.is_shadow()):
        if object_type == 'STRUCT_ID':
            if name.target.struct_type != struct_type:
                p.lexer.warning(
                    "'%s' declared as %s here, but first declared as %s" % (name, struct_type, name.target.struct_type),
                    p.position(2)
                )
                p.lexer.note('previously declared here', name.target.position)
            p[0] = (name.target, None)
        else:
            if name.is_qualified():
                p.lexer.error('qualified name %s does not name a struct' % name, name.position)
            else:
                p.lexer.error('name %s does not name a struct' % name, name.position)
            p.lexer.note('previously declared here', name.target.position)
            p[0] = (None, Enum(p.lexer, name.position, name.name))
    elif name.target:
        # optimistically use provided declaration, but if definition, it will be overriden
        if object_type == 'STRUCT_ID':
            p[0] = (name.target, Enum(p.lexer, name.position, name.name))
        else:
            # Previously declared object is not a type
            p[0] = (None, Enum(p.lexer, name.position, name.name))
    else:
        p[0] = (None, Enum(p.lexer, name.position, name.name))
    p.set_position_absolute(0, name.position)


def p_enum_push(p):
    # type: (YaccProduction) -> None
    """
        enum_push :
    """
    p[0] = p[-1][1]
    if not p[0]:
        p[0] = p[-1][0]
    else:
        p[0].register()
    p[0].define(p.position(-2))


def p_enum_pop(p):
    # type: (YaccProduction) -> None
    """
        enum_pop :
    """
    enum = p[-3]
    enum.pop_scope_recursive()


def p_enum_begin(p):
    # type: (YaccProduction) -> None
    """
        enum_begin : enum_header enum_push LBRACE BRACE_MARKER
    """
    p[0] = p[2]
    p.set_position(0, 1)


def p_enum_definition(p):
    # type: (YaccProduction) -> None
    """
        enum_definition : enum_begin enum_declaration_list RBRACE enum_pop
    """
    p[0] = p[1]
    p.set_position(0, 1)


def p_enum_header_anonymous(p):
    # type: (YaccProduction) -> None
    """
        enum_header_anonymous : ENUM verify_template_stack_0 LBRACE BRACE_MARKER
    """
    p[0] = Enum(p.lexer, p.position(1), None)
    p[0].register()
    p[0].define(p.position(3))
    p.set_position(0, 1)


def p_enum_definition_anonymous(p):
    # type: (YaccProduction) -> None
    """
        enum_definition : enum_header_anonymous enum_declaration_list RBRACE enum_pop
    """
    p[0] = p[1]
    p.set_position(0, 1)


def p_enum_declaration_list(p):
    # type: (YaccProduction) -> None
    """
        enum_declaration_list : enum_declaration_list_nonempty
                              |
    """


def p_enum_declaration_list_nonempty(p):
    # type: (YaccProduction) -> None
    """
        enum_declaration_list_nonempty : enum_item COMMA enum_declaration_list_nonempty
                                       | enum_item
    """


def p_enum_value_name(p):
    # type: (YaccProduction) -> None
    """
        enum_value_name : ID
    """
    p[0] = p[1]
    p.set_position(0, 1)


def p_enum_value_name_shadows(p):
    # type: (YaccProduction) -> None
    """
        enum_value_name : METHOD_ID_SHADOW
                        | VARIABLE_ID_SHADOW
                        | STRUCT_ID_SHADOW
                        | TYPENAME_ID_SHADOW
    """
    assert isinstance(p.slice[1], LexToken)
    if p.slice[1].found_object:
        owner = p.slice[1].found_object.parent
        enum_namespace = p.lexer.scopes[-2].owner
        if owner == enum_namespace:
            p.lexer.error("redefinition of '%s'" % p[1], p.position(1))
            p.lexer.note('previous definition is here', p.slice[1].found_object.position)
    p[0] = p[1]
    p.set_position(0, 1)


def p_enum_value_error(p):
    # type: (YaccProduction) -> None
    """
        enum_value_name : VARIABLE_ID
    """
    # TODO
    p[0] = p[1]
    p.set_position(0, 1)


def p_enum_value_value_optional(p):
    # type: (YaccProduction) -> None
    """
        enum_value_value_optional : EQUALS expression
                                  |
    """
    if len(p) > 1:
        p[0] = p[2]


def p_enum_declaration_item(p):
    # type: (YaccProduction) -> None
    """
        enum_item : enum_value_name enum_value_value_optional
    """
    p[0] = EnumItem(p.lexer, p.position(1), p[1], p[2])
    p[0].register()


def p_type_enum_declaration(p):
    # type: (YaccProduction) -> None
    """
        typedecl : enum_header
    """
    p[0] = p[1][0]
    if not p[0]:
        p[0] = p[1][1]
        p[0].register()
    p.set_position(0, 1)


def p_type_enum_definition(p):
    # type: (YaccProduction) -> None
    """
        type_definition : enum_definition
    """
    p[0] = p[1]
    p.set_position(0, 1)


if TYPE_CHECKING:
    from ply.yacc import YaccProduction