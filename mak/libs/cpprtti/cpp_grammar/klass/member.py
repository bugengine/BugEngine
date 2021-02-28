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

from ...cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_member_specification(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        member-specification : member-declaration member-specification?
                             | access-specifier COLON member-specification?
    """


@cpp98
def p_member_declaration(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        member-declaration : attribute-specifier-seq? decl-specifier-seq? member-declarator-list? SEMI
                           | function-definition
                           | function-definition SEMI
                           | using-declaration
                           | static_assert-declaration
                           | template-declaration
                           | alias-declaration
    """


@cpp98
def p_member_declarator_list(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        member-declarator-list : member-declarator
                               | member-declarator-list COMMA member-declarator
    """


@cpp98
def p_member_declarator(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        member-declarator : declarator virt-specifier-seq? brace-or-equal-initializer?
                          | IDENTIFIER? attribute-specifier-seq? virt-specifier-seq? COLON constant-expression
    """


#def p_pure_specifier(parser, p):
#    # type: (CppParser, YaccProduction) -> None
#    """
#        pure-specifier : OP_EQUALS INTEGER_LITERAL
#    """


@cpp98
def p_virt_specifier_seq(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        virt-specifier-seq : virt-specifier
                           | virt-specifier-seq virt-specifier
    """


@cpp98
def p_virt_specifier(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        virt-specifier : KW_OVERRIDE
                       | KW_FINAL
                       | KW_NEW
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser