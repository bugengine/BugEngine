import sys
import ply.yacc
import ply.lex
import os
import rtti

from optparse import OptionParser

parser = OptionParser()
parser.set_usage('ddf.py [options] file1 [file2... fileN]')
parser.add_option("-o", "--output", dest="folder", help="Places the output into <folder>", default='')
parser.add_option("--cpp", dest="cpp", help="extension used for source implementation", default='.cc')
parser.add_option("-d", dest="macro", action="append", help="define <macro> so that it will be removed during parsing")
parser.add_option("-D", dest="macrofile", action="append", help="add the content of <macrofile> to the macros, one macro per line")


# Reserved words
reserved = (
		'BE_PUBLISH',
		'STRUCT', 'CLASS', 'ENUM', 'NAMESPACE', 'UNION',
		'USING', 'NEW', 'DELETE',
		'PUBLIC', 'PROTECTED', 'PRIVATE', 'FRIEND',
		'SIGNED', 'UNSIGNED', 'SHORT', 'CHAR', 'LONG', 'INT', 'FLOAT', 'DOUBLE',
		'EXPLICIT', 'INLINE', 'EXTERN', 'STATIC', 'CONST', 'VOLATILE', 'VIRTUAL', 'OVERRIDE', 'MUTABLE',
		'TEMPLATE', 'TYPENAME', 'OPERATOR', 'TYPEDEF', 'THROW'
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
t_ignore           = ' \t\x0c\r'

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
)

global_macro_map = {
	"__declspec": True,
	"__attribute__": True,
	"CALLBACK": False,
	"WINAPI": False,
	"__cdecl": False,
	"__fastcall": False,
	"__stdcall": False,
	"PASCAL": False,
}

def t_ID(t):
	r'~?[a-zA-Z_\$][a-zA-Z_0-9\$]*[ \t]*'
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

t_MACRO_ignore = ' \t\x0c'
def t_MACRO_skip(t):
	r'[^\(\)\n]'
	pass

def t_MACRO_NEWLINE(t):
	r'\n+'
	t.lexer.lineno += t.value.count("\n")

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
	t.lexer.error += 1
	print("%s(%d): Illegal character %s" % (t.lexer.sourcename, t.lexer.lineno, repr(t.value[0])))
	t.lexer.skip(1)

def t_MACRO_error(t):
	t.lexer.error += 1
	print("%s(%d): Illegal character %s" % (t.lexer.sourcename, t.lexer.lineno, repr(t.value[0])))
	t.lexer.skip(1)





def p_decls(t):
	"""
		decls :
		decls : decls decl
	"""
	pass

def p_decl_template(t):
	"""
		decl : template decl
	"""
	pass

def p_using(t):
	"""
		decl : USING name SEMI
		decl : USING NAMESPACE name SEMI
	"""
	pass

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
	t.parser.namespace = rtti.Namespace(t.parser.namespace, t[2], t.lineno(1))

def p_namespace_anonymous(t):
	"""
		namespace_anonymous : NAMESPACE LBRACE
	"""
	t.parser.namespace = rtti.Namespace(t.parser.namespace, "", t.lineno(1))

def p_namespace_end(t):
	"""
		namespace_end : RBRACE
	"""
	t.parser.namespace = t.parser.namespace.parent

def p_unused(t):
	"""
		decl : EXTERN STRING LBRACE decls RBRACE
	"""
	pass

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

def p_template_param_value_opt(t):
	"""
		template_param_value_opt :
		template_param_value_opt : EQUALS skiplist
	"""
	pass


def p_param(t):
	"""
		param : type param_name_opt array_opt param_value_opt
	"""
	t[0] = (t[1]+t[3], t[2])

def p_function_param(t):
	"""
		param : function_pointer_with_name param_value_opt
	"""
	t[0] = t[1]

def p_param_ellipsis(t):
	"""
		param : ELLIPSIS
	"""
	t[0] = ('...','')

def p_template_param(t):
	"""
		template_param : type param_name_opt array_opt template_param_value_opt
		template_param : template class param_name_opt array_opt template_param_value_opt
		template_param : function_pointer_with_name template_param_value_opt
		template_param : TYPENAME
		template_param : ELLIPSIS
	"""
	pass


def p_params_list_none(t):
	"""
		params_list :
	"""
	t[0] = []

def p_params_list(t):
	"""
		params_list : cs_params_list
	"""
	t[0] = t[1]

def p_template_params_list(t):
	"""
		template_params_list :
		template_params_list : cs_template_params_list
	"""
	pass

def p_cs_params_list(t):
	"""
		cs_params_list : param
	"""
	t[0] = [t[1]]

def p_cs_params_list_extend(t):
	"""
		cs_params_list : cs_params_list COMMA param
	"""
	t[0] = t[1]
	t[0].append(t[3])


def p_cs_template_params_list(t):
	"""
		cs_template_params_list : template_param
		cs_template_params_list : cs_template_params_list COMMA template_param
	"""
	pass

###################################
# type

def p_type_modifier(t):
	"""
		type_modifier : MUTABLE
		type_modifier : CONST
		type_modifier : VOLATILE
	"""
	t[0] = t[1]

def p_type_modifier_list(t):
	"""
		type_modifier_list :
		type_modifier_list : type_modifier_list type_modifier
	"""
	if len(t) > 1:
		t[0] = t[1] + " " + t[2]
	else:
		t[0] = ''

def p_integer_type(t):
	"""
		simple_type : integer_type_list integer_type
	"""
	t[0] = t[1] + " " + t[2]

def p_integer_type_item(t):
	"""
		integer_type : LONG
		integer_type : UNSIGNED
		integer_type : SIGNED
		integer_type : CHAR
		integer_type : SHORT
		integer_type : INT
		integer_type : FLOAT
		integer_type : DOUBLE
	"""
	t[0] = t[1]

def p_integer_type_list(t):
	"""
		integer_type_list :
		integer_type_list : integer_type_list integer_type
	"""
	if len(t) > 1:
		t[0] = t[1] + " " + t[2]
	else:
		t[0] = ''

def p_array_opt(t):
	"""
		array_opt :
	"""
	t[0] = ""

def p_array_opt_2(t):
	"""
		array_opt : array_opt LBRACKET RBRACKET
		array_opt : array_opt LBRACKET skiplist_all RBRACKET
	"""
	t[0] = " ".join(t[1:])

def p_type_name(t):
	"""
		simple_type : name
	"""
	t[0] = t[1]

def p_type_const(t):
	"""
		simple_type : type_modifier simple_type
	"""
	t[0] = t[1] + " " + t[2]

def p_complex_type(t):
	"""
		type : simple_type type_modifier_list
	"""
	if t[2]:
		t[0] = t[1] + " " + t[2]
	else:
		t[0] = t[1]

def p_complex_type_2(t):
	"""
		type : type TIMES type_modifier_list
		type : type AND type_modifier_list
	"""
	t[0] = t[1] + " " + t[2] + " " + t[3]

###################################
# function pointers
def typeList(params):
	if len(params) == 1:
		if params[0][0] == 'void':
			return ""
		else:
			return params[0][0]
	result = params[0][0]
	for type,name in params[1:]:
		result = result + ', ' + type
		return result

def p_function_pointer_name(t):
	"""
		function_pointer_with_name : type LPAREN ID RPAREN LPAREN params_list RPAREN
	"""
	t[0] = (t[1] + t[2] + t[4] + t[5] + typeList(t[6]) + t[7], t[3])

def p_function_pointer_name_2(t):
	"""
		function_pointer_with_name : type LPAREN pointer_on_member ID RPAREN LPAREN params_list RPAREN
	"""
	t[0] = (t[1] + t[2] + t[3] + t[5] + t[6] + typeList(t[7]) + t[8], t[4])

def p_function_pointer_no_name(t):
	"""
		function_pointer_with_name : type LPAREN RPAREN LPAREN params_list RPAREN
		function_pointer_without_name : type LPAREN pointer_on_member RPAREN LPAREN params_list RPAREN
	"""
	t[0] = " ".join(t[1:])

def p_function_pointer_type(t):
	"""
		type : function_pointer_without_name
	"""
	t[0] = t[1]

###################################
# decls
def p_typedef(t):
	"""
		decl : TYPEDEF type ID
		decl : TYPEDEF function_pointer_with_name
	"""
	rtti.Typedef(t.parser.namespace, t[3], t.lineno(1))


def p_modifier_left(t):
	"""
		modifier_left : STATIC
		modifier_left : EXPLICIT
		modifier_left : INLINE
		modifier_left : VIRTUAL
		modifier_left : OVERRIDE
		modifier_left : FRIEND
		modifier_left : EXTERN
		modifier_left : EXTERN STRING
	"""
	t[0] = t[1]

def p_modifier_list(t):
	"""
		modifier_list : modifier_left
	"""
	t[0] = [t[1]]

def p_modifier_list_2(t):
	"""
		modifier_list : modifier_list modifier_left
	"""
	t[0] = t[1] + [t[2]]

def p_field_length_opt(t):
	"""
		field_length_opt :
		field_length_opt : COLON DECIMAL
	"""
	pass
def p_decl(t):
	"""
		decl : SEMI
		decl : type array_opt SEMI
		decl : modifier_list type array_opt SEMI
		decl : function_pointer_with_name SEMI
		decl : modifier_list function_pointer_with_name SEMI
	"""
def p_variable_decl(t):
	"""
		decl : type name array_opt param_value_opt field_length_opt SEMI
	"""
	t.parser.namespace.addMember(t[1]+t[3], t[2])

def p_variable_decl_2(t):
	"""
		decl : modifier_list type name array_opt param_value_opt field_length_opt SEMI
	"""
	if 'static' in t[1]:
		pass
		#t.parser.namespace.meta.addMember(t[1]+t[3], t[2])
	else:
		t.parser.namespace.addMember(t[1]+t[3], t[2])

###################################
# Value
def p_constant(t):
	"""
		constant : STRING
		constant : WSTRING
		constant : CHARCONST
		constant : WCHAR
		constant : DECIMAL
		constant : OCTAL
		constant : HEX
		constant : FLOATING
	"""
	t[0] = t[1]

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
	t[0] = t[1]

def p_operator_2(t):
	"""
		operator : NEW LBRACKET RBRACKET
		operator : DELETE LBRACKET RBRACKET
	"""
	t[0] = t[1] + t[2] + t[3]

###################################
# template stuff
def p_template(t):
	"""
		template : TEMPLATE LT template_params_list GT
	"""
	pass

###################################
# Name
def p_name_opt(t):
	"""
		name_opt :
		name_opt : define_name
	"""
	if len(t) > 1:
		t[0] = t[1]
	else:
		t[0] = ''

def p_pointer_on_member(t):
	"""
		pointer_on_member : TIMES
		pointer_on_member : namelist TIMES
		pointer_on_member : SCOPE namelist TIMES
	"""
	t[0] = " ".join(t[1:])

def p_operator_name(t):
	"""
		operator_name : OPERATOR
		operator_name : namelist OPERATOR
		operator_name : SCOPE namelist OPERATOR
	"""
	
def expandname(name, container):
	for c in container.objects:
		if name == c.name:
			return container.fullname
	if container.parent:
		return expandname(name, container.parent)
	else:
		return ''


def p_define_name(t):
	"""
		define_name : name_item
	"""
	t[0] = t[1]

def p_define_name_2(t):
	"""
		define_name : namelist name_item
	"""
	t[0] = t[1] + t[2]

def p_define_name_3(t):
	"""
		define_name : SCOPE namelist name_item
	"""
	t[0] = t[1] + t[2] + t[3]

def p_name(t):
	"""
		name : name_item
	"""
	namespace = expandname(t[1], t.parser.namespace)
	if namespace:
		t[0] =namespace+'::'+t[1]
	else:
		t[0] = t[1]

def p_name_2(t):
	"""
		name : namelist name_item
	"""
	first = t[1][:t[1].find('::')]
	namespace = expandname(first, t.parser.namespace)
	if namespace:
		t[0] = namespace + '::' + t[1] + t[2]
	else:
		t[0] = t[1] + t[2]

def p_name_3(t):
	"""
		name : SCOPE namelist name_item
	"""
	t[0] = t[1] + t[2] + t[3]

def p_name_item(t):
	"""
		name_item : ID
	"""
	t[0] = t[1]

def p_name_item_2(t):
	"""
		name_item : TYPENAME name_item
		name_item : TEMPLATE name_item
	"""
	t[0] = t[1] + " " + t[2]

def p_name_item_3(t):
	"""
		name_item : name_item LT skiplist_comma GT
	"""
	t[0] = t[1] + t[2] + " " + t[3] + " " + t[4]

def p_namelist(t):
	"""
		namelist : name_item SCOPE
		namelist : namelist name_item SCOPE
	"""
	t[0] = t[1] + t[2]
	if len(t) > 3:
		t[0] += t[3]


###################################
# visibility
def p_visibility(t):
	"""
		visibility : PUBLIC
		visibility : PROTECTED
		visibility : PRIVATE
	"""
	t[0] =t[1]

def p_visibility_opt(t):
	"""
		visibility_opt :
		visibility_opt : visibility
	"""
	if len(t) > 1:
		t[0] = t[1]
	else:
		t[0] = ''

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
		parent_opt : COLON visibility_opt name extra_parents
	"""
	if len(t) > 1:
		if t[2] == 'public':
			t[0] = t[3]
		else:
			t[0] = t[4]
	else:
		t[0] = ''

def p_extra_parents(t):
	"""
		extra_parents :
		extra_parents : COMMA visibility_opt name extra_parents
	"""
	if len(t) > 1:
		if t[2] == 'PUBLIC':
			t[0] = t[3]
		else:
			t[0] = t[4]
	else:
		t[0] = ''


###################################
# class
def p_struct_or_class(t):
	"""
		class : CLASS
		class : STRUCT
		class : UNION
	"""
	pass

def p_class_header(t):
	"""
		class_header : name_opt parent_opt LBRACE
	"""
	t.parser.namespace = rtti.Class(t.parser.namespace, t[1], t[2], t.lineno(3))

def p_class(t):
	"""
		simple_type :	class class_header decls RBRACE
	"""
	t[0] = t.parser.namespace.fullname
	t.parser.namespace = t.parser.namespace.parent

def p_class_decl(t):
	"""
		simple_type :	class name
	"""
	t[0] = rtti.Typedef(t.parser.namespace, t[2], t.lineno(2)).fullname


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

def p_enum_header(t):
	"""
		enum_header :	ENUM name_opt LBRACE
	"""
	t.parser.namespace = rtti.Enum(t.parser.namespace, t[2], t.lineno(3))
	
def p_enum(t):
	"""
		simple_type :	enum_header enum_values RBRACE
	"""
	t[0] = t.parser.namespace.fullname
	t.parser.namespace = t.parser.namespace.parent

###################################
# method

def p_method_modifier_right(t):
	"""
		method_modifier_right :
		method_modifier_right : method_modifier_right CONST
		method_modifier_right : method_modifier_right VOLATILE
		method_modifier_right : method_modifier_right VIRTUAL
		method_modifier_right : method_modifier_right OVERRIDE
	"""
	if len(t) > 1:
		t[0] = t[1]
		t[0].add(t[2])
	else:
		t[0] = set()

def p_method_pure_virtual(t):
	"""
		method_modifier_right : method_modifier_right EQUALS DECIMAL
	"""
	t[0] = t[1]
	t[0].add('PUREVIRTUAL')

def p_method_throws(t):
	"""
		method_modifier_right : method_modifier_right THROW LPAREN skiplist_all RPAREN
	"""
	t[0] = t[1]

def p_method(t):
	"""
		method : type name LPAREN params_list RPAREN method_modifier_right
	"""
	pass

def p_constructor(t):
	"""
		method : type LPAREN params_list RPAREN method_modifier_right
	"""
	pass

def p_operator_function(t):
	"""
		method : type operator_name operator LPAREN params_list RPAREN method_modifier_right
		method : type operator_name LT LPAREN params_list RPAREN method_modifier_right
		method : type operator_name GT LPAREN params_list RPAREN method_modifier_right
		method : type operator_name LBRACKET RBRACKET LPAREN params_list RPAREN method_modifier_right
		method : type operator_name LPAREN RPAREN LPAREN params_list RPAREN method_modifier_right
		method : operator_name type LPAREN params_list RPAREN method_modifier_right
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
		initializer : name LPAREN skiplist_all RPAREN
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
				| CHAR
				| SHORT
				| LONG
				| INT
				| FLOAT
				| DOUBLE
				| EXPLICIT
				| EXTERN
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
	t[0] = t[1]

def p_skiplist_base(t):
	"""
		skiplist_base :
	"""
	t[0] = ""

def p_skiplist_base_1(t):
	"""
		skiplist_base : skiplist_base LBRACE skiplist_all RBRACE
		skiplist_base : skiplist_base LBRACKET skiplist_all RBRACKET
		skiplist_base : skiplist_base LPAREN skiplist_all RPAREN
	"""
	t[0] = " ".join(t[1:])

def p_skiplist_base_2(t):
	"""
		skiplist_base : skiplist_base operator
		skiplist_base : skiplist_base keyword
		skiplist_base : skiplist_base constant
		skiplist_base : skiplist_base ID
		skiplist_base : skiplist_base SCOPE
	"""
	t[0] = " ".join(t[1:])

def p_skiplist(t):
	"""
		skiplist : skiplist_base
		skiplist : skiplist LT skiplist_comma GT skiplist
	"""
	t[0] = " ".join(t[1:])

precedence = (
	('right', 'LT', 'GT'),
	('right', 'SCOPE'),
	('left', 'TEMPLATE'),
	('right', 'LPAREN'),
)
def p_skiplist_with_gt(t):
	"""
		skiplist_gt : skiplist_base
		skiplist_gt : skiplist_gt LT skiplist_base
		skiplist_gt : skiplist_gt GT skiplist_base
		skiplist_gt : skiplist_gt LT skiplist_comma GT skiplist_base
	"""
	t[0] = " ".join(t[1:])

def p_skiplist_with_comma(t):
	"""
		skiplist_comma : skiplist
		skiplist_comma : skiplist_comma COMMA skiplist
	"""
	t[0] = " ".join(t[1:])

def p_skiplist_all(t):
	"""
		skiplist_all : skiplist_base
		skiplist_all : skiplist_all COMMA skiplist_base
		skiplist_all : skiplist_all LT skiplist_all
		skiplist_all : skiplist_all GT skiplist_all
		skiplist_all : skiplist_all SEMI skiplist_base
	"""
	t[0] = " ".join(t[1:])

def p_error(errtoken):
	if errtoken:
		errtoken.lexer.error += 1
		if hasattr(errtoken,"lineno"): lineno = errtoken.lineno
		else: lineno = 0
		if lineno:
			sys.stderr.write("%s(%d): Syntax error: unexpected token %s\n" % (errtoken.lexer.sourcename, lineno, errtoken.type))
		else:
			sys.stderr.write("%s: Syntax error, token=%s" % (errortoken.lexer.sourcename, errtoken.type))
	else:
		sys.stderr.write("Parse error in input. EOF\n")



def doParse(source, output, temppath, macro = [], macrofile = []):
	lexer = ply.lex.lex()
	lexer.inside = 0
	lexer.sourcename = source
	lexer.error = 0
	yacc = ply.yacc.yacc(method='LALR', debugfile=os.path.join(temppath, 'parser.out'), tabmodule=os.path.join(temppath, 'parsetab'), picklefile=sys.argv[0]+'c')
	yacc.namespace = rtti.Root(source)

	lexer.macro_map = dict(global_macro_map)
	if macro:
		for m in macro:
			if m.endswith('()'):
				lexer.macro_map[m[:-2].strip()] = True
			else:
				lexer.macro_map[m.strip()] = False
	if macrofile:
		for f in macrofile:
			try:
				macros = open(f, 'r')
			except IOError,e:
				raise Exception("cannot open macro file %s : %s" % (f, str(e)))
			for m in macros.readlines():
				m = m.strip()
				if m.endswith('()'):
					lexer.macro_map[m[:-2].strip()] = True
				else:
					lexer.macro_map[m.strip()] = False

	try:
		input = open(source, 'r')
	except IOError,e:
		raise Exception("cannot open input file %s : %s" % (source, str(e)))


	yacc.parse(input.read(), lexer=lexer, debug=0)
	input.close()

	if lexer.error != 0:
		return lexer.error

	try:
		implementation = open(output, 'w')
	except IOError,e:
		raise Exception("cannot open output file %s : %s" % (output, str(e)))
	yacc.namespace.dump(implementation)

	return 0


if __name__ == '__main__':
	(options, args) = parser.parse_args()
	if not args:
		parser.print_help()

	for arg in args:
		base,ext = os.path.splitext(arg)
		path,filename = os.path.split(base)
		sourcename = arg

		if options.folder:
			outputname = os.path.join(options.folder, filename+options.cpp)
		else:
			outputname = os.path.join(base+options.cpp)
		if os.path.normpath(outputname) == os.path.normpath(sourcename):
			raise Exception("source file and target file are the same: %s" % outputname)

		path = os.path.abspath(os.path.split(sys.argv[0])[0])
		if doParse(sourcename, outputname, path, options.macro, options.macrofile) > 0:
			exit(1)
	exit(0)

