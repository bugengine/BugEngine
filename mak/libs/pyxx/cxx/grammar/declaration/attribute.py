"""
attribute-specifier-seq:
    attribute-specifier-seq? attribute-specifier

attribute-specifier:
    [ [ attribute-using-prefix? attribute-list ] ]
    alignment-specifier

alignment-specifier:
    alignas ( type-id ...? )
    alignas ( constant-expression ...? )

attribute-using-prefix:
    using attribute-namespace :

attribute-list:
    attribute?
    attribute-list , attribute?
    attribute ...
    attribute-list , attribute ...

attribute:
    attribute-token attribute-argument-clause?

attribute-token:
    identifier
    attribute-scoped-token

attribute-scoped-token:
    attribute-namespace :: identifier

attribute-namespace:
    identifier

attribute-argument-clause:
    ( balanced-token-seq? )

balanced-token-seq:
    balanced-token
    balanced-token-seq balanced-token

balanced-token:
    ( balanced-token-seq? )
    [ balanced-token-seq? ]
    { balanced-token-seq? }
    any token other than a parenthesis, a bracket, or a brace
"""

import glrp
from ...parser import cxx98
from be_typing import TYPE_CHECKING


#@glrp.rule('attribute-specifier-seq : attribute-specifier-seq? attribute-specifier')
@glrp.rule('attribute-specifier-seq[prec:right,0] : [prec:left,0]attribute-specifier')
@glrp.rule('attribute-specifier-seq[prec:right,1] : attribute-specifier-seq [prec:left,0]attribute-specifier')
@cxx98
def attribute_specifier_seq(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('attribute-specifier : [prec:nonassoc,0][split]"[" "[" attribute-using-prefix? attribute-list "]" "]"')
@glrp.rule('attribute-specifier : alignment-specifier')
@cxx98
def attribute_specifier(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('alignment-specifier : [prec:nonassoc,1]"alignas" "(" type-id "..."? ")"')
@glrp.rule('alignment-specifier : [prec:nonassoc,1]"alignas" "(" constant-expression "..."? ")"')
@cxx98
def alignment_specifier(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('attribute-using-prefix : "using" attribute-namespace ":"')
@cxx98
def attribute_using_prefix(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


#@glrp.rule('attribute-list : attribute?')
@glrp.rule('attribute-list[split] :')
@glrp.rule('attribute-list : attribute')
@glrp.rule('attribute-list : attribute-list "," attribute?')
@glrp.rule('attribute-list : attribute "..."')
@glrp.rule('attribute-list : attribute-list "," attribute "..."')
@cxx98
def attribute_list(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('attribute : attribute-token attribute-argument-clause?')
@cxx98
def attribute(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('attribute-token[split] : "identifier"')
@glrp.rule('attribute-token : attribute-scoped-token')
@cxx98
def attribute_token(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('attribute-scoped-token : attribute-namespace "::" "identifier"')
@cxx98
def attribute_scoped_token(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('attribute-namespace : "identifier"')
@cxx98
def attribute_namespace(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('attribute-argument-clause : "(" balanced-token-seq? ")"')
@cxx98
def attribute_argument_clause(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('balanced-token-seq : balanced-token')
@glrp.rule('balanced-token-seq : balanced-token-seq balanced-token')
@cxx98
def balanced_token_seq(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


@glrp.rule('balanced-token : "(" balanced-token-seq? ")"')
@glrp.rule('balanced-token : "[" balanced-token-seq? "]"')
@glrp.rule('balanced-token : "{" balanced-token-seq? "}"')
@glrp.rule('balanced-token : "asm"')
@glrp.rule('balanced-token : "auto"')
@glrp.rule('balanced-token : "bool"')
@glrp.rule('balanced-token : "break"')
@glrp.rule('balanced-token : "case"')
@glrp.rule('balanced-token : "catch"')
@glrp.rule('balanced-token : "char"')
@glrp.rule('balanced-token : "class"')
@glrp.rule('balanced-token : "const"')
@glrp.rule('balanced-token : "const_cast"')
@glrp.rule('balanced-token : "continue"')
@glrp.rule('balanced-token : "default"')
@glrp.rule('balanced-token : "delete"')
@glrp.rule('balanced-token : "do"')
@glrp.rule('balanced-token : "double"')
@glrp.rule('balanced-token : "dynamic_cast"')
@glrp.rule('balanced-token : "else"')
@glrp.rule('balanced-token : "enum"')
@glrp.rule('balanced-token : "explicit"')
@glrp.rule('balanced-token : "extern"')
@glrp.rule('balanced-token : "false"')
@glrp.rule('balanced-token : "float"')
@glrp.rule('balanced-token : "for"')
@glrp.rule('balanced-token : "friend"')
@glrp.rule('balanced-token : "goto"')
@glrp.rule('balanced-token : "if"')
@glrp.rule('balanced-token : "inline"')
@glrp.rule('balanced-token : "int"')
@glrp.rule('balanced-token : "long"')
@glrp.rule('balanced-token : "mutable"')
@glrp.rule('balanced-token : "namespace"')
@glrp.rule('balanced-token : "new"')
@glrp.rule('balanced-token : "operator"')
@glrp.rule('balanced-token : "private"')
@glrp.rule('balanced-token : "protected"')
@glrp.rule('balanced-token : "public"')
@glrp.rule('balanced-token : "register"')
@glrp.rule('balanced-token : "reinterpret_cast"')
@glrp.rule('balanced-token : "return"')
@glrp.rule('balanced-token : "short"')
@glrp.rule('balanced-token : "signed"')
@glrp.rule('balanced-token : "sizeof"')
@glrp.rule('balanced-token : "static"')
@glrp.rule('balanced-token : "static_cast"')
@glrp.rule('balanced-token : "struct"')
@glrp.rule('balanced-token : "switch"')
@glrp.rule('balanced-token : "template"')
@glrp.rule('balanced-token : "this"')
@glrp.rule('balanced-token : "throw"')
@glrp.rule('balanced-token : "true"')
@glrp.rule('balanced-token : "try"')
@glrp.rule('balanced-token : "typedef"')
@glrp.rule('balanced-token : "typeid"')
@glrp.rule('balanced-token : "typename"')
@glrp.rule('balanced-token : "union"')
@glrp.rule('balanced-token : "unsigned"')
@glrp.rule('balanced-token : "using"')
@glrp.rule('balanced-token : "virtual"')
@glrp.rule('balanced-token : "void"')
@glrp.rule('balanced-token : "volatile"')
@glrp.rule('balanced-token : "wchar_t"')
@glrp.rule('balanced-token : "while"')
@glrp.rule('balanced-token : "alignas"')
@glrp.rule('balanced-token : "alignof"')
@glrp.rule('balanced-token : "char16_t"')
@glrp.rule('balanced-token : "char32_t"')
@glrp.rule('balanced-token : "constexpr"')
@glrp.rule('balanced-token : "decltype"')
@glrp.rule('balanced-token : "final"')
@glrp.rule('balanced-token : "noexcept"')
@glrp.rule('balanced-token : "nullptr"')
@glrp.rule('balanced-token : "override"')
@glrp.rule('balanced-token : "static_assert"')
@glrp.rule('balanced-token : "thread_local"')
@glrp.rule('balanced-token : "char8_t"')
@glrp.rule('balanced-token : "concept"')
@glrp.rule('balanced-token : "consteval"')
@glrp.rule('balanced-token : "constinit"')
@glrp.rule('balanced-token : "co_await"')
@glrp.rule('balanced-token : "co_return"')
@glrp.rule('balanced-token : "co_yield"')
@glrp.rule('balanced-token : "export"')
@glrp.rule('balanced-token : "import"')
@glrp.rule('balanced-token : "module"')
@glrp.rule('balanced-token : "requires"')
@glrp.rule('balanced-token : "atomic_cancel"')
@glrp.rule('balanced-token : "atomic_commit"')
@glrp.rule('balanced-token : "atomic_noexcept"')
@glrp.rule('balanced-token : "synchronized"')
@glrp.rule('balanced-token : "reflexpr"')
@glrp.rule('balanced-token : "+"')
@glrp.rule('balanced-token : "-"')
@glrp.rule('balanced-token : "*"')
@glrp.rule('balanced-token : "/"')
@glrp.rule('balanced-token : "%"')
@glrp.rule('balanced-token : "|"')
@glrp.rule('balanced-token : "&"')
@glrp.rule('balanced-token : "~"')
@glrp.rule('balanced-token : "^"')
@glrp.rule('balanced-token : "<<"')
@glrp.rule('balanced-token : ">>"')
@glrp.rule('balanced-token : "||"')
@glrp.rule('balanced-token : "&&"')
@glrp.rule('balanced-token : "!"')
@glrp.rule('balanced-token : "<"')
@glrp.rule('balanced-token : ">"')
@glrp.rule('balanced-token : "<="')
@glrp.rule('balanced-token : ">="')
@glrp.rule('balanced-token : "=="')
@glrp.rule('balanced-token : "!="')
@glrp.rule('balanced-token : "<=>"')
@glrp.rule('balanced-token : "="')
@glrp.rule('balanced-token : "*="')
@glrp.rule('balanced-token : "/="')
@glrp.rule('balanced-token : "%="')
@glrp.rule('balanced-token : "+="')
@glrp.rule('balanced-token : "-="')
@glrp.rule('balanced-token : "<<="')
@glrp.rule('balanced-token : ">>="')
@glrp.rule('balanced-token : "&="')
@glrp.rule('balanced-token : "|="')
@glrp.rule('balanced-token : "^="')
@glrp.rule('balanced-token : "++"')
@glrp.rule('balanced-token : "--"')
@glrp.rule('balanced-token : "->"')
@glrp.rule('balanced-token : "->*"')
@glrp.rule('balanced-token : ".*"')
@glrp.rule('balanced-token : "?"')
@glrp.rule('balanced-token : "::"')
@glrp.rule('balanced-token : ","')
@glrp.rule('balanced-token : "."')
@glrp.rule('balanced-token : ";"')
@glrp.rule('balanced-token : ":"')
@glrp.rule('balanced-token : "..."')
@glrp.rule('balanced-token : "identifier"')
@glrp.rule('balanced-token : "floating-literal"')
@glrp.rule('balanced-token : "integer-literal"')
@glrp.rule('balanced-token : "string-literal"')
@glrp.rule('balanced-token : "character-literal"')
@glrp.rule('balanced-token : "user-defined-floating-literal"')
@glrp.rule('balanced-token : "user-defined-integer-literal"')
@glrp.rule('balanced-token : "user-defined-string-literal"')
@glrp.rule('balanced-token : "user-defined-character-literal"')
@cxx98
def balanced_token(self, p):
    # type: (CxxParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ...parser import CxxParser