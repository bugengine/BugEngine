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

from be_typing import TYPE_CHECKING


def p_namespace_name(p):
    # type: (YaccProduction) -> None
    """
        namespace-name : original-namespace-name
                       | namespace-alias
    """


def p_original_namespace_name(p):
    # type: (YaccProduction) -> None
    """
        original-namespace-name : IDENTIFIER
    """


def p_namespace_definition(p):
    # type: (YaccProduction) -> None
    """
        namespace-definition : named-namespace-definition
                             | unnamed-namespace-definition
    """


def p_named_namespace_definition(p):
    # type: (YaccProduction) -> None
    """
        named-namespace-definition : original-namespace-definition
                                   | extension-namespace-definition
    """


def p_original_namespace_definition(p):
    # type: (YaccProduction) -> None
    """
        original-namespace-definition : inline-opt KW_NAMESPACE identifier LBRACE namespace-body RBRACE
    """


def p_extension_namespace_definition(p):
    # type: (YaccProduction) -> None
    """
        extension-namespace-definition : inline-opt KW_NAMESPACE original-namespace-name LBRACE namespace-body RBRACE
    """


def p_unnanmed_namespace_definition(p):
    # type: (YaccProduction) -> None
    """
        unnamed-namespace-definition : inline-opt KW_NAMESPACE LBRACE namespace-body RBRACE
    """


def p_namespace_body(p):
    # type: (YaccProduction) -> None
    """
        namespace-body : declaration-seq-opt
    """


def p_inline_opt(p):
    # type: (YaccProduction) -> None
    """
        inline-opt : KW_INLINE
                   | empty
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction