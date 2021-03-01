"""
https://en.cppreference.com/w/cpp/keyword
"""
from be_typing import TYPE_CHECKING

keywords_bugengine = (
    'u8',
    'u16',
    'u32',
    'u64',
    'i8',
    'i16',
    'i32',
    'i64',
    'published',
    'final',
    'override',
)                          # tupe: Tuple[str,...]

keywords_98 = (
                           #'and',
                           #'and_eq',
    'asm',
    'auto',
                           #'bitand',
                           #'bitor',
    'bool',
    'break',
    'case',
    'catch',
    'char',
    'class',
                           #'compl',
    'const',
    'const_cast',
    'continue',
    'default',
    'delete',
    'do',
    'double',
    'dynamic_cast',
    'else',
    'enum',
    'explicit',
    'extern',
    'false',
    'float',
    'for',
    'friend',
    'goto',
    'if',
    'inline',
    'int',
    'long',
    'mutable',
    'namespace',
    'new',
                           #'not',
                           #'not_eq',
    'operator',
                           #'or',
                           #'or_eq',
    'private',
    'protected',
    'public',
    'register',
    'reinterpret_cast',
    'return',
    'short',
    'signed',
    'sizeof',
    'static',
    'static_cast',
    'struct',
    'switch',
    'template',
    'this',
    'throw',
    'true',
    'try',
    'typedef',
    'typeid',
    'typename',
    'union',
    'unsigned',
    'using',
    'virtual',
    'void',
    'volatile',
    'wchar_t',
    'while',
                           #'xor',
                           #'xor_eq',
) + keywords_bugengine     # tupe: Tuple[str,...]

keywords_03 = tuple()  # type: Tuple[str, ...]

keywords_11 = (
    'alignas',
    'alignof',
    'char16_t',
    'char32_t',
    'constexpr',
    'decltype',
    'noexcept',
    'nullptr',
    'static_assert',
    'thread_local',
)                      # tupe: Tuple[str,...]

keywords_14 = tuple()  # type: Tuple[str, ...]

keywords_17 = tuple()  # type: Tuple[str, ...]

keywords_20 = (
    'char8_t',
    'concept',
    'consteval',
    'constinit',
    'co_await',
    'co_return',
    'co_yield',
    'requires',
)                  # tupe: Tuple[str,...]

keywords_transactional = (
    'atomic_cancel',
    'atomic_commit',
    'atomic_noexcept',
    'synchronized',
)                              # tupe: Tuple[str,...]

keywords_reflection = ('reflexpr', )   # tupe: Tuple[str,...]

keywords_23 = keywords_transactional + keywords_reflection # tupe: Tuple[str,...]

if TYPE_CHECKING:
    from typing import Tuple