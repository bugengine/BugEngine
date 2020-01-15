from ..cl_ast.ast_types import Array, Pointer
from ..cl_ast import CastError, CastOptions
from ..cl_ast import Variable
from be_typing import TYPE_CHECKING


def p_variable_initial_value_opt(p):
    # type: (YaccProduction) -> None
    """
        variable_initial_value_opt :
    """
    p[0] = None


def p_variable_initial_value(p):
    # type: (YaccProduction) -> None
    """
        variable_initial_value_opt : EQUALS expression
    """
    p[0] = p[2]


def p_variable_initial_value_constructor(p):
    # type: (YaccProduction) -> None
    """
        variable_initial_value_opt : LPAREN expression_list RPAREN
    """
    p[0] = p[2]


def p_type_modifier_opt_end(p):
    # type: (YaccProduction) -> None
    """
        type_modifier_opt :
    """
    p[0] = p[-1]


def p_type_modifier_opt(p):
    # type: (YaccProduction) -> None
    """
        type_modifier_opt : type_modifier_opt TIMES
    """
    p[0] = Pointer(p.lexer, p.position(2), p[1])


def p_variable_array_specifier_opt_end(p):
    # type: (YaccProduction) -> None
    """
        variable_array_specifier_opt :
    """
    p[0] = p[-3]


def p_variable_array_specifier_opt(p):
    # type: (YaccProduction) -> None
    """
        variable_array_specifier_opt : variable_array_specifier_opt LBRACKET expression RBRACKET
    """
    p[0] = Array(p.lexer, p.position(2), p[1], p[3])


def p_variable_array_specifier_opt_error(p):
    # type: (YaccProduction) -> None
    """
        variable_array_specifier_opt : variable_array_specifier_opt LBRACKET RBRACKET
    """
    p[0] = Pointer(p.lexer, p.position(2), p[1])


def p_variable_declaration(p):
    # type: (YaccProduction) -> None
    """
        variable_declaration : object_declaration variable_initial_value_opt
    """
    attributes, _, name, owner, variable_type = p[1]
    owner.pop_scope_recursive()
    object_type = name.get_type()
    if object_type != 'ID' and (name.is_qualified() or not name.is_shadow()):
        if object_type != 'VARIABLE_ID':
            p.lexer.error('%s redeclared as different kind of symbol' % name, name.position)
            p.lexer.note('previous declaration of %s' % name, name.target.position)
            p[0] = Variable(p.lexer, name.position, name.name, variable_type, p[2], attributes)
        else:
            try:
                name.target.type.distance(variable_type, CastOptions(CastOptions.CAST_NONE))
            except CastError:
                p.lexer.error(
                    "redefinition of '%s' with a different type: '%s' vs '%s'" %
                    (name, variable_type.pretty_name(), name.target.type.pretty_name()), name.position
                )
                p.lexer.note('previous declaration of %s' % name, name.target.position)
            p[0] = [name.target]
    else:
        v = Variable(p.lexer, name.position, name.name, variable_type, p[2], attributes)
        v.register()
        p[0] = [v]
        for s in attributes:
            if s.specifier == 'inline':
                p.lexer.error('inline can only be used on functions', s.position)


def p_extract_original_type(p):
    # type: (YaccProduction) -> None
    """
        extract_original_type :
    """
    p[0] = p[-2][0].type.original_type()


def p_variable_declaration_cted(p):
    # type: (YaccProduction) -> None
    """
        variable_declaration : variable_declaration COMMA extract_original_type type_modifier_opt object_name variable_array_specifier_opt variable_initial_value_opt
    """
    name = p[5][1]
    object_type = name.get_type()
    variable_type = p[6]
    if object_type != 'ID' and (name.is_qualified() or not name.is_shadow()):
        if name.get_type() != 'VARIABLE_ID':
            p.lexer.error('%s redeclared as different kind of symbol', name.position)
            p.lexer.note('previous declaration of %s' % name, name.target.position)
            p[0] = Variable(p.lexer, name.position, name.name, variable_type, p[7], p[1][0].attributes)
        else:
            try:
                name.target.distance(variable_type, CastOptions(CastOptions.CAST_NONE))
            except CastError:
                p.lexer.error(
                    "redefinition of '%s' with a different type: '%s' vs '%s'" %
                    (name, variable_type.pretty_name(), name.target.type.pretty_name()), name.position
                )
                p.lexer.note('previous declaration of %s' % name, name.target.position)
            p[0] = [name.target]
    else:
        v = Variable(p.lexer, name.position, name.name, variable_type, p[7], p[1][0].attributes)
        v.register()
        p[0] = [v]
        for s in p[1]:
            if s.specifier == 'inline':
                p.lexer.error('inline can only be used on functions', s.position)


if TYPE_CHECKING:
    from ply.yacc import YaccProduction