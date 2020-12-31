"""
operator-token: 	 
  	Look at preprocessing-op-or-punc below See C++ Standard Core Language Issue n. 189
punctuator: 	 
  	Look at preprocessing-op-or-punc below See C++ Standard Core Language Issue n. 189
preprocessing-op-or-punc: 	 
  	{
  	}
  	[
  	]
  	#
  	##
  	(
  	)
  	<:
  	:>
  	<%
  	%>
  	%:
  	%:%:
  	;
  	:
  	...
  	new
  	delete
  	?
  	::
  	.
  	.*
  	+
  	-
  	*
  	/
  	%
  	^
  	&
  	|
  	~
  	!
  	=
  	<
  	>
  	+=
  	-=
  	*=
  	/=
  	%=
  	^=
  	&=
  	|=
  	<<
  	>>
  	<<=
  	>>=
  	==
  	!=
  	<=
  	>=
  	&&
  	||
  	++
  	--
  	,
  	->*
  	->
  	and         [not in BugEngine]
  	and_eq      [not in BugEngine]
  	bitand      [not in BugEngine]
  	bitor       [not in BugEngine]
  	compl       [not in BugEngine]
  	not         [not in BugEngine]
  	not_eq      [not in BugEngine]
  	or          [not in BugEngine]
  	or_eq       [not in BugEngine]
  	xor         [not in BugEngine]
  	xor_eq      [not in BugEngine]
"""

# Operators
t_OP_PLUS = r'\+'
t_OP_MINUS = r'-'
t_OP_TIMES = r'\*'
t_OP_DIVIDE = r'/'
t_OP_MOD = r'%'
t_OP_OR = r'\|'
t_OP_AND = r'&'
t_OP_NOT = r'~'
t_OP_XOR = r'\^'
t_OP_LSHIFT = r'<<'
t_OP_RSHIFT = r'>>'
t_OP_LOR = r'\|\|'
t_OP_LAND = r'&&'
t_OP_LNOT = r'!'
t_OP_LT = r'<'
t_OP_GT = r'>'
t_OP_LE = r'<='
t_OP_GE = r'>='
t_OP_EQ = r'=='
t_OP_NE = r'!='

# Assignment operators
t_OP_EQUALS = r'='
t_OP_TIMESEQUAL = r'\*='
t_OP_DIVEQUAL = r'/='
t_OP_MODEQUAL = r'%='
t_OP_PLUSEQUAL = r'\+='
t_OP_MINUSEQUAL = r'-='
t_OP_LSHIFTEQUAL = r'<<='
t_OP_RSHIFTEQUAL = r'>>='
t_OP_ANDEQUAL = r'&='
t_OP_OREQUAL = r'\|='
t_OP_XOREQUAL = r'\^='

# Increment/decrement
t_OP_PLUSPLUS = r'\+\+'
t_OP_MINUSMINUS = r'--'

# ->, ->*, .*
t_OP_ARROW = r'->'
t_OP_ARROWMEMBER = r'->\*'
t_OP_PERIODMEMBER = r'\.\*'

# ?
t_OP_COND = r'\?'

# Scope
t_OP_SCOPE = r'::'

# Punctuation
t_LBRACE = r'{'
t_RBRACE = r'}'
t_LPAREN = r'\('
t_RPAREN = r'\)'
t_LBRACKET = r'\['
t_RBRACKET = r'\]'
t_COMMA = r','
t_PERIOD = r'\.'
t_SEMI = r';'
t_COLON = r':'
t_ELLIPSIS = r'\.\.\.'
