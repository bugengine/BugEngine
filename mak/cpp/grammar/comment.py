def p_comment_left(p):
    """
        tag : DOXYGEN_BEGIN skip_doxy DOXYGEN_END
    """


def p_comment_right(p):
    """
        doc_left : DOXYGEN_BEGIN_LEFT skip_doxy DOXYGEN_END doc_left
                 |
    """

