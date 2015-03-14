from cpp.ply import lex
from cpp.tokens import tokens, reserved


states = (
    ('MACRO', 'exclusive'),
    ('DOXYGEN', 'exclusive'),
)


# Completely ignored characters
t_ANY_ignore           = ' \t\x0c\r'

def t_NEWLINE(t):
    r'\n+'
    t.lexer.lineno += len(t.value)

# Constants
integer_suffix_opt = r'(([uU][lL])|([lL][uU])|[uU]|[lL])?'
t_DECIMAL = '(0'+integer_suffix_opt+')|([1-9][0-9]*'+integer_suffix_opt+')'
t_OCTAL = '0[0-7]*'+integer_suffix_opt
t_HEX = '0[xX][0-9a-fA-F]+'+integer_suffix_opt

simple_escape = r"""([a-zA-Z\\?'"])"""
octal_escape = r"""([0-7]{1,3})"""
hex_escape = r"""(x[0-9a-fA-F]+)"""

escape_sequence = r"""(\\("""+simple_escape+'|'+octal_escape+'|'+hex_escape+'))'
cconst_char = r"""([^'\\\n]|"""+escape_sequence+')'
t_CHARACTER = "'"+cconst_char+"'"
t_WIDE_CHARACTER = 'L'+t_CHARACTER

# String literals (K&R2: A.2.6)
string_char = r"""([^"\\\n]|"""+escape_sequence+')'
t_STRING  = '"'+string_char+'*"'
t_WIDE_STRING = 'L'+t_STRING

# Floating constants (K&R2: A.2.5.3)
exponent_part = r"""([eE][-+]?[0-9]+)"""
fractional_constant = r"""([0-9]*\.[0-9]+)|([0-9]+\.)"""
t_FLOAT = '(((('+fractional_constant+')'+exponent_part+'?)|([0-9]+'+exponent_part+'))[FfLl]?)'

# ID
def t_ID(t):
    r'[a-zA-Z_\$][a-zA-Z_0-9\$]*[ \t]*'
    t.value = t.value.strip()
    try:
        remove_paren = t.lexer.macros[t.value]
        if remove_paren and t.lexer.lexdata[t.lexer.lexpos] == '(':
            t.lexer.begin('MACRO')
    except KeyError:
        t.type = reserved.get(t.value, 'ID')
        return t


# Macro state
def t_MACRO_open(t):
    r'\('
    t.lexer.inside += 1

def t_MACRO_close(t):
    r'\)'
    t.lexer.inside -= 1
    if t.lexer.inside == 0:
        t.lexer.begin('INITIAL')

def t_MACRO_skip(t):
    r'[^\(\)\n]'

def t_MACRO_NEWLINE(t):
    r'\n+'
    t.lexer.lineno += len(t.value)

# Doxygen Comments
def t_doxygen_line_left(t):
    r'//[/!]<'
    t.lexer.begin('DOXYGEN')
    t.lexer.doxyline=1
    t.lexer.doxyleft = 1
    t.type = "DOXYGEN_BEGIN_LEFT"
    return t

def t_doxygen_line(t):
    r'//[/!]'
    t.lexer.begin('DOXYGEN')
    t.lexer.doxyline=1
    t.lexer.doxyleft = 0
    t.type = "DOXYGEN_BEGIN"
    return t

def t_doxygen_block_left(t):
    r'/\*[\*!]<'
    t.lexer.begin('DOXYGEN')
    t.lexer.doxyline=0
    t.lexer.doxyleft = 1
    t.type = "DOXYGEN_BEGIN_LEFT"
    return t

def t_doxygen_long(t):
    r'/\*!'
    t.lexer.begin('DOXYGEN')
    t.lexer.doxyline=0
    t.lexer.doxyleft = 0
    t.type = "DOXYGEN_BEGIN"
    return t

def t_doxygen_long_2(t):
    r'/\*\*[^\*]'
    t.lexer.begin('DOXYGEN')
    t.lexer.doxyline=0
    t.type = "DOXYGEN_BEGIN"
    return t

t_DOXYGEN_ignore = ' \t\x0c\r'

def t_DOXYGEN_list(t):
    r'\n[ \t]*(\*[ \t]*)?-\#?'
    t.type = "DOXYGEN_LIST"
    t.lexer.lineno += 1
    t.value = t.value[t.value.find('*')+1:]
    return t

def t_DOXYGEN_newline_extend(t):
    r'\n[ \t]*//[/!]<?'
    t.lexer.lineno += 1
    t.type = "DOXYGEN_NEWLINE"
    return t

def t_DOXYGEN_newline(t):
    r'\n'
    t.lexer.lineno += 1
    if t.lexer.doxyline:
        t.lexer.begin('INITIAL')
        t.type = "DOXYGEN_END"
        return t
    else:
        t.type = "DOXYGEN_NEWLINE"
        return t

def t_DOXYGEN_end(t):
    r'\*+/'
    if not t.lexer.doxyline:
        t.lexer.begin('INITIAL')
        t.type = "DOXYGEN_END"
        return t

def t_DOXYGEN_word(t):
    r'([^ \t\x0c\r\n])+'
    t.type = "DOXYGEN_WORD"
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
    if t.value.find('include') != -1:
        t.lexer.classes.write(t.value+'\n')

# Operators
t_ADD                   = r'\+'
t_SUBSTRACT             = r'-'
t_MULTIPLY              = r'\*'
t_DIVIDE                = r'/'
t_MODULO                = r'%'
t_BITWISE_OR            = r'\|'
t_BITWISE_AND           = r'&'
t_BITWISE_NOT           = r'~'
t_BITWISE_XOR           = r'\^'
t_LEFT_SHIFT            = r'<<'
t_RIGHT_SHIFT           = r'>>'
t_LOGICAL_OR            = r'\|\|'
t_LOGICAL_AND           = r'&&'
t_LOGICAL_NOT           = r'!'
t_LESS_THAN             = r'<'
t_LESS_THAN_OR_EQUAL    = r'<='
t_GREATER_THAN          = r'>'
t_GREATER_THAN_OR_EQUAL = r'>='
t_EQUAL                 = r'=='
t_NOT_EQUAL             = r'!='
t_SCOPE                 = r'::'

# Assignment operators
t_ASSIGN                = r'='
t_ADD_ASSIGN            = r'\+='
t_SUBSTRACT_ASSIGN      = r'-='
t_MULTIPLY_ASSIGN       = r'\*='
t_DIVIDE_ASSIGN         = r'/='
t_MODULO_ASSIGN         = r'%='
t_LEFT_SHIFT_ASSIGN     = r'<<='
t_RIGHT_SHIFT_ASSIGN    = r'>>='
t_BITWISE_AND_ASSIGN    = r'&='
t_BITWISE_OR_ASSIGN     = r'\|='
t_BITWISE_XOR_ASSIGN    = r'\^='

# Increment/decrement
t_INCREMENT             = r'\+\+'
t_DECREMENT             = r'--'

# ->
t_DEREFERENCE           = r'->'

# ?
t_CONDITIONAL           = r'\?'

# Delimeters
t_LEFT_PARENTHESIS      = r'\('
t_RIGHT_PARENTHESIS     = r'\)'
t_LEFT_BRACKET          = r'\['
t_RIGHT_BRACKET         = r'\]'
t_LEFT_BRACE            = r'\{'
t_RIGHT_BRACE           = r'\}'
t_COMMA                 = r','
t_PERIOD                = r'\.'
t_SEMICOLON             = r';'
t_COLON                 = r':'
t_ELLIPSIS              = r'\.\.\.'

def t_ANY_error(t):
    t.lexer.error_count += 1
    sys.stderr.write("%s:%d: Illegal character %s\n" % (t.lexer.source, t.lineno, repr(t.value[0])))
    t.lexer.skip(1)


class Lexer(lex.Lexer):
    def __init__(self):
        super(Lexer, self).__init__()
        self.macros = {
                "__declspec": True,
                "__attribute__": True,
                "CALLBACK": False,
                "WINAPI": False,
                "__cdecl": False,
                "__fastcall": False,
                "__stdcall": False,
                "PASCAL": False,
            }
        self.error_count = 0
        self.source = ''
        self.inside = 0

    def define_macro(self, name):
        name=name.strip()
        if name.endswith('()'):
            self.macros[name[:-2]] = True
        else:
            self.macros[name] = False

lex.Lexer = Lexer

lexer = lex.lex()

