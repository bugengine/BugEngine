"""
token:
  	identifier
  	keyword
  	literal
  	operator-token See C++ Standard Core Language Issue n. 189
  	punctuator See C++ Standard Core Language Issue n. 189
"""

from .keyword import keywords

tokens = tuple('KW_' + kw.upper() for kw in keywords) + (
    'IDENTIFIER',
    'CLASS_IDENTIFIER',
    'NAMESPACE_IDENTIFIER',
    'NAMESPACE_ALIAS_IDENTIFIER',
    'ENUM_IDENTIFIER',
    'TEMPLATE_IDENTIFIER',
    'CLASS_TEMPLATE_IDENTIFIER',
    'TYPEDEF_IDENTIFIER',
    'FLOATING_LITERAL',
    'INTEGER_LITERAL',
    'STRING_LITERAL',
    'OP_PLUS',
    'OP_MINUS',
    'OP_TIMES',
    'OP_DIVIDE',
    'OP_MOD',
    'OP_OR',
    'OP_AND',
    'OP_NOT',
    'OP_XOR',
    'OP_LSHIFT',
    'OP_RSHIFT',
    'OP_LOR',
    'OP_LAND',
    'OP_LNOT',
    'OP_LT',
    'OP_GT',
    'OP_LE',
    'OP_GE',
    'OP_EQ',
    'OP_NE',
    'OP_EQUALS',
    'OP_TIMESEQUAL',
    'OP_DIVEQUAL',
    'OP_MODEQUAL',
    'OP_PLUSEQUAL',
    'OP_MINUSEQUAL',
    'OP_LSHIFTEQUAL',
    'OP_RSHIFTEQUAL',
    'OP_ANDEQUAL',
    'OP_OREQUAL',
    'OP_XOREQUAL',
    'OP_PLUSPLUS',
    'OP_MINUSMINUS',
    'OP_ARROW',
    'OP_COND',
    'OP_SCOPE',
    'OP_ARROWMEMBER',
    'OP_PERIODMEMBER',
    'LBRACE',
    'RBRACE',
    'LPAREN',
    'RPAREN',
    'LBRACKET',
    'RBRACKET',
    'COMMA',
    'PERIOD',
    'SEMI',
    'COLON',
    'ELLIPSIS',
)
