"""
storage-class-specifier:
      auto     Removed in C++0x
      register
      static
      thread_local     C++0x
      extern
      mutable
"""

from ....cpp_parser import cpp98, cpp11, deprecate_cpp11
from be_typing import TYPE_CHECKING


@cpp98
def p_storage_class_specifier(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        storage-class-specifier : KW_REGISTER
                                | KW_STATIC
                                | KW_EXTERN
                                | KW_MUTABLE
    """


@cpp98
@deprecate_cpp11
def p_storage_class_specifier_auto(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        storage-class-specifier : KW_AUTO
    """


@cpp11
def p_storage_class_specifier_thread_local(parser, p):
    # type: (CppParser, YaccProduction) -> None
    """
        storage-class-specifier : KW_THREAD_LOCAL
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction
    from ....cpp_parser import CppParser