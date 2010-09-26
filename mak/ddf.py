import sys
import ply.yacc as yacc
import ply.lex as lex
import os

from optparse import OptionParser

error = 0
parser = OptionParser()
parser.set_usage('ddf.py [options] file1 [file2... fileN]')
parser.add_option("-o", "--output", dest="folder", help="Places the output into <folder>", default='')
parser.add_option("--cpp", dest="cpp", help="extension used for source implementation", default='.cc')
(options, args) = parser.parse_args()


implementation = None

# Reserved words
reserved = (
		'BE_PUBLISH',
		'STRUCT', 'CLASS', 'ENUM', 'NAMESPACE', 'UNION',
		'USING', 'NEW', 'DELETE',
		'PUBLIC', 'PROTECTED', 'PRIVATE', 'FRIEND',
		'SIGNED', 'UNSIGNED',
		'INLINE','STATIC', 'CONST', 'VOLATILE', 'VIRTUAL', 'OVERRIDE', 'MUTABLE',
		'TEMPLATE', 'TYPENAME', 'OPERATOR', 'TYPEDEF', 'THROW'
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
def t_ALIGNMENT(t):
	r'BE_SET_ALIGNMENT[ \t]*\([^\)]*\)'
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

def t_error(t):
	global error
	error += 1
	print("Illegal character %s" % repr(t.value[0]))
	t.lexer.skip(1)

lexer = lex.lex()

def p_decls(t):
	"""
		decls :
		decls : decls template_opt modifier_left decl
	"""
	pass

def p_using(t):
	"""
		decl : USING NAMESPACE ID SEMI
	"""
	pass

precedence = (
	("left", "LT", "GT"),
	("left", "SCOPE"),
)
###################################
# Namespaces
def p_namespace(t):
	"""
		decl : namespace_begin decls namespace_end
		decl : namespace_anonymous skiplist_all namespace_end
		decl : NAMESPACE ID EQUALS ID SEMI
	"""
	pass

def p_namespace_begin(t):
	"""
		namespace_begin : NAMESPACE ID LBRACE
	"""
	implementation.write('namespace %s {\n' % t[2])

def p_namespace_anonymous(t):
	"""
		namespace_anonymous : NAMESPACE LBRACE
	"""
	implementation.write('namespace {\n')

def p_namespace_end(t):
	"""
		namespace_end : RBRACE
	"""
	implementation.write('}\n')

###################################
# params
def p_param_name_opt(t):
	"""
		param_name_opt :
		param_name_opt : ID
	"""
	pass

def p_param_value_opt(t):
	"""
		param_value_opt :
		param_value_opt : EQUALS skiplist_gt
	"""
	pass

def p_param(t):
	"""
		param : type param_name_opt array_opt param_value_opt
	"""
	pass

def p_params_list(t):
	"""
		params_list :
		params_list : cs_params_list
	"""
	pass
	
def p_cs_params_list(t):
	"""
		cs_params_list : param
		cs_params_list : cs_params_list COMMA param
	"""
	pass

###################################
# type

def p_type_modifier(t):
	"""
		type_modifier : MUTABLE
		type_modifier : CONST
		type_modifier : SIGNED
		type_modifier : UNSIGNED
		type_modifier : VOLATILE
		type_modifier : TIMES
		type_modifier : AND
	"""
	pass

def p_array_opt(t):
	"""
		array_opt :
		array_opt : array_opt LBRACKET RBRACKET
		array_opt : array_opt LBRACKET skiplist_all RBRACKET
	"""
	pass

def p_type(t):
	"""
		type : name
		type : type_modifier name
		type : type type_modifier
	"""
	pass

###################################
# decls
def p_typedef(t):
	"""
		decl : TYPEDEF type ID
	"""
	pass

def p_modifier_left(t):
	"""
		modifier_left :
		modifier_left : modifier_left STATIC
		modifier_left : modifier_left INLINE
		modifier_left : modifier_left VIRTUAL
		modifier_left : modifier_left OVERRIDE
	"""
	pass
	
def p_decl(t):
	"""
		decl : SEMI
		decl : type array_opt SEMI
		decl : type name array_opt SEMI
		decl : FRIEND CLASS name SEMI
		decl : FRIEND STRUCT name SEMI
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
	"""
	pass

def p_operator(t):
	"""
		operator : NEW
		operator : DELETE
		operator : PLUS
		operator : MINUS
		operator : TIMES
		operator : DIVIDE
		operator : MOD
		operator : OR
		operator : AND
		operator : NOT
		operator : XOR
		operator : LSHIFT
		operator : RSHIFT
		operator : LOR
		operator : LAND
		operator : LNOT
		operator : LE
		operator : GE
		operator : EQ
		operator : NE
		operator : EQUALS
		operator : TIMESEQUAL
		operator : DIVEQUAL
		operator : MODEQUAL
		operator : PLUSEQUAL
		operator : MINUSEQUAL
		operator : LSHIFTEQUAL
		operator : RSHIFTEQUAL
		operator : ANDEQUAL
		operator : OREQUAL
		operator : XOREQUAL
		operator : PLUSPLUS
		operator : MINUSMINUS
		operator : ARROW
		operator : CONDOP
		operator : PERIOD
		operator : COLON
		operator : ELLIPSIS
	"""
	pass

###################################
# template stuff
def p_template(t):
	"""
		template : TEMPLATE LT params_list GT
	"""
	pass

def p_typename_opt(t):
	"""
		typename_opt :
		typename_opt : TYPENAME
	"""
	pass
	
def p_template_opt(t):
	"""
		template_opt :
		template_opt : template_opt template
	"""
	pass

def p_template_params_opt(t):
	"""
		template_params_opt :
		template_params_opt : LT skiplist_comma GT
	"""
	if len(t) > 1: print t[1], t[3]
	pass

###################################
# Name
def p_name_opt(t):
	"""
		name_opt : 
		name_opt : name
	"""
	pass

def p_name(t):
	"""
		name : namelist
		name : SCOPE namelist
	"""
	pass

def p_name_item(t):
	"""
		name_item : typename_opt template_opt ID template_params_opt
	"""
	print t[3]
	pass
	
def p_name_operator(t):
	"""
		name_operator : OPERATOR operator array_opt
		name_operator : OPERATOR LT array_opt
		name_operator : OPERATOR GT array_opt
		name_operator : OPERATOR type
		name_operator : OPERATOR LPAREN RPAREN
		name_operator : OPERATOR LBRACKET RBRACKET
	"""
	pass

def p_namelist_items(t):
	"""
		namelist_items : name_item
		namelist_items : namelist_items SCOPE name_item
	"""
	pass

def p_namelist(t):
	"""
		namelist : name_operator
		namelist : namelist_items
		namelist : namelist_items SCOPE name_operator
	"""
	pass
	

###################################
# visibility
def p_visibility(t):
	"""
		visibility : PUBLIC
		visibility : PROTECTED
		visibility : PRIVATE
	"""
	print t[1]
	pass

def p_visibility_opt(t):
	"""
		visibility_opt :
		visibility_opt : visibility
	"""
	pass
	
def p_declare_visibility(t):
	"""
		decl : visibility COLON
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
# class
def p_struct_or_class(t):
	"""
		class : CLASS
		class : STRUCT
		class : UNION
	"""
	pass

def p_class(t):
	"""
		type :	class name_opt parent_opt LBRACE decls RBRACE
		type :	class name
	"""
	pass
	
def p_enum_values(t):
	"""
		enum_values :
		enum_values : enum_value
		enum_values : enum_value COMMA enum_values
	"""
	pass
	
def p_enum_value(t):
	"""
		enum_value : ID param_value_opt
	"""
	pass

def p_enum(t):
	"""
		type :	ENUM name_opt LBRACE enum_values RBRACE
	"""
	pass
	
###################################
# method
	
def p_method_modifier_right(t):
	"""
		method_modifier_right :
		method_modifier_right : method_modifier_right CONST
		method_modifier_right : method_modifier_right VOLATILE
		method_modifier_right : method_modifier_right VIRTUAL
		method_modifier_right : method_modifier_right OVERRIDE
		method_modifier_right : method_modifier_right EQUALS DECIMAL
		method_modifier_right : method_modifier_right THROW LPAREN skiplist_all RPAREN
	"""
	pass

def p_method(t):
	"""
		method : type name template_params_opt LPAREN params_list RPAREN method_modifier_right
		method : name template_params_opt LPAREN params_list RPAREN method_modifier_right
	"""
	pass

def p_initializers(t):
	"""
		initializers :
		initializers : COLON initializer_list
	"""
	pass
	
def p_initializer_list(t):
	"""
		initializer_list : initializer
		initializer_list : initializer_list COMMA initializer
	"""
	pass
	
def p_initializer(t):
	"""
		initializer : ID LPAREN skiplist_all RPAREN
	"""
	pass

def p_method_decl_or_impl(t):
	"""
		decl : method SEMI
		decl : method initializers LBRACE skiplist_all RBRACE
	"""
	pass

###################################
# skiplist
def p_keyword(t):
	"""
		keyword : STRUCT
				| CLASS
				| ENUM
				| NAMESPACE
				| UNION
				| USING
				| PUBLIC
				| PROTECTED
				| PRIVATE
				| FRIEND
				| SIGNED
				| UNSIGNED
				| INLINE
				| STATIC
				| CONST
				| VOLATILE
				| VIRTUAL
				| OVERRIDE
				| TEMPLATE
				| TYPENAME
				| OPERATOR
				| TYPEDEF
				| THROW
	"""
	pass

def p_skiplist_base(t):
	"""
		skiplist_base :
		skiplist_base : skiplist_base LBRACE skiplist_all RBRACE
		skiplist_base : skiplist_base LBRACKET skiplist_all RBRACKET
		skiplist_base : skiplist_base LPAREN skiplist_all RPAREN
		skiplist_base : skiplist_base operator
		skiplist_base : skiplist_base keyword
		skiplist_base : skiplist_base constant
		skiplist_base : skiplist_base ID
		skiplist_base : skiplist_base SCOPE
		skiplist_base : skiplist_base SEMI
	"""
	if len(t) > 3: print t[2]
	pass

def p_skiplist(t):
	"""
		skiplist : skiplist_base
		skiplist : skiplist LT skiplist_comma GT
	"""
	pass

def p_skiplist_with_gt(t):
	"""
		skiplist_gt : skiplist_base
		skiplist_gt : skiplist_gt LT skiplist_base
		skiplist_gt : skiplist_gt GT skiplist_base
	"""
	pass

def p_skiplist_with_comma(t):
	"""
		skiplist_comma : skiplist
		skiplist_comma : skiplist_comma COMMA skiplist
	"""
	pass

def p_skiplist_all(t):
	"""
		skiplist_all : skiplist_base
		skiplist_all : skiplist_all COMMA skiplist_base
		skiplist_all : skiplist_all LT skiplist_base
		skiplist_all : skiplist_all GT skiplist_base
	"""
	pass

def p_error(errtoken):
	global error
	error += 1
	if errtoken:
		if hasattr(errtoken,"lineno"): lineno = errtoken.lineno
		else: lineno = 0
		if lineno:
			sys.stderr.write("yacc: Syntax error at line %d, token=%s\n" % (lineno, errtoken.type))
		else:
			sys.stderr.write("yacc: Syntax error, token=%s" % errtoken.type)
	else:
		sys.stderr.write("yacc: Parse error in input. EOF\n")


path = os.path.abspath(os.path.split(sys.argv[0])[0])
yacc = yacc.yacc(method='LALR', debugfile=os.path.join(path, 'parser.out'), tabmodule=os.path.join(path, 'parsetab'), picklefile=sys.argv[0]+'c')
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
	else:
		sourcefile = os.path.join(base+options.cpp)
	if os.path.normpath(sourcefile) == os.path.normpath(arg):
		raise Exception("source file and target file are the same: %s" % sourcefile)
	try:
		implementation = open(sourcefile, 'w')
	except IOError,e:
		raise Exception("cannot open output file %s : %s" % (sourcefile, str(e)))
	implementation.write("#include <%s>\n" % arg)

	olderror = error
	yacc.parse(input.read(), lexer=lexer)
	implementation.close()
	if error != olderror:
		os.remove(sourcefile)
exit(error)


