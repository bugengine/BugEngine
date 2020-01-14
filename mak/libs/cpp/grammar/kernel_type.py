from ..tree import Parameter


def p_kernel_method_inout(p):
    """
        param_type : BE_SEGMENT
                   | BE_SEGMENTS
                   | BE_STREAM
    """
    p[0] = p[1]


def p_kernel_method_arg(p):
    """
        method_arg : param_type LEFT_PARENTHESIS type RIGHT_PARENTHESIS ID
    """
    p[0] = Parameter(p[3], p[5], '')
    p[0].add_tag(('kernel_param', p[1]))
