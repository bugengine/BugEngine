import sys
try:
	import mak.cpp.lexer as lexer
except:
	import lexer

tokens = lexer.tokens

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
	t.parser.namespace = t.parser.rtti.Namespace(t.parser.namespace, t[2], t.lineno(1))

def p_namespace_anonymous(t):
	"""
		namespace_anonymous : NAMESPACE LBRACE
	"""
	t.parser.namespace = t.parser.rtti.Namespace(t.parser.namespace, "", t.lineno(1))

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
	"""
	t[0] = '?'

def p_param_name_opt_2(t):
	"""
		param_name_opt : ID
	"""
	t[0] = t[1]

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
	t[0] = (t[1][1]+t[3], t[2], [])

def p_param_tag(t):
	"""
		param : tags type param_name_opt array_opt param_value_opt
	"""
	t[0] = (t[2][1]+t[4], t[3], t[1])

def p_function_param(t):
	"""
		param : function_pointer_with_name param_value_opt
	"""
	t[0] = (t[1][0], t[1][1], [])

def p_function_param_tag(t):
	"""
		param : tags function_pointer_with_name param_value_opt
	"""
	t[0] = (t[2][0], t[2][1], t[1])

def p_param_ellipsis(t):
	"""
		param : ELLIPSIS
	"""
	t[0] = ('...','', [])

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

def p_type_modifier_1(t):
	"""
		type_modifier : MUTABLE
	"""
	t[0] = ""

def p_type_modifier_2(t):
	"""
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
	t[0] = ('', t[1] + " " + t[2])

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
	t[0] = ('', t[1])

def p_type_const(t):
	"""
		simple_type : type_modifier simple_type
	"""
	t[0] = (t[1]+" "+t[2][0], t[1] + " " + t[2][1])

def p_complex_type(t):
	"""
		type : simple_type type_modifier_list
	"""
	if t[2]:
		t[0] = (t[2], t[1][1] + " " + t[2])
	else:
		t[0] = t[1]

def p_complex_type_2(t):
	"""
		type : type TIMES type_modifier_list
		type : type AND type_modifier_list
	"""
	t[0] = (t[3], t[1][1] + " " + t[2] + " " + t[3])

###################################
# function pointers
def typeList(params):
	if len(params) == 1:
		if params[0][0] == 'void':
			return ""
		else:
			return params[0][0]
	result = params[0][0]
	for type,name,tags in params[1:]:
		result = result + ', ' + type
		return result

def p_function_pointer_name(t):
	"""
		function_pointer_with_name : type LPAREN ID RPAREN LPAREN params_list RPAREN
	"""
	t[0] = (t[1][1] + t[2] + t[4] + t[5] + typeList(t[6]) + t[7], t[3])

def p_function_pointer_name_2(t):
	"""
		function_pointer_with_name : type LPAREN pointer_on_member ID RPAREN LPAREN params_list RPAREN
	"""
	t[0] = (t[1][1] + t[2] + t[3] + t[5] + t[6] + typeList(t[7]) + t[8], t[4])

def p_function_pointer_no_name(t):
	"""
		function_pointer_without_name : type LPAREN RPAREN LPAREN params_list RPAREN
		function_pointer_without_name : type LPAREN pointer_on_member RPAREN LPAREN params_list RPAREN
	"""
	t[0] = t[1][1]+" "+" ".join(t[2:])

def p_function_pointer_type(t):
	"""
		type : function_pointer_without_name
	"""
	t[0] = ('', t[1])

###################################
# decls
def p_typedef(t):
	"""
		decl : TYPEDEF type ID
	"""
	t.parser.rtti.Typedef(t.parser.namespace, t[3], t.lineno(1))
def p_typedef2(t):
	"""
		decl : TYPEDEF function_pointer_with_name
	"""
	t.parser.rtti.Typedef(t.parser.namespace, t[2][1], t.lineno(1))


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
	t[0] = set([t[1]])

def p_modifier_list_2(t):
	"""
		modifier_list : modifier_list modifier_left
	"""
	t[0] = t[1]
	t[0].add(t[2])

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
	t.parser.namespace.addMember(t[1][1]+t[3], t[1][0], t[2], [], t.lineno(6))

def p_variable_decl_2(t):
	"""
		decl : modifier_list type name array_opt param_value_opt field_length_opt SEMI
	"""
	if 'static' in t[1]:
		t.parser.namespace.meta.addMember(t[1]+t[3], t[2])
	else:
		t.parser.namespace.addMember(t[2][1]+t[4], t[1][0], t[3], [], t.lineno(7))

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
		visibility : PUBLISHED
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
	t.parser.namespace.visibility = t[1]

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
		if t[2] == 'public':
			t[0] = t[3]
		else:
			t[0] = t[4]
	else:
		t[0] = ''


###################################
# tags
def p_tags(t):
	"""
		tags : tag taglist
	"""
	t[0] = [t[1]] + t[2]

def p_taglist(t):
	"""
		taglist :
	"""
	t[0] = []

def p_taglist_2(t):
	"""
		taglist : tag taglist
	"""
	t[0] = [t[1]] + t[2]

def p_tag(t):
	"""
		tag : BE_TAG LPAREN name LPAREN skiplist RPAREN RPAREN
	"""
	t[0] = (t[3],t[5])

###################################
# class
def p_struct_kw(t):
	"""
		struct : STRUCT
		struct : UNION
	"""
	pass

def p_class_kw(t):
	"""
		class : CLASS
	"""
	pass

def p_struct_header(t):
	"""
		struct_header : name_opt parent_opt LBRACE
	"""
	t.parser.namespace = t.parser.rtti.Class(t.parser.namespace, t[1], t[2], t.lineno(3), t.parser.namespace.visibility, True)
	t.parser.namespace.visibility = 'published'

def p_class_header(t):
	"""
		class_header : name_opt parent_opt LBRACE
	"""
	t.parser.namespace = t.parser.rtti.Class(t.parser.namespace, t[1], t[2], t.lineno(3), t.parser.namespace.visibility, False)
	t.parser.namespace.visibility = 'protected'

def p_class(t):
	"""
		class_def : class class_header decls RBRACE
		class_def : struct struct_header decls RBRACE
	"""

def p_class_2(t):
	"""
		decl : class_def
	"""
	t[0] = ('', t.parser.namespace.fullname)
	t.parser.namespace = t.parser.namespace.parent

def p_class_3(t):
	"""
		decl : BE_META LPAREN name RPAREN class_def
	"""
	t[0] = ('', t.parser.namespace.fullname)
	t.parser.namespace.metaclass = t[3]
	t.parser.namespace = t.parser.namespace.parent

def p_class_4(t):
	"""
		decl : tags class_def
	"""
	t[0] = ('', t.parser.namespace.fullname)
	t.parser.namespace.tags = t[1]
	t.parser.namespace = t.parser.namespace.parent

def p_class_5(t):
	"""
		decl : tags BE_META LPAREN name RPAREN class_def
	"""
	t[0] = ('', t.parser.namespace.fullname)
	t.parser.namespace.tags = t[1]
	t.parser.namespace.metaclass = t[4]
	t.parser.namespace = t.parser.namespace.parent

def p_class_decl(t):
	"""
		simple_type : class name
		simple_type : struct name
	"""
	t[0] = ('', t.parser.rtti.Typedef(t.parser.namespace, t[2], t.lineno(2)).fullname)

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
	t.parser.namespace.addEnumValue(t[1])

def p_enum_value_tag(t):
	"""
		enum_value : tags ID param_value_opt
	"""
	t.parser.namespace.addEnumValue(t[2])

def p_enum_header(t):
	"""
		enum_header :	ENUM name_opt LBRACE
	"""
	t.parser.namespace = t.parser.rtti.Enum(t.parser.namespace, t[2], t.lineno(3), t.parser.namespace.visibility)

def p_enum_header_2(t):
	"""
		enum_header :	tags ENUM name_opt LBRACE
	"""
	t.parser.namespace = t.parser.rtti.Enum(t.parser.namespace, t[3], t.lineno(4), t.parser.namespace.visibility)
	t.parser.namespace.tags = t[1]

def p_enum(t):
	"""
		decl :	enum_header enum_values RBRACE
	"""
	t[0] = ('', t.parser.namespace.fullname)
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
	t[0] = (t[2], t[6], t[1][1], t[4], t.lineno(3))

def p_constructor(t):
	"""
		method : type LPAREN RPAREN method_modifier_right
	"""
	t[4].add('static')
	t[0] = ('?ctor', t[4], 'void', [], t.lineno(2))

def p_constructor_2(t):
	"""
		method : type LPAREN params_list RPAREN method_modifier_right
	"""
	t[5].add('static')
	t[0] = ('?ctor', t[5], 'void', t[3], t.lineno(2))

def p_destructor(t):
	"""
		method : NOT ID LPAREN params_list RPAREN method_modifier_right
	"""
	t[0] = ('?dtor', t[6], 'void', t[4], t.lineno(3))

def p_destructor_2(t):
	"""
		method : namelist NOT ID LPAREN params_list RPAREN method_modifier_right
	"""
	t[0] = ('?dtor', t[7], 'void', t[5], t.lineno(4))

def p_operator_function(t):
	"""
		method : type operator_name operator LPAREN params_list RPAREN method_modifier_right
		method : type operator_name LT LPAREN params_list RPAREN method_modifier_right
		method : type operator_name GT LPAREN params_list RPAREN method_modifier_right
	"""
	t[0] = (t[3], t[7], t[1][1], t[5], t.lineno(4))

def p_operator_function_2(t):
	"""
		method : type operator_name LBRACKET RBRACKET LPAREN params_list RPAREN method_modifier_right
	"""
	t[0] = ('?index', t[8], t[1][1], t[6], t.lineno(5))

def p_operator_function_3(t):
	"""
		method : type operator_name LPAREN RPAREN LPAREN params_list RPAREN method_modifier_right
	"""
	t[0] = ('?call', t[8], t[1][1], t[6], t.lineno(5))

def p_operator_function_cast(t):
	"""
		method : operator_name type LPAREN params_list RPAREN method_modifier_right
	"""
	t[0] = (t[2][1], t[6], t[2][1], t[4], t.lineno(3))

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
	t.parser.namespace.addMethod(t[1][0], t[1][1], t[1][2], t[1][3], [], t[1][4])

def p_method_decl_or_impl_2(t):
	"""
		decl : tags method SEMI
		decl : tags method initializers LBRACE skiplist_all RBRACE
	"""
	t.parser.namespace.addMethod(t[2][0], t[2][1], t[2][2], t[2][3], t[1], t[2][4])

def p_method_decl_or_impl_3(t):
	"""
		decl : modifier_list method SEMI
	"""
	t.parser.namespace.addMethod(t[2][0], t[2][1]|t[1], t[2][2], t[2][3], [], t[2][4])

def p_method_decl_or_impl_4(t):
	"""
		decl : tags modifier_list method SEMI
	"""
	t.parser.namespace.addMethod(t[3][0], t[3][1]|t[1], t[3][2], t[3][3], t[1], t[3][4])

def p_method_decl_or_impl_5(t):
	"""
		decl : modifier_list method initializers LBRACE skiplist_all RBRACE
	"""
	t.parser.namespace.addMethod(t[2][0], t[2][1]|t[1], t[2][2], t[2][3], [], t[2][4])

def p_method_decl_or_impl_6(t):
	"""
		decl : tags modifier_list method initializers LBRACE skiplist_all RBRACE
	"""
	t.parser.namespace.addMethod(t[3][0], t[3][1]|t[1], t[3][2], t[3][3], t[1], t[3][4])

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
				| BE_TAG
				| BE_META
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

