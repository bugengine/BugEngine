from ..cl_ast.ast_templates import Template, TemplateValueParameter, TemplateTypenameParameter, TemplateTemplateParameter
from be_typing import TYPE_CHECKING


def p_verify_template_stack_0(p):
    # type: (YaccProduction) -> None
    """
        verify_template_stack_0 :
    """
    p.lexer.finalize_template_stack()


def p_verify_template_stack_1(p):
    # type: (YaccProduction) -> None
    """
        verify_template_stack_1 :
    """
    p.lexer.finalize_template_stack()
    p[0] = p[-1][1]
    p[0].parent and p[0].parent.bind()
    p[0].show_errors()


def p_verify_template_stack_1_opt(p):
    # type: (YaccProduction) -> None
    """
        verify_template_stack_1_opt :
    """
    if p.lexer.template_stack is not None:
        p.lexer.finalize_template_stack()
        p[0] = p[-1][1]
        p[0].parent and p[0].parent.bind()
        p[0].show_errors()
    else:
        p[0] = p[-1][0]


def p_template_argument(p):
    # type: (YaccProduction) -> None
    """
        template_argument : expression                                                  %prec NAME2
                          | type
    """
    p[0] = p[1]


def p_template_argument_list(p):
    # type: (YaccProduction) -> None
    """
        template_argument_list : template_argument COMMA template_argument_list         %prec NAME2
                               | template_argument                                      %prec NAME2
    """
    p[0] = [p[1]]
    if len(p) > 2:
        p[0] += p[3]


def p_disable_name_tpl_stack(p):
    # type: (YaccProduction) -> None
    """
        disable_name_tpl_stack :
    """
    p.lexer.disable_template_stack()


def p_enable_name_tpl_stack(p):
    # type: (YaccProduction) -> None
    """
        enable_name_tpl_stack :
    """
    p.lexer.enable_template_stack()


def p_template_arguments(p):
    # type: (YaccProduction) -> None
    """
        template_arguments : LT disable_name_tpl_stack template_argument_list enable_name_tpl_stack GT  %prec TEMPLATEGT
    """
    p[0] = p[3]


def p_template_arguments_empty(p):
    # type: (YaccProduction) -> None
    """
        template_arguments : LT GT                                                  %prec TEMPLATEGT
    """
    p[0] = []


def p_template_parameter_name(p):
    # type: (YaccProduction) -> None
    """
        template_parameter_name :   object_name
                                |   type_name
                                |   namespace_name
    """
    name = p[1][1]
    if name.is_qualified():
        p.lexer.logger.C0015(name.position)
    p[0] = name.name
    p.set_position_absolute(0, name.position)


def p_template_parameter_default_value_opt(p):
    # type: (YaccProduction) -> None
    """
        template_parameter_default_value_opt :  EQUALS expression
                                             |  EQUALS type
                                             |
    """
    if len(p) > 2:
        p[0] = p[2]


def p_template_parameter_value(p):
    # type: (YaccProduction) -> None
    """
        template_parameter : template_specifier_opt_detach type template_parameter_name template_parameter_default_value_opt
    """
    if p[1]:
        p.lexer.logger.C0016(p[1][0].position)
    p[0] = TemplateValueParameter(p.lexer, p[3] and p.position(3) or p[2].position, p[3], p[2], p[4])
    p[0].register()
    for t in p[1][::-1]:
        p.lexer.pop_scope(t.scope)


def p_template_parameter_typename(p):
    # type: (YaccProduction) -> None
    """
        template_parameter : template_specifier_opt_detach TYPENAME template_parameter_name template_parameter_default_value_opt
        template_parameter : template_specifier_opt_detach struct_keyword template_parameter_name template_parameter_default_value_opt
    """
    for t in p[1][::-1]:
        p.lexer.pop_scope(t.scope)
    if p[1]:
        if len(p[1]) > 1:
            p.lexer.logger.C0401(p[1][1].position)
        p[0] = TemplateTemplateParameter(
            p.lexer, p[3] and p.position(3) or p.position(2), p[3], p[1][0].parameters, p[4]
        )
    else:
        p[0] = TemplateTypenameParameter(p.lexer, p[3] and p.position(3) or p.position(2), p[3], p[4])
    p[0].register()


def p_template_parameter_list(p):
    # type: (YaccProduction) -> None
    """
        template_parameter_list : template_parameter_list COMMA template_parameter
                                | template_parameter
    """
    if len(p) > 2:
        p[0] = p[1] + [p[3]]
    else:
        p[0] = [p[1]]


def p_template_parameters(p):
    # type: (YaccProduction) -> None
    """
        template_parameters : template_parameter_list
    """
    p[0] = p[1]


def p_template_parameters_none(p):
    # type: (YaccProduction) -> None
    """
        template_parameters :
    """
    p[0] = []


def p_template_push(p):
    # type: (YaccProduction) -> None
    """
        template_push :
    """
    p[0] = Template(p.lexer, p.position(-1))
    p[0].register()


def p_template_specifier(p):
    # type: (YaccProduction) -> None
    """
        template_specifier : TEMPLATE LT template_push template_parameters GT
    """
    p[0] = p[3]


def p_template_specifier_list(p):
    # type: (YaccProduction) -> None
    """
        template_specifier_list : template_specifier template_specifier_list
    """
    p[0] = [p[1]] + p[2]


def p_template_specifier_list_end(p):
    # type: (YaccProduction) -> None
    """
        template_specifier_list :
    """
    p[0] = []


def p_template_specifier_opt(p):
    # type: (YaccProduction) -> None
    """
        template_specifier_opt : template_specifier_list
    """
    p[0] = p[1]
    p.lexer.push_template_stack(p[0])


def p_template_create(p):
    # type: (YaccProduction) -> None
    """

        template_create :
    """
    p[0] = Template(p.lexer, p.position(-1))


def p_template_specifier_opt_detach(p):
    # type: (YaccProduction) -> None
    """
        template_specifier_opt_detach : TEMPLATE LT template_create template_parameters GT template_specifier_opt_detach
    """
    p[0] = p[6] + [p[3]]


def p_template_specifier_opt_detach_none(p):
    # type: (YaccProduction) -> None
    """
        template_specifier_opt_detach :
    """
    p[0] = []


if TYPE_CHECKING:
    from ply.yacc import YaccProduction