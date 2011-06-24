
# Reserved words
reserved = (
		'PUBLISHED',
		'STRUCT', 'CLASS', 'ENUM', 'NAMESPACE', 'UNION',
		'USING', 'NEW', 'DELETE',
		'PUBLIC', 'PROTECTED', 'PRIVATE', 'FRIEND',
		'SIGNED', 'UNSIGNED', 'SHORT', 'CHAR', 'LONG', 'INT', 'FLOAT', 'DOUBLE',
		'EXPLICIT', 'INLINE', 'EXTERN', 'STATIC', 'CONST', 'VOLATILE', 'VIRTUAL', 'OVERRIDE', 'MUTABLE',
		'TEMPLATE', 'TYPENAME', 'OPERATOR', 'TYPEDEF', 'THROW',
		'BE_TAG', 'BE_META'
	)

tokens = reserved + (
	'ID', 'CHARCONST', 'WCHAR', 'STRING', 'WSTRING', 'FLOATING', 'DECIMAL', 'OCTAL', 'HEX',
	# Operators
	'PLUS', 'MINUS', 'TIMES', 'DIVIDE', 'MOD',
	'OR', 'AND', 'NOT', 'XOR', 'LSHIFT', 'RSHIFT',
	'LOR', 'LAND', 'LNOT',
	'LT', 'LE', 'GT', 'GE', 'EQ', 'NE', 'SCOPE',

	# Assignment
	'EQUALS', 'TIMESEQUAL', 'DIVEQUAL', 'MODEQUAL',
	'PLUSEQUAL', 'MINUSEQUAL',
	'LSHIFTEQUAL','RSHIFTEQUAL', 'ANDEQUAL', 'XOREQUAL',
	'OREQUAL',

	# Increment/decrement
	'PLUSPLUS', 'MINUSMINUS',

	# Structure dereference (->)
	'ARROW',

	# Conditional operator (?)
	'CONDOP',

	# Delimeters
	'LPAREN', 'RPAREN',         # ( )
	'LBRACKET', 'RBRACKET',     # [ ]
	'LBRACE', 'RBRACE',         # { }
	'COMMA', 'PERIOD',          # . ,
	'SEMI', 'COLON',            # ; :

	# Ellipsis (...)
	'ELLIPSIS',
)

# Completely ignored characters
t_ANY_ignore           = ' \t\x0c\r'

# Newlines
def t_NEWLINE(t):
	r'\n+'
	t.lexer.lineno += t.value.count("\n")

integer_suffix_opt = r'(([uU][lL])|([lL][uU])|[uU]|[lL])?'
t_DECIMAL = '(0'+integer_suffix_opt+')|([1-9][0-9]*'+integer_suffix_opt+')'
t_OCTAL = '0[0-7]*'+integer_suffix_opt
t_HEX = '0[xX][0-9a-fA-F]+'+integer_suffix_opt

simple_escape = r"""([a-zA-Z\\?'"])"""
octal_escape = r"""([0-7]{1,3})"""
hex_escape = r"""(x[0-9a-fA-F]+)"""

escape_sequence = r"""(\\("""+simple_escape+'|'+octal_escape+'|'+hex_escape+'))'
cconst_char = r"""([^'\\\n]|"""+escape_sequence+')'
t_CHARCONST = "'"+cconst_char+"'"
t_WCHAR = 'L'+t_CHARCONST

# string literals (K&R2: A.2.6)
string_char = r"""([^"\\\n]|"""+escape_sequence+')'
t_STRING  = '"'+string_char+'*"'
t_WSTRING = 'L'+t_STRING

# floating constants (K&R2: A.2.5.3)
exponent_part = r"""([eE][-+]?[0-9]+)"""
fractional_constant = r"""([0-9]*\.[0-9]+)|([0-9]+\.)"""
t_FLOATING = '(((('+fractional_constant+')'+exponent_part+'?)|([0-9]+'+exponent_part+'))[FfLl]?)'


reserved_map = { }
for r in reserved:
	reserved_map[r.lower()] = r

states = (
	('MACRO', 'exclusive'),
	('DOXYGEN', 'exclusive')
)


def t_ID(t):
	r'[a-zA-Z_\$][a-zA-Z_0-9\$]*[ \t]*'
	t.value = t.value.strip()
	try:
		remove_paren = t.lexer.macro_map[t.value]
		if remove_paren and t.lexer.lexdata[t.lexer.lexpos] == '(':
			t.lexer.begin('MACRO')
	except KeyError:
		t.type = reserved_map.get(t.value, "ID")
		return t

def t_MACRO_open(t):
	r'\('
	t.lexer.inside = t.lexer.inside + 1
	pass

def t_MACRO_close(t):
	r'\)'
	t.lexer.inside = t.lexer.inside - 1
	if t.lexer.inside == 0:
		t.lexer.begin('INITIAL')
	pass

def t_MACRO_skip(t):
	r'[^\(\)\n]'
	pass

def t_MACRO_NEWLINE(t):
	r'\n+'
	t.lexer.lineno += t.value.count("\n")

# Comments
def t_doxygen_line(t):
	r'//[/\!]'
	t.lexer.begin('DOXYGEN')
	t.lexer.doxyline=1
	
def t_doxygen_long(t):
	r'/\*\!'
	t.lexer.begin('DOXYGEN')
	t.lexer.doxyline=0

def t_doxygen_long_2(t):
	r'/\*\*[^\*]'
	t.lexer.begin('DOXYGEN')
	t.lexer.doxyline=0


def t_DOXYGEN_newline(t):
	r'\n+'
	t.lexer.lineno += t.value.count("\n")
	if t.lexer.doxyline:
		t.lexer.begin('INITIAL')


def t_comment(t):
	r'/\*(.|\n)*?\*/'
	t.lexer.lineno += t.value.count('\n')

def t_comment_2(t):
	r'\//([^\\\n]|(\\.)|(\\\n))*'
	t.lexer.lineno += t.value.count('\n')

def t_preprocessor(t):
	r'\#([^\\\n]|(\\.)|(\\\n))*'
	t.lexer.lineno += t.value.count('\n')
	pass

# Operators
t_PLUS              = r'\+'
t_MINUS             = r'-'
t_TIMES             = r'\*'
t_DIVIDE            = r'/'
t_MOD               = r'%'
t_OR                = r'\|'
t_AND               = r'&'
t_NOT               = r'~'
t_XOR               = r'\^'
t_LSHIFT            = r'<<'
t_RSHIFT            = r'>>'
t_LOR               = r'\|\|'
t_LAND              = r'&&'
t_LNOT              = r'!'
t_LT                = r'<'
t_GT                = r'>'
t_LE                = r'<='
t_GE                = r'>='
t_EQ                = r'=='
t_NE                = r'!='
t_SCOPE             = r'::'

# Assignment operators
t_EQUALS            = r'='
t_TIMESEQUAL        = r'\*='
t_DIVEQUAL          = r'/='
t_MODEQUAL          = r'%='
t_PLUSEQUAL         = r'\+='
t_MINUSEQUAL        = r'-='
t_LSHIFTEQUAL       = r'<<='
t_RSHIFTEQUAL       = r'>>='
t_ANDEQUAL          = r'&='
t_OREQUAL           = r'\|='
t_XOREQUAL          = r'\^='

# Increment/decrement
t_PLUSPLUS          = r'\+\+'
t_MINUSMINUS        = r'--'

# ->
t_ARROW             = r'->'

# ?
t_CONDOP            = r'\?'

# Delimeters
t_LPAREN            = r'\('
t_RPAREN            = r'\)'
t_LBRACKET          = r'\['
t_RBRACKET          = r'\]'
t_LBRACE            = r'\{'
t_RBRACE            = r'\}'
t_COMMA             = r','
t_PERIOD            = r'\.'
t_SEMI              = r';'
t_COLON             = r':'
t_ELLIPSIS          = r'\.\.\.'

def t_ANY_error(t):
	t.lexer.error += 1
	print("%s(%d): Illegal character %s" % (t.lexer.sourcename, t.lexer.lineno, repr(t.value[0])))
	t.lexer.skip(1)

