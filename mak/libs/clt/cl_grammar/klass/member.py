"""
member-specification:
      member-declaration member-specificationopt
      access-specifier : member-specificationopt
member-declaration:
      attribute-specifier-seqopt decl-specifier-seqopt member-declarator-listopt ;     C++0x
      function-definition ;opt
      using-declaration
      static_assert-declaration     C++0x
      template-declaration
      alias-declaration     C++0x
member-declarator-list:
      member-declarator
      member-declarator-list , member-declarator
member-declarator:
      declarator virt-specifier-seqopt pure-specifieropt
      declarator virt-specifier-seqopt brace-or-equal-initializeropt     C++0x
      identifieropt attribute-specifier-seqopt virt-specifier-seqopt : constant-expression
virt-specifier-seq:
      virt-specifier
      virt-specifier-seq virt-specifier
virt-specifier:
      override
      final
      new
pure-specifier:
      = 0
"""

from be_typing import TYPE_CHECKING


def p_member_specification(p):
    # type: (YaccProduction) -> None
    """
        member-specification : member-declaration member-specification-opt
                             | access-specifier COLON member-specification-opt
    """


def p_member_specification_opt(p):
    # type: (YaccProduction) -> None
    """
        member-specification-opt : member-specification
                                 | empty
    """


def p_member_declaration(p):
    # type: (YaccProduction) -> None
    """
        member-declaration : attribute-specifier-seq decl-specifier-seq member-declarator-list-opt SEMI
                           | attribute-specifier-seq member-declarator-list-opt SEMI
                           | decl-specifier-seq member-declarator-list-opt SEMI
                           | member-declarator-list-opt SEMI
                           | function-definition
                           | function-definition SEMI
                           | using-declaration
                           | static_assert-declaration
                           | template-declaration
                           | alias-declaration
    """


def p_member_declarator_list(p):
    # type: (YaccProduction) -> None
    """
        member-declarator-list : member-declarator
                               | member-declarator-list COMMA member-declarator
    """


def p_member_declarator_list_opt(p):
    # type: (YaccProduction) -> None
    """
        member-declarator-list-opt : member-declarator-list
                                   | empty
    """
    # TODO: empty


def p_member_declarator(p):
    # type: (YaccProduction) -> None
    """
        member-declarator : declarator virt-specifier-seq-opt brace-or-equal-initializer-opt
                          | identifier attribute-specifier-seq virt-specifier-seq-opt COLON constant-expression
                          | virt-specifier-seq-opt COLON constant-expression
                          | identifier-opt virt-specifier-seq-opt COLON constant-expression
    """
    # pure specifier is covered in equal-initializer


def p_virt_specifier_seq(p):
    # type: (YaccProduction) -> None
    """
        virt-specifier-seq : virt-specifier
                           | virt-specifier-seq virt-specifier
    """


def p_virt_specifier_seq_opt(p):
    # type: (YaccProduction) -> None
    """
        virt-specifier-seq-opt : virt-specifier-seq
    """
    # TODO: empty


def p_virt_specifier(p):
    # type: (YaccProduction) -> None
    """
        virt-specifier : KW_OVERRIDE
                       | KW_FINAL
                       | KW_NEW
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction