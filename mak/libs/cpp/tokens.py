reserved = {
    'struct': 'STRUCT',
    'class': 'CLASS',
    'enum': 'ENUM',
    'be_pod': 'BE_POD',
    'namespace': 'NAMESPACE',
    'union': 'UNION',
    'using': 'USING',
    'new': 'NEW',
    'delete': 'DELETE',
    'published': 'PUBLISHED',
    'public': 'PUBLIC',
    'protected': 'PROTECTED',
    'private': 'PRIVATE',
    'friend': 'FRIEND',
    'signed': 'SIGNED',
    'unsigned': 'UNSIGNED',
    'short': 'SHORT',
    'char': 'CHAR',
    'long': 'LONG',
    'int': 'INT',
    'void': 'VOID',
    'explicit': 'EXPLICIT',
    'inline': 'INLINE',
    'extern': 'EXTERN',
    'static': 'STATIC',
    'mutable': 'MUTABLE',
    'const': 'CONST',
    'volatile': 'VOLATILE',
    'virtual': 'VIRTUAL',
    'override': 'OVERRIDE',
    'template': 'TEMPLATE',
    'typename': 'TYPENAME',
    'operator': 'OPERATOR',
    'typedef': 'TYPEDEF',
    'throw': 'THROW',
    'be_tag': 'BE_TAG',
    'be_segment': 'BE_SEGMENT',
    'be_segments': 'BE_SEGMENTS',
    'be_stream': 'BE_STREAM',
    '__constant': '__CONSTANT',
    '__private': '__PRIVATE',
    '__local': '__LOCAL',
    '__global': '__GLOBAL',
    'restrict': 'RESTRICT',
    '__restrict': '__RESTRICT',
}

tokens = [
    'ID',
    'CHARACTER',
    'WIDE_CHARACTER',
    'STRING',
    'WIDE_STRING',
    'FLOAT',
    'DECIMAL',
    'OCTAL',
    'HEX',
    'ADD',
    'SUBSTRACT',
    'MULTIPLY',
    'DIVIDE',
    'MODULO',
    'BITWISE_OR',
    'BITWISE_AND',
    'BITWISE_NOT',
    'BITWISE_XOR',
    'LEFT_SHIFT',
    'RIGHT_SHIFT',
    'LOGICAL_OR',
    'LOGICAL_AND',
    'LOGICAL_NOT',
    'LESS_THAN',
    'LESS_THAN_OR_EQUAL',
    'GREATER_THAN',
    'GREATER_THAN_OR_EQUAL',
    'EQUAL',
    'NOT_EQUAL',
    'SCOPE',
    'ASSIGN',
    'ADD_ASSIGN',
    'SUBSTRACT_ASSIGN',
    'MULTIPLY_ASSIGN',
    'DIVIDE_ASSIGN',
    'MODULO_ASSIGN',
    'LEFT_SHIFT_ASSIGN',
    'RIGHT_SHIFT_ASSIGN',
    'BITWISE_AND_ASSIGN',
    'BITWISE_OR_ASSIGN',
    'BITWISE_XOR_ASSIGN',
    'INCREMENT',
    'DECREMENT',
    'DEREFERENCE',
    'CONDITIONAL',
    'LEFT_PARENTHESIS',
    'RIGHT_PARENTHESIS',
    'LEFT_BRACKET',
    'RIGHT_BRACKET',
    'LEFT_BRACE',
    'RIGHT_BRACE',
    'COMMA',
    'PERIOD',
    'SEMICOLON',
    'COLON',
    'ELLIPSIS',
    'DOXYGEN_BEGIN',
    'DOXYGEN_BEGIN_LEFT',
    'DOXYGEN_END',
    'DOXYGEN_NEWLINE',
    'DOXYGEN_WORD',
    'DOXYGEN_LIST',
] + list(sorted(reserved.values()))
