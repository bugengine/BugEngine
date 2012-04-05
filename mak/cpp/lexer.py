import cpp.tokens
import cpp.yacc

tokens = []
reserved_map = { }

for name,r in cpp.tokens.__dict__.items():
	if type(r) == type(cpp.tokens.keyword):
		if cpp.tokens.keyword in r.__bases__:
			tokens.append(r.__name__)
			reserved_map[r.__name__.lower()] = r
		elif cpp.yacc.Token in r.__bases__:
			tokens.append(r.__name__)


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
		k = reserved_map.get(t.value, cpp.tokens.ID)
		t.type = k.__name__
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
def t_doxygen_line_left(t):
	r'//[/!]<'
	t.lexer.begin('DOXYGEN')
	t.lexer.doxyline=1
	t.lexer.doxyleft = 1
	t.type = "DOXY_BEGIN_LEFT"
	return t

def t_doxygen_line(t):
	r'//[/!]'
	t.lexer.begin('DOXYGEN')
	t.lexer.doxyline=1
	t.lexer.doxyleft = 0
	t.type = "DOXY_BEGIN"
	return t

def t_doxygen_block_left(t):
	r'/\*[\*!]<'
	t.lexer.begin('DOXYGEN')
	t.lexer.doxyline=0
	t.lexer.doxyleft = 1
	t.type = "DOXY_BEGIN_LEFT"
	return t

def t_doxygen_long(t):
	r'/\*!'
	t.lexer.begin('DOXYGEN')
	t.lexer.doxyline=0
	t.lexer.doxyleft = 0
	t.type = "DOXY_BEGIN"
	return t

def t_doxygen_long_2(t):
	r'/\*\*[^\*]'
	t.lexer.begin('DOXYGEN')
	t.lexer.doxyline=0
	t.type = "DOXY_BLOCK"
	return t

t_DOXYGEN_ignore = ' \t\x0c\r'

def t_DOXYGEN_list(t):
	r'\n[ \t]*(\*[ \t]*)?-\#?'
	t.type = "DOXY_LIST"
	t.value = t.value[t.value.find('*')+1:]
	return t

def t_DOXYGEN_newline_extend(t):
	r'\n[ \t]*//[/!]<?'
	t.lexer.lineno += 1
	t.type = "DOXY_NEWLINE"
	return t

def t_DOXYGEN_newline(t):
	r'\n'
	t.lexer.lineno += 1
	if t.lexer.doxyline:
		t.lexer.begin('INITIAL')
		t.type = "DOXY_END"
		return t
	else:
		t.type = "DOXY_NEWLINE"
		return t

def t_DOXYGEN_end(t):
	r'\*+/'
	if not t.lexer.doxyline:
		t.lexer.begin('INITIAL')
		t.type = "DOXY_END"
		return t

def t_DOXYGEN_word(t):
	r'([^ \t\x0c\r\n])+'
	t.type = "DOXY_WORD"
	return t





def t_comment(t):
	r'/\*(.|\n)*?\*/'
	t.lexer.lineno += t.value.count('\n')

def t_comment_2(t):
	r'\//([^\\\n]|(\\.)|(\\\n))*'
	t.lexer.lineno += t.value.count('\n')

def t_preprocessor(t):
	r'\#([^\\\n]|(\\.)|(\\\n))*'
	t.lexer.lineno += t.value.count('\n')

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

