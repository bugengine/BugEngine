"""
namespace-name:
      original-namespace-name
      namespace-alias
original-namespace-name:
      identifier
namespace-definition:
      named-namespace-definition
      unnamed-namespace-definition
named-namespace-definition:
      original-namespace-definition
      extension-namespace-definition
original-namespace-definition:
      inlineopt namespace identifier { namespace-body }     C++0x
extension-namespace-definition:
      inlineopt namespace original-namespace-name { namespace-body }     C++0x
unnamed-namespace-definition:
      inlineopt namespace { namespace-body }
namespace-body:
      declaration-seqopt
"""

from ...cpp_parser import cpp98
from be_typing import TYPE_CHECKING

#def p_namespace_name(parser, p):
#    # type: (CppParser, YaccProduction) -> None
#    """
#        namespace-name : original-namespace-name
#                       | namespace-alias
#    """

#def p_original_namespace_name(parser, p):
#    # type: (CppParser, YaccProduction) -> None
#    """
#        original-namespace-name : IDENTIFIER
#    """


@cpp98
def p_namespace_definition(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        namespace-definition : named-namespace-definition
                             | unnamed-namespace-definition
    """


#def p_named_namespace_definition(parser, p):
#    # type: (CppParser, YaccProduction) -> None
#    """
#        named-namespace-definition : original-namespace-definition
#                                   | extension-namespace-definition
#    """


@cpp98
def p_original_namespace_definition(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        named-namespace-definition : attribute-specifier-seq? decl-specifier-seq? KW_NAMESPACE IDENTIFIER LBRACE namespace-body RBRACE
    """
    # TODO: no attributes
    # TODO: decl specifier INLINE only
    print('namespace %s' % p[3])


#def p_extension_namespace_definition(parser, p):
#    # type: (CppParser, YaccProduction) -> None
#    """
#        extension-namespace-definition : KW_INLINE? KW_NAMESPACE original-namespace-name LBRACE namespace-body RBRACE
#    """


@cpp98
def p_unnanmed_namespace_definition(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        unnamed-namespace-definition : attribute-specifier-seq? decl-specifier-seq? KW_NAMESPACE LBRACE namespace-body RBRACE
    """
    # TODO: no attributes
    # TODO: decl specifier INLINE only


@cpp98
def p_namespace_body(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        namespace-body : declaration-seq?
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ...cpp_parser import CppParser