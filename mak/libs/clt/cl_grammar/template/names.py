"""
simple-template-id:
      template-name < template-argument-listopt >     C++0x
template-id:
      simple-template-id     C++0x
      operator-function-id < template-argument-listopt >     C++0x
      literal-operator-id < template-argument-listopt >     C++0x
template-name:
      identifier
template-argument-list:
      template-argument ...opt     C++0x
      template-argument-list , template-argument ...opt     C++0x
template-argument:
      constant-expression     C++0x
      type-id     C++0x
      id-expression     C++0x
"""

from be_typing import TYPE_CHECKING


def p_simple_template_id(p):
    # type: (YaccProduction) -> None
    """
        simple-template-id : template-name OP_LT template-argument-list-opt OP_GT
    """


def p_class_template_id(p):
    # type: (YaccProduction) -> None
    """
        class-template-id : class-template-name OP_LT template-argument-list-opt OP_GT
    """

def p_template_id(p):
    # type: (YaccProduction) -> None
    """
        template-id : operator-function-id OP_LT template-argument-list-opt OP_GT
                    | literal-operator-id OP_LT template-argument-list-opt OP_GT
    """


def p_template_name(p):
    # type: (YaccProduction) -> None
    """
        template-name : TEMPLATE_IDENTIFIER
    """


def p_class_template_name(p):
    # type: (YaccProduction) -> None
    """
        class-template-name : CLASS_TEMPLATE_IDENTIFIER
    """


def p_template_argument_list_opt(p):
    # type: (YaccProduction) -> None
    """
        template-argument-list-opt : template-argument-list
    """
    # TODO: empty


def p_template_argument_list(p):
    # type: (YaccProduction) -> None
    """
        template-argument-list : template-argument ellipsis-opt
                               | template-argument-list COMMA template-argument ellipsis-opt
    """


def p_template_argument(p):
    # type: (YaccProduction) -> None
    """
        template-argument : constant-expression
                          | type-id
    """
    # id-expression is included in constant-expression


def p_ellipsis_opt(p):
    # type: (YaccProduction) -> None
    """
        ellipsis-opt : ELLIPSIS
    """
    # TODO: empty


if TYPE_CHECKING:
    from ply.yacc import YaccProduction