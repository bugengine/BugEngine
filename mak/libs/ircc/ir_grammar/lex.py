
from ply.lex import TOKEN
from be_typing import TYPE_CHECKING

t_ignore = ' \t'

keywords = (
    # header
    'source_filename',
    'target',
    'datalayout',
    'triple',
    'attributes',

    # structs
    'type',
    'opaque',
    'addrspace',

    # COMDAT
    'comdat',
    'any',
    'exactmatch',
    'largest',
    'noduplicates',
    'samesize',

    # types
    'i1',
    'i8',
    'i16',
    'i32',
    'i64',
    'float',
    'double',
    'half',
    'void',
    'metadata',

    # values
    'null',
    'undef',

    # methods
    'declare',
    'define',

    # linkage
    'private',
    'linker_private',
    'linker_private_weak',
    'internal',
    'available_externally',
    'linkonce',
    'weak',
    'common',
    'appending',
    'extern_weak',
    'linkonce_odr',
    'weak_odr',
    'external',

    # DLL attributes
    'dllimport',
    'dllexport',

    # visibility
    'default',
    'hidden',
    'protected',

    # method tags
    'section',
    'gc',
    'prefix',
    'prologue',
    'personality',

    # calling convention
    'spir_func',
    'spir_kernel',
    'ccc',
    'fastcc',
    'coldcc',
    'webkit_jscc',
    'anyregcc',
    'preserve_mostcc',
    'preserve_allcc',
    'cxx_fast_tlscc',
    'swiftcc',
    'tailcc',
    'cfguard_checkcc',
    'cc',

    # preemption
    'dso_local',
    'dso_preemptable',

    # method attribute
    'unnamed_addr',
    'local_unnamed_addr',
    'alignstack',
    'allocsize',
    'alwaysinline',
    'builtin',
    'cold',
    'convergent',
    'inaccessiblememonly',
    'inaccessiblemem_or_argmemonly',
    'inlinehint',
    'jumptable',
    'minsize',
    'naked',
    'no-inline-line-tables',
    'no-jump-tables',
    'nobuiltin',
    'noduplicate',
    'nofree',
    'noimplicitfloat',
    'noinline',
    'nonlazybind',
    'noredzone',
    'indirect-tls-seg-refs',
    'noreturn',
    'norecurse',
    'willreturn',
    'nosync',
    'nounwind',
    'null-pointer-is-valid',
    'optforfuzzing',
    'optnone',
    'optsize',
    'patchable-function',
    'probe-stack',
    'readnone',
    'readonly',
    'stack-probe-size',
    'no-stack-arg-probe',
    'writeonly',
    'argmemonly',
    'returns_twice',
    'safestack',
    'sanitize_address',
    'sanitize_memory',
    'sanitize_memtag',
    'speculative_load_hardening',
    'speculatable',
    'ssp',
    'sspreq',
    'sspstrong',
    'strictfp',
    'denormal-fp-math',
    'denormal-fp-math-f32',
    'thunk',
    'uwtable',
    'nocf_check',
    'shadowcallstack',

    # fp
    'ieee',
    'preserve-sign',
    'positive-zero',

    # parameter attributes
    'zeroext',
    'signext',
    'inreg',
    'byval',
    'preallocated',
    'inalloca',
    'sret',
    'align',
    'noalias',
    'nocapture',
    'nest',
    'returned',
    'nonnull',
    'dereferenceable',
    'dereferenceable_or_null',
    'swiftself',
    'swifterror',
    'immarg',

    # debug metadata
    'distinct',

    # opcodes
    'ret',
    'br',
    'switch',
    'unreachable',
    'add',
    'fadd',
    'sub',
    'fsub',
    'mul',
    'fmul',
    'udiv',
    'sdiv',
    'fdiv',
    'urem',
    'srem',
    'frem',
    'shl',
    'lshr',
    'ashr',
    'and',
    'or',
    'xor',
    'extractelement',
    'insertelement',
    'shufflevector',
    'extractvalue',
    'insertvalue',
    'alloca',
    'load',
    'store',
    'getelementptr',
    'trunc',
    'zext',
    'sext',
    'fptrunc',
    'fpext',
    'fptoui',
    'fptosi',
    'uitofp',
    'sitofp',
    'ptrtoint',
    'inttoptr',
    'bitcast',
    'addrspacecast',
    'icmp',
    'fcmp',
    'phi',
    'select',
    'call',
)

simple_escape = r"""([a-zA-Z\\?'"])"""
octal_escape = r"""([0-7]{1,3})"""
hex_escape = r"""(x[0-9a-fA-F]+)"""
escape_sequence = r"""(\\(""" + simple_escape + '|' + octal_escape + '|' + hex_escape + '))'
string_char = r"""([^"\\\n]|%s)""" % escape_sequence
t_LITERAL_STRING = '"' + string_char + '*"'
t_ID_COMDAT = '\\$[a-zA-Z\._][a-zA-Z\._0-9]*'
def t_ID_LABEL(t):
    # type: (LexToken) -> LexToken
    r'[a-zA-Z\._][a-zA-Z\._0-9]*'
    if t.lexer.ir_lexer._keywords_enabled:
        if t.value in keywords:
            t.type = t.value.upper()
    return t
t_LITERAL_DECIMAL = '[-+]?[0-9]+'
t_ATTRIBUTE_GROUP = '\\#[0-9]+'
t_METADATA_NAME = '(![-a-zA-Z\$\._][-a-zA-Z\$\._0-9]+)|(!"[^"]*")'
t_METADATA_REF = '![0-9]+'
t_METADATA_MARK = '!'
t_EQUAL = '='
t_LBRACE = '{'
t_RBRACE = '}'
t_COMMA = ','
t_COLON = ':'
t_LBRACKET = '\\['
t_RBRACKET = '\\]'
t_LPAREN = '\\('
t_RPAREN = '\\)'
t_LANGLE = '<'
t_RANGLE = '>'
t_X = 'x'
t_STAR = '\\*'
t_PIPE = '\\|'


for kw in keywords:
    globals()['t_%s' % kw.upper().replace('-', '_')] = kw
t_ID = '([%@][-a-zA-Z\$\._0-9]+)|([%@]"[^"]*")'

def t_COMMENT(t):
    # type: (LexToken) -> Optional[LexToken]
    r';[^\n]*\n+'
    t.lexer.ir_lexer._lineno += t.value.count("\n")
    return None

def t_NEWLINE(t):
    # type: (LexToken) -> Optional[LexToken]
    r'\n+'
    t.lexer.ir_lexer._lineno += t.value.count("\n")
    return None


def t_error(t):
    # type: (LexToken) -> Optional[LexToken]
    t.value = t.value[0]
    t.lexer.ir_lexer.logger.C0000(t.lexer.ir_lexer.position(t), t.value[0])
    t.lexer.skip(1)
    return None


tokens = (
    'ID',
    'ID_COMDAT',
    'ID_LABEL',
    'LITERAL_STRING',
    'LITERAL_DECIMAL',
    'ATTRIBUTE_GROUP',
    'METADATA_NAME',
    'METADATA_REF',

    'METADATA_MARK',
    'LBRACE',
    'RBRACE',
    'LPAREN',
    'LPAREN_MARK',
    'RPAREN',
    'LBRACKET',
    'RBRACKET',
    'LANGLE',
    'RANGLE',

    'EQUAL',
    'X',
    'STAR',
    'COMMA',
    'COLON',
    'PIPE'
) + tuple(
    k.upper().replace('-', '_') for k in keywords
)

if TYPE_CHECKING:
    from typing import Optional
    from ply.lex import LexToken