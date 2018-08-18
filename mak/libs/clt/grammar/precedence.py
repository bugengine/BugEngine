precedence = (
    ('nonassoc','IFX'),
    ('nonassoc','ELSE'),
    ('left',    'COMMA'),
    ('right',   'CONDOP', 'COLON', 'EQUALS', 'TIMESEQUAL', 'DIVEQUAL',
                'MODEQUAL', 'PLUSEQUAL', 'MINUSEQUAL', 'LSHIFTEQUAL',
                'RSHIFTEQUAL', 'ANDEQUAL', 'OREQUAL', 'XOREQUAL'),
    ('left',    'LOR'),
    ('left',    'LAND'),
    ('left',    'OR'),
    ('left',    'XOR'),
    ('left',    'AND'),
    ('left',    'EQ', 'NE'),
    ('nonassoc','LT', 'GT', 'LE', 'GE'),
    ('left',    'LSHIFT', 'RSHIFT'),
    ('left',    'PLUS', 'MINUS'),
    ('left',    'TIMES', 'DIVIDE', 'MOD'),
    ('right',   'PRE_PLUSPLUS', 'PRE_MINUSMINUS', 'UNARY_PLUS', 'UNARY_MINUS',
                'LNOT', 'NOT', 'UNARY_TIMES', 'UNARY_AND', 'SIZEOF'),
    ('left',    'PRIO1'),
    ('left',    'PRIO0'),
    ('left',    'NAME2'),
    ('left',    'NAME0'),
    ('nonassoc','SCOPE'),
    ('nonassoc','TYPEMODIFIER'),
    ('nonassoc','TEMPLATEGT'),
)
