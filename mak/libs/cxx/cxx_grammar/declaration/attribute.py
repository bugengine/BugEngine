"""
attribute-specifier-seq:
      attribute-specifier     C++0x
      attribute-specifier-seq attribute-specifier     C++0x
attribute-specifier:
      [ [ attribute-list ] ]     C++0x
      alignment-specifier     C++0x
alignment-specifier:
      alignas ( type-id ...opt )     C++0x
      alignas ( alignment-expression ...opt )     C++0x
attribute-list:
      attributeopt     C++0x
      attribute-list , attributeopt     C++0x
      attribute ...     C++0x
      attribute-list , attribute ...     C++0x
attribute:
      attribute-token attribute-argument-clauseopt     C++0x
attribute-token:
      identifier     C++0x
      attribute-scoped-token     C++0x
attribute-scoped-token:
      attribute-namespace :: identifier     C++0x
attribute-namespace:
      identifier     C++0x
attribute-argument-clause: 	 
      ( balanced-token-seq )     C++0x
balanced-token-seq:
      balanced-token     C++0x
      balanced-token-seq balanced-token     C++0x
balanced-token:
      ( balanced-token-seq )     C++0x
      [ balanced-token-seq ]     C++0x
      { balanced-token-seq }     C++0x
      token     C++0x - except a parenthesis, a bracket, or a brace
"""

from ...cxx_parser import cxx98, cxx11, disabled
import glrp
from be_typing import TYPE_CHECKING


@glrp.rule("attribute-specifier-seq : attribute-specifier")
@glrp.rule("attribute-specifier-seq : attribute-specifier-seq attribute-specifier")
@cxx98
def p_attribute_specifier_seq(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("attribute-specifier : '[[' '[' attribute-list ']' ']'")
@glrp.rule("attribute-specifier : alignment-specifier")
@cxx11
def p_attribute_specifier(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


# the only valid attribute specifier for C++ 98 is a documentation comment
@glrp.rule("attribute-specifier : doxycomment")
@cxx98
def p_attribute_specifier_doc(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("alignment-specifier : 'alignas' '(' type-id '...'? ')'")
@glrp.rule("alignment-specifier : 'alignas' '(' expression '...'? ')'")
@cxx11
def p_alignment_specifier(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("attribute-list : attribute?")
@glrp.rule("attribute-list : attribute-list ',' attribute?")
@glrp.rule("attribute-list : attribute '...'")
@glrp.rule("attribute-list : attribute-list ',' attribute '...'")
@cxx11
def p_attribute_list(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("attribute : attribute-token attribute-argument-clause?")
@cxx11
def p_attribute(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("attribute-token : 'identifier' | attribute-scoped-token")
@cxx11
def p_attribute_token(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("attribute-scoped-token : 'identifier' '::' 'identifier'")
@cxx11
def p_attribute_scoped_token(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("attribute-argument-clause : '(' balanced-token-seq ')'")
@cxx11
def p_attribute_argument_clause(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("balanced-token-seq : balanced-token")
@glrp.rule("balanced-token-seq : balanced-token-seq balanced-token")
@cxx11
def p_balanced_token_seq(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("balanced-token : '(' balanced-token-seq ')'")
@glrp.rule("balanced-token : '[' balanced-token-seq ']'")
@glrp.rule("balanced-token : '{' balanced-token-seq '}'")
@glrp.rule("balanced-token : token")
@cxx11
def p_balanced_token(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule("token : 'identifier'")
@glrp.rule("token : 'floating-literal'")
@glrp.rule("token : 'integer-literal'")
@glrp.rule("token : 'string-literal'")
@glrp.rule("token : '+'")
@glrp.rule("token : '-'")
@glrp.rule("token : '*'")
@glrp.rule("token : '/'")
@glrp.rule("token : '%'")
@glrp.rule("token : '|'")
@glrp.rule("token : '&'")
@glrp.rule("token : '~'")
@glrp.rule("token : '^'")
@glrp.rule("token : '<<'")
@glrp.rule("token : '>>'")
@glrp.rule("token : '||'")
@glrp.rule("token : '&&'")
@glrp.rule("token : '!'")
@glrp.rule("token : '<'")
@glrp.rule("token : '>'")
@glrp.rule("token : '<='")
@glrp.rule("token : '>='")
@glrp.rule("token : '=='")
@glrp.rule("token : '!='")
@glrp.rule("token : '='")
@glrp.rule("token : '*='")
@glrp.rule("token : '/='")
@glrp.rule("token : '%='")
@glrp.rule("token : '+='")
@glrp.rule("token : '-='")
@glrp.rule("token : '<<='")
@glrp.rule("token : '>>='")
@glrp.rule("token : '&='")
@glrp.rule("token : '|='")
@glrp.rule("token : '^='")
@glrp.rule("token : '++'")
@glrp.rule("token : '--'")
@glrp.rule("token : '->'")
@glrp.rule("token : '?'")
@glrp.rule("token : '::'")
@glrp.rule("token : '->*'")
@glrp.rule("token : '.*'")
@glrp.rule("token : ','")
@glrp.rule("token : '.'")
@glrp.rule("token : ';'")
@glrp.rule("token : ':'")
@glrp.rule("token : '...'")
@glrp.rule("token : '<['")
@glrp.rule("token : ']>'")
@glrp.rule("token : 'alignas'")
@glrp.rule("token : 'alignof'")
@glrp.rule("token : 'asm'")
@glrp.rule("token : 'auto'")
@glrp.rule("token : 'bool'")
@glrp.rule("token : 'break'")
@glrp.rule("token : 'case'")
@glrp.rule("token : 'catch'")
@glrp.rule("token : 'char'")
@glrp.rule("token : 'char16_t'")
@glrp.rule("token : 'char32_t'")
@glrp.rule("token : 'class'")
@glrp.rule("token : 'const'")
@glrp.rule("token : 'constexpr'")
@glrp.rule("token : 'const_cast'")
@glrp.rule("token : 'continue'")
@glrp.rule("token : 'decltype'")
@glrp.rule("token : 'default'")
@glrp.rule("token : 'delete'")
@glrp.rule("token : 'do'")
@glrp.rule("token : 'double'")
@glrp.rule("token : 'dynamic_cast'")
@glrp.rule("token : 'else'")
@glrp.rule("token : 'enum'")
@glrp.rule("token : 'explicit'")
@glrp.rule("token : 'extern'")
@glrp.rule("token : 'false'")
@glrp.rule("token : 'final'")
@glrp.rule("token : 'float'")
@glrp.rule("token : 'for'")
@glrp.rule("token : 'friend'")
@glrp.rule("token : 'goto'")
@glrp.rule("token : 'if'")
@glrp.rule("token : 'inline'")
@glrp.rule("token : 'int'")
@glrp.rule("token : 'long'")
@glrp.rule("token : 'mutable'")
@glrp.rule("token : 'namespace'")
@glrp.rule("token : 'new'")
@glrp.rule("token : 'noexcept'")
@glrp.rule("token : 'nullptr'")
@glrp.rule("token : 'operator'")
@glrp.rule("token : 'override'")
@glrp.rule("token : 'private'")
@glrp.rule("token : 'protected'")
@glrp.rule("token : 'public'")
@glrp.rule("token : 'register'")
@glrp.rule("token : 'reinterpret_cast'")
@glrp.rule("token : 'return'")
@glrp.rule("token : 'short'")
@glrp.rule("token : 'signed'")
@glrp.rule("token : 'sizeof'")
@glrp.rule("token : 'static'")
@glrp.rule("token : 'static_assert'")
@glrp.rule("token : 'static_cast'")
@glrp.rule("token : 'struct'")
@glrp.rule("token : 'switch'")
@glrp.rule("token : 'template'")
@glrp.rule("token : 'this'")
@glrp.rule("token : 'thread_local'")
@glrp.rule("token : 'throw'")
@glrp.rule("token : 'true'")
@glrp.rule("token : 'try'")
@glrp.rule("token : 'typedef'")
@glrp.rule("token : 'typeid'")
@glrp.rule("token : 'typename'")
@glrp.rule("token : 'union'")
@glrp.rule("token : 'unsigned'")
@glrp.rule("token : 'using'")
@glrp.rule("token : 'virtual'")
@glrp.rule("token : 'void'")
@glrp.rule("token : 'volatile'")
@glrp.rule("token : 'wchar_t'")
@glrp.rule("token : 'while'")
@glrp.rule("token : 'u8'")
@glrp.rule("token : 'u16'")
@glrp.rule("token : 'u32'")
@glrp.rule("token : 'u64'")
@glrp.rule("token : 'i8'")
@glrp.rule("token : 'i16'")
@glrp.rule("token : 'i32'")
@glrp.rule("token : 'i64'")
@glrp.rule("token : 'published'")
@cxx11
def p_token(parser, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...cxx_parser import CxxParser