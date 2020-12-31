"""
storage-class-specifier:
      auto     Removed in C++0x
      register
      static
      thread_local     C++0x
      extern
      mutable
"""

from be_typing import TYPE_CHECKING


def p_storage_class_specifier(p):
    # type: (YaccProduction) -> None
    """
        storage-class-specifier : KW_REGISTER
                                | KW_STATIC
                                | KW_THREAD_LOCAL
                                | KW_EXTERN
                                | KW_MUTABLE
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction