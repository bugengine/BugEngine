#!python
import sys
import ply.yacc as yacc
import ply.lex as lex
import os

from optparse import OptionParser

parser = OptionParser()
parser.set_usage('ddf.py [options] file1 [file2... fileN]')
parser.add_option("-o", "--output", dest="folder", help="Places the output into <folder>", default='')
parser.add_option("--cpp", dest="cpp", help="extension used for source implementation", default='.cc')
(options, args) = parser.parse_args()


implementation = None

# Reserved words
reserved = (
		'BE_PUBLISH',
		'STRUCT', 'CLASS', 'ENUM', 'NAMESPACE',
		'USING',
		'PUBLIC', 'PROTECTED', 'PRIVATE', 'FRIEND',
		'SIGNED', 'UNSIGNED',
		'INLINE','STATIC', 'CONST', 'VOLATILE', 'VIRTUAL', 'OVERRIDE',
		'TRUE', 'FALSE', 'NULL', 'NULLPTR',
		'TEMPLATE', 'TYPENAME', 'OPERATOR', 'TYPEDEF'
	)

tokens = reserved + (
	'ID', 'CHAR', 'WCHAR', 'STRING', 'WSTRING', 'FLOAT', 'DECIMAL', 'OCTAL', 'HEX',
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
t_ignore           = ' \t\x0c'

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
t_CHAR = "'"+cconst_char+"'"
t_WCHAR = 'L'+t_CHAR

# string literals (K&R2: A.2.6)
string_char = r"""([^"\\\n]|"""+escape_sequence+')'
t_STRING  = '"'+string_char+'*"'
t_WSTRING = 'L'+t_STRING

# floating constants (K&R2: A.2.5.3)
exponent_part = r"""([eE][-+]?[0-9]+)"""
fractional_constant = r"""([0-9]*\.[0-9]+)|([0-9]+\.)"""
t_FLOAT = '(((('+fractional_constant+')'+exponent_part+'?)|([0-9]+'+exponent_part+'))[FfLl]?)'


reserved_map = { }
for r in reserved:
	reserved_map[r.lower()] = r

def t_API(t):
	r'be_api[ \t]*\([^\)]*\)'
	pass

def t_ID(t):
	r'~?[a-zA-Z_\$][a-zA-Z_0-9\$]*'
	t.type = reserved_map.get(t.value,"ID")
	return t

# Comments
def t_comment(t):
	r'/\*(.|\n)*?\*/'
	t.lexer.lineno += t.value.count('\n')

def t_comment_2(t):
	r'//.*(\\\n.*)*'
	t.lexer.lineno += t.value.count('\n')

def t_preprocessor(t):
	r'\#.*'
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

def t_error(t):
	print("Illegal character %s" % repr(t.value[0]))
	t.lexer.skip(1)

lexer = lex.lex()
lexer.inside = 0

def p_decls(t):
	"""
		decls :
		decls : decls decl
	"""
	pass

def p_using(t):
	"""
		decl : USING NAMESPACE ID SEMI
	"""
	pass


###################################
# Namespaces
def p_namespace(t):
	"""
		decl : namespace_begin decls namespace_end
		decl : namespace_anonymous skiplist namespace_end
		decl : NAMESPACE ID EQUALS ID SEMI
	"""
	pass

def p_namespace_begin(t):
	"""
		namespace_begin : NAMESPACE ID LBRACE
	"""
	yacc.namespace.append(t[2])

def p_namespace_anonymous(t):
	"""
		namespace_anonymous : NAMESPACE LBRACE
	"""
	yacc.namespace.append('')

def p_namespace_end(t):
	"""
		namespace_end : RBRACE
	"""

###################################
# type
def p_type(t):
	"""
		type : name
	"""
	pass

###################################
# typedef
def p_typedef(t):
	"""
		decl : TYPEDEF type ID SEMI
	"""
	pass


###################################
# Value
def p_constant(t):
	"""
		constant : STRING
		constant : WSTRING
		constant : CHAR
		constant : WCHAR
		constant : DECIMAL
		constant : OCTAL
		constant : HEX
		constant : FLOAT
		constant : TRUE
		constant : FALSE
		constant : NULL
		constant : NULLPTR
	"""
	pass

def p_value(t):
	"""
		value : constant
	"""

precedence = (
)


###################################
# Name
def p_name(t):
	"""
		name : namelist
		name : SCOPE namelist
	"""
	pass

def p_namelist(t):
	"""
		namelist : ID
		namelist : ID SCOPE namelist
	"""
	pass

###################################
# visibility
def p_visibility_opt(t):
	"""
		visibility_opt :
		visibility_opt : PUBLIC
		visibility_opt : PROTECTED
		visibility_opt : PRIVATE
	"""
	pass

###################################
# parent
def p_parent_opt(t):
	"""
		parent_opt :
		parent_opt : COLON visibility_opt name
	"""
	pass

###################################
# skiplist
def p_skiplist(t):
	"""
		skiplist :
	"""
	pass

yacc = yacc.yacc(method='LALR')
yacc.namespace = []

if not args:
	parser.print_help()

for arg in args:
	base,ext = os.path.splitext(arg)
	path,filename = os.path.split(base)
	try:
		input = open(arg, 'r')
	except IOError,e:
		raise Exception("cannot open input file %s : %s" % (arg, str(e)))

	if options.folder:
		sourcefile = os.path.join(options.folder, filename+options.cpp)
		print sourcefile
	else:
		sourcefile = os.path.join(base+options.cpp)
	try:
		implementation = open(sourcefile, 'w')
	except IOError,e:
		raise Exception("cannot open output file %s : %s" % (sourcefile, str(e)))

	yacc.parse(input.read(), lexer=lexer)


