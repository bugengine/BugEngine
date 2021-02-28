"""
storage-class-specifier:
      auto     Removed in C++0x
      register
      static
      thread_local     C++0x
      extern
      mutable
"""

from ....cpp_parser import cpp98
from be_typing import TYPE_CHECKING


@cpp98
def p_storage_class_specifier(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        storage-class-specifier : KW_REGISTER
                                | KW_STATIC
                                | KW_THREAD_LOCAL
                                | KW_EXTERN
                                | KW_MUTABLE
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ....cpp_parser import CppParser