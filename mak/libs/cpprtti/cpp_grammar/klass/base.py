"""
base-clause:
      : base-specifier-list
base-specifier-list:
      base-specifier ...opt     C++0x
      base-specifier-list , base-specifier ...opt     C++0x
base-specifier:
      attribute-specifier-seqopt base-type-specifier     C++0x
      attribute-specifier-seqopt virtual access-specifieropt base-type-specifier     C++0x
      attribute-specifier-seqopt access-specifier virtualopt base-type-specifier     C++0x
class-or-decltype:
      ::opt nested-name-specifieropt class-name     C++0x
      decltype-specifier     C++0x
base-type-specifier:
      class-or-decltype     C++0x
access-specifier:
      private
      protected
      public
"""

from be_typing import TYPE_CHECKING


def p_base_clause(p):
    # type: (YaccProduction) -> None
    """
        base-clause : COLON base-specifier-list
    """


def p_base_clause_opt(p):
    # type: (YaccProduction) -> None
    """
        base-clause-opt : base-clause
                        | empty
    """


def p_base_specifier_list(p):
    # type: (YaccProduction) -> None
    """
        base-specifier-list : base-specifier ellipsis-opt
                            | base-specifier-list COMMA base-specifier ellipsis-opt
    """


def p_base_specifier(p):
    # type: (YaccProduction) -> None
    """
        base-specifier : attribute-specifier-seq-opt base-type-specifier
                       | attribute-specifier-seq-opt KW_VIRTUAL access-specifier-opt base-type-specifier
                       | attribute-specifier-seq access-specifier virtual-opt base-type-specifier
    """


def p_class_or_decltype(p):
    # type: (YaccProduction) -> None
    """
        class-or-decltype : scope-opt nested-name-specifier-opt type-name
                          | decltype-specifier
    """


def p_base_type_specifier(p):
    # type: (YaccProduction) -> None
    """
        base-type-specifier : class-or-decltype
    """


def p_access_specifier(p):
    # type: (YaccProduction) -> None
    """
        access-specifier : KW_PRIVATE
                         | KW_PROTECTED
                         | KW_PUBLIC
                         | KW_PUBLISHED
    """


def p_access_specifier_opt(p):
    # type: (YaccProduction) -> None
    """
        access-specifier-opt : access-specifier
                             | empty
    """


def p_virtual_opt(p):
    # type: (YaccProduction) -> None
    """
        virtual-opt : KW_VIRTUAL
                    | empty
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction