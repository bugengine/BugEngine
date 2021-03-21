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

from ...cxx_parser import cxx98, cxx11
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("member-specification : member-declaration member-specification?")
@glrp.rule("member-specification : access-specifier ':' member-specification?")
@cxx98
def p_member_specification(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("member-declaration : attribute-specifier-seq? decl-specifier-seq? member-declarator-list? ';'")
@glrp.rule("member-declaration : function-definition ';'?")
@glrp.rule("member-declaration : using-declaration")
@glrp.rule("member-declaration : template-declaration")
@cxx98
def p_member_declaration(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("member-declaration : static_assert-declaration")
@glrp.rule("member-declaration : alias-declaration")
@cxx11
def p_member_declaration_cxx11(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("member-declarator-list : member-declarator")
@glrp.rule("member-declarator-list : member-declarator-list ',' member-declarator")
@cxx98
def p_member_declarator_list(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("member-declarator : declarator virt-specifier-seq? pure-specifier?")
@glrp.rule("member-declarator : 'identifier'? attribute-specifier-seq? virt-specifier-seq? ':' constant-expression")
@cxx98
def p_member_declarator(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("member-declarator : declarator virt-specifier-seq? brace-or-equal-initializer")
@cxx11
def p_member_declarator_braced(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("pure-specifier : '=' 'integer-literal'")
@cxx98
def p_pure_specifier(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("virt-specifier-seq : virt-specifier")
@glrp.rule("virt-specifier-seq : virt-specifier-seq virt-specifier")
@cxx98
def p_virt_specifier_seq(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("virt-specifier : 'override' | 'final' | 'new'")
@cxx11
def p_virt_specifier(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser