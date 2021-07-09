"""
(6.7) declaration:
    declaration-specifiers init-declarator-listopt ;
    attribute-specifier-sequence declaration-specifiers init-declarator-list ;
    static_assert-declaration attribute-declaration
(6.7) declaration-specifiers:
    declaration-specifier attribute-specifier-sequenceopt
    declaration-specifier declaration-specifiers
(6.7) declaration-specifier:
    storage-class-specifier
    type-specifier-qualifier
    function-specifier
(6.7) init-declarator-list:
    init-declarator
    init-declarator-list , init-declarator
(6.7) init-declarator:
    declarator
    declarator = initializer
(6.7) attribute-declaration:
    attribute-specifier-sequence ;
(6.7.1) storage-class-specifier:
    typedef
    extern
    static
    _Thread_local
    auto
    register
(6.7.2) type-specifier:
    void
    char
    short
    int
    long
    float
    double
    signed
    unsigned
    _Bool
    _Complex
    _Decimal32
    _Decimal64
    _Decimal128
    atomic-type-specifier
    struct-or-union-specifier
    enum-specifier
    typedef-name
(6.7.2.1) struct-or-union-specifier:
    struct-or-union attribute-specifier-sequenceopt identifieropt { member-declaration-list }
    struct-or-union attribute-specifier-sequenceopt identifier
(6.7.2.1) struct-or-union:
    struct
    union
(6.7.2.1) member-declaration-list:
    member-declaration
    member-declaration-list member-declaration
(6.7.2.1) member-declaration:
    attribute-specifier-sequenceopt specifier-qualifier-list member-declarator-listopt ;
    static_assert-declaration
(6.7.2.1) specifier-qualifier-list:
    type-specifier-qualifier attribute-specifier-sequenceopt
    type-specifier-qualifier specifier-qualifier-list
(6.7.2.1) type-specifier-qualifier:
    type-specifier
    type-qualifier
    alignment-specifier
(6.7.2.1) member-declarator-list:
    member-declarator
    member-declarator-list , member-declarator
(6.7.2.1) member-declarator:
    declarator
    declaratoropt : constant-expression
(6.7.2.2) enum-specifier:
    enum attribute-specifier-sequenceopt identifieropt { enumerator-list }
    enum attribute-specifier-sequenceopt identifieropt { enumerator-list , }
    enum identifier
(6.7.2.2) enumerator-list:
    enumerator
    enumerator-list , enumerator
(6.7.2.2) enumerator:
    enumeration-constant attribute-specifier-sequenceopt
    enumeration-constant attribute-specifier-sequenceopt = constant-expression
(6.7.2.4) atomic-type-specifier:
    _Atomic ( type-name )
(6.7.3) type-qualifier:
    const
    restrict
    volatile
    _Atomic
(6.7.4) function-specifier:
    inline
    _Noreturn
(6.7.5) alignment-specifier:
    _Alignas ( type-name )
    _Alignas ( constant-expression )
(6.7.6) declarator:
    pointeropt direct-declarator
(6.7.6) direct-declarator:
    identifier attribute-specifier-sequenceopt
    ( declarator )
    array-declarator attribute-specifier-sequenceopt
    function-declarator attribute-specifier-sequenceopt
(6.7.6) array-declarator:
    direct-declarator [ type-qualifier-listopt assignment-expressionopt ]
    direct-declarator [ static type-qualifier-listopt assignment-expression ]
    direct-declarator [ type-qualifier-list static assignment-expression ]
    direct-declarator [ type-qualifier-listopt * ]
(6.7.6) function-declarator:
    direct-declarator ( parameter-type-listopt )
(6.7.6) pointer:
    * attribute-specifier-sequenceopt type-qualifier-listopt
    * attribute-specifier-sequenceopt type-qualifier-listopt pointer
(6.7.6) type-qualifier-list:
    type-qualifier
    type-qualifier-list type-qualifier
(6.7.6) parameter-type-list:
    parameter-list
    parameter-list , ...
(6.7.6) parameter-list:
    parameter-declaration
    parameter-list , parameter-declaration
(6.7.6) parameter-declaration:
    attribute-specifier-sequenceopt declaration-specifiers declarator
    attribute-specifier-sequenceopt declaration-specifiers abstract-declaratoropt
(6.7.7) type-name:
    specifier-qualifier-list abstract-declaratoropt
(6.7.7) abstract-declarator:
    pointer
    pointeropt direct-abstract-declarator
(6.7.7) direct-abstract-declarator:
    ( abstract-declarator )
    array-abstract-declarator attribute-specifier-sequenceopt
    function-abstract-declarator attribute-specifier-sequenceopt
(6.7.7) array-abstract-declarator:
    direct-abstract-declaratoropt [ type-qualifier-listopt assignment-expressionopt ]
    direct-abstract-declaratoropt [ static type-qualifier-listopt assignment-expression ]
    direct-abstract-declaratoropt [ type-qualifier-list static assignment-expression ]
    direct-abstract-declaratoropt [ * ]
(6.7.7) function-abstract-declarator:
    direct-abstract-declaratoropt ( parameter-type-listopt )
(6.7.8) typedef-name:
    identifier
(6.7.9) initializer:
    assignment-expression
    { initializer-list }
    { initializer-list , }
(6.7.9) initializer-list:
    designationopt initializer
    initializer-list , designationopt initializer
(6.7.9) designation:
    designator-list =
(6.7.9) designator-list:
    designator
    designator-list designator
(6.7.9) designator:
    [ constant-expression ]
    . identifier
(6.7.10) static_assert-declaration:
    _Static_assert ( constant-expression , string-literal ) ;
    _Static_assert ( constant-expression ) ;
(6.7.11.1) attribute-specifier-sequence:
    attribute-specifier-sequenceopt attribute-specifier
(6.7.11.1) attribute-specifier:
    [ [ attribute-list ] ]
(6.7.11.1) attribute-list:
    attributeopt
    attribute-list , attributeopt
(6.7.11.1) attribute:
    attribute-token attribute-argument-clauseopt
(6.7.11.1) attribute-token:
    standard-attribute
    attribute-prefixed-token
(6.7.11.1) standard-attribute:
    identifier
(6.7.11.1) attribute-prefixed-token:
    attribute-prefix :: identifier
(6.7.11.1) attribute-prefix:
    identifier
(6.7.11.1) attribute-argument-clause:
    ( balanced-token-sequenceopt )
(6.7.11.1) balanced-token-sequence:
    balanced-token
    balanced-token-sequence balanced-token
(6.7.11.1) balanced-token:
    ( balanced-token-sequenceopt )
    [ balanced-token-sequenceopt ]
    { balanced-token-sequenceopt }
    any token other than a parenthesis, a bracket, or a brace
"""

import glrp
from ..parser import c89
from be_typing import TYPE_CHECKING


@c89
@glrp.rule('declaration : declaration-specifiers init-declarator-list? ";"')
@glrp.rule('declaration : attribute-specifier-sequence declaration-specifiers init-declarator-list ";"')
@glrp.rule('declaration : static_assert-declaration attribute-declaration')
def declaration(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('declaration-specifiers : declaration-specifier attribute-specifier-sequence?')
@glrp.rule('declaration-specifiers : declaration-specifier declaration-specifiers')
def declaration_specifiers(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('declaration-specifier : storage-class-specifier')
@glrp.rule('declaration-specifier : type-specifier-qualifier')
@glrp.rule('declaration-specifier : function-specifier')
def declaration_specifier(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('init-declarator-list : init-declarator')
@glrp.rule('init-declarator-list : init-declarator-list "," init-declarator')
def init_declarator_list(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('init-declarator : declarator')
@glrp.rule('init-declarator : declarator "=" initializer')
def init_declarator(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('attribute-declaration : attribute-specifier-sequence ";"')
def attribute_declaration(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('storage-class-specifier : "typedef"')
@glrp.rule('storage-class-specifier : "extern"')
@glrp.rule('storage-class-specifier : "static"')
@glrp.rule('storage-class-specifier : "_Thread_local"')
@glrp.rule('storage-class-specifier : "auto"')
@glrp.rule('storage-class-specifier : "register"')
def storage_class_specifier(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('type-specifier : "void"')
@glrp.rule('type-specifier : "char"')
@glrp.rule('type-specifier : "short"')
@glrp.rule('type-specifier : "int"')
@glrp.rule('type-specifier : "long"')
@glrp.rule('type-specifier : "float"')
@glrp.rule('type-specifier : "double"')
@glrp.rule('type-specifier : "signed"')
@glrp.rule('type-specifier : "unsigned"')
@glrp.rule('type-specifier : "_Bool"')
@glrp.rule('type-specifier : "_Complex"')
@glrp.rule('type-specifier : "_Decimal32"')
@glrp.rule('type-specifier : "_Decimal64"')
@glrp.rule('type-specifier : "_Decimal128"')
@glrp.rule('type-specifier : atomic-type-specifier')
@glrp.rule('type-specifier : struct-or-union-specifier')
@glrp.rule('type-specifier : enum-specifier')
@glrp.rule('type-specifier : typedef-name')
def type_specifier(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule(
    'struct-or-union-specifier : struct-or-union attribute-specifier-sequence? "identifier"?"{" member-declaration-list "}"'
)
@glrp.rule('struct-or-union-specifier : struct-or-union attribute-specifier-sequence? "identifier"')
def struct_or_union_specifier(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('struct-or-union : "struct"')
@glrp.rule('struct-or-union : "union"')
def struct_or_union(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('member-declaration-list : member-declaration')
@glrp.rule('member-declaration-list : member-declaration-list member-declaration')
def member_declaration_list(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('member-declaration : attribute-specifier-sequence? specifier-qualifier-list member-declarator-list? ";"')
@glrp.rule('member-declaration : static_assert-declaration')
def member_declaration(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('specifier-qualifier-list : type-specifier-qualifier attribute-specifier-sequence?')
@glrp.rule('specifier-qualifier-list : type-specifier-qualifier specifier-qualifier-list')
def specifier_qualifier_list(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('type-specifier-qualifier : type-specifier')
@glrp.rule('type-specifier-qualifier : type-qualifier')
@glrp.rule('type-specifier-qualifier : alignment-specifier')
def type_specifier_qualifier(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('member-declarator-list : member-declarator')
@glrp.rule('member-declarator-list : member-declarator-list "," member-declarator')
def member_declarator_list(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('member-declarator : declarator')
@glrp.rule('member-declarator : declarator? ":" constant-expression')
def member_declarator(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('enum-specifier : "enum" attribute-specifier-sequence? "identifier"? "{" enumerator-list "}"')
@glrp.rule('enum-specifier : "enum" attribute-specifier-sequence? "identifier"? "{" enumerator-list "," "}"')
@glrp.rule('enum-specifier : "enum" "identifier"')
def enum_specifier(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('enumerator-list : enumerator')
@glrp.rule('enumerator-list : enumerator-list "," enumerator')
def enumerator_list(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('enumerator : enumeration-constant attribute-specifier-sequence?')
@glrp.rule('enumerator : enumeration-constant attribute-specifier-sequence? "=" constant-expression')
def enumerator(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('atomic-type-specifier : "_Atomic" "(" type-name ")"')
def atomic_type_specifier(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('type-qualifier : "const"')
@glrp.rule('type-qualifier : "restrict"')
@glrp.rule('type-qualifier : "volatile"')
@glrp.rule('type-qualifier : "_Atomic"')
def type_qualifier(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('function-specifier : "inline"')
@glrp.rule('function-specifier : "_Noreturn"')
def function_specifier(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('alignment-specifier : "_Alignas" "(" type-name ")"')
@glrp.rule('alignment-specifier : "_Alignas" "(" constant-expression ")"')
def alignment_specifier(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('declarator : pointer? direct-declarator')
def declarator(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('direct-declarator : identifier attribute-specifier-sequence?')
@glrp.rule('direct-declarator : "(" declarator ")"')
@glrp.rule('direct-declarator : array-declarator attribute-specifier-sequence?')
@glrp.rule('direct-declarator : function-declarator attribute-specifier-sequence?')
def direct_declarator(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('array-declarator : direct-declarator "[" type-qualifier-list? assignment-expression? "]"')
@glrp.rule('array-declarator : direct-declarator "[" static type-qualifier-list? assignment-expression "]"')
@glrp.rule('array-declarator : direct-declarator "[" type-qualifier-list static assignment-expression "]"')
@glrp.rule('array-declarator : direct-declarator "[" type-qualifier-list? "*" "]"')
def array_declarator(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('function-declarator : direct-declarator "(" parameter-type-list? ")"')
def function_declarator(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('pointer : "*" attribute-specifier-sequence? type-qualifier-list?')
@glrp.rule('pointer : "*" attribute-specifier-sequence? type-qualifier-list? pointer')
def pointer(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('type-qualifier-list : type-qualifier')
@glrp.rule('type-qualifier-list : type-qualifier-list type-qualifier')
def type_qualifier_list(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('parameter-type-list : parameter-list')
@glrp.rule('parameter-type-list : parameter-list "," "..."')
def parameter_type_list(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('parameter-list : parameter-declaration')
@glrp.rule('parameter-list : parameter-list "," parameter-declaration')
def parameter_list(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('parameter-declaration : attribute-specifier-sequence? declaration-specifiers declarator')
@glrp.rule('parameter-declaration : attribute-specifier-sequence? declaration-specifiers abstract-declarator?')
def parameter_declaration(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('type-name : specifier-qualifier-list abstract-declarator?')
def type_name(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('abstract-declarator : pointer')
@glrp.rule('abstract-declarator : pointer? direct-abstract-declarator')
def abstract_declarator(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('direct-abstract-declarator : "(" abstract-declarator ")"')
@glrp.rule('direct-abstract-declarator : array-abstract-declarator attribute-specifier-sequence?')
@glrp.rule('direct-abstract-declarator : function-abstract-declarator attribute-specifier-sequence?')
def direct_abstract_declarator(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule(
    'array-abstract-declarator : direct-abstract-declarator? "[" type-qualifier-list? assignment-expression? "]"'
)
@glrp.rule(
    'array-abstract-declarator : direct-abstract-declarator? "[" "static" type-qualifier-list? assignment-expression "]"'
)
@glrp.rule(
    'array-abstract-declarator : direct-abstract-declarator? "[" type-qualifier-list "static" assignment-expression "]"'
)
@glrp.rule('array-abstract-declarator : direct-abstract-declarator? "[" "*" "]"')
def array_abstract_declarator(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('function-abstract-declarator : direct-abstract-declarator? "(" parameter-type-list? ")"')
def function_abstract_declarator(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('typedef-name : "identifier"')
def typedef_name(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('initializer : assignment-expression')
@glrp.rule('initializer : "{" initializer-list "}"')
@glrp.rule('initializer : "{" initializer-list "," "}"')
def initializer(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('initializer-list : designation? initializer')
@glrp.rule('initializer-list : initializer-list "," designation? initializer')
def initializer_list(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('designation : designator-list "="')
def designation(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('designator-list : designator')
@glrp.rule('designator-list : designator-list designator')
def designator_list(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('designator : "[" constant-expression "]"')
@glrp.rule('designator : "." "identifier"')
def designator(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('static_assert-declaration : "_Static_assert" "(" constant-expression "," "string-literal" ")" ";"')
@glrp.rule('static_assert-declaration : "_Static_assert" "(" constant-expression ")" ";"')
def static_assert_declaration(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('attribute-specifier-sequence : attribute-specifier-sequence? attribute-specifier')
def attribute_specifier_sequence(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('attribute-specifier : "[" "[" attribute-list "]" "]"')
def attribute_specifier(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('attribute-list : attribute?')
@glrp.rule('attribute-list : attribute-list "," attribute?')
def attribute_list(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('attribute : attribute-token attribute-argument-clause?')
def attribute(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('attribute-token : standard-attribute')
@glrp.rule('attribute-token : attribute-prefixed-token')
def attribute_token(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('standard-attribute : "identifier"')
def standard_attribute(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('attribute-prefixed-token : attribute-prefix "::" "identifier"')
def attribute_prefixed_token(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('attribute-prefix : "identifier"')
def attribute_prefix(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('attribute-argument-clause : "(" balanced-token-sequence? ")"')
def attribute_argument_clause(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('balanced-token-sequence : balanced-token')
@glrp.rule('balanced-token-sequence : balanced-token-sequence balanced-token')
def balanced_token_sequence(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


@c89
@glrp.rule('balanced-token : "(" balanced-token-sequence? ")"')
@glrp.rule('balanced-token : "[" balanced-token-sequence? "]"')
@glrp.rule('balanced-token : "{" balanced-token-sequence? "}"')
@glrp.rule('balanced-token : "auto"')
@glrp.rule('balanced-token : "break"')
@glrp.rule('balanced-token : "case"')
@glrp.rule('balanced-token : "char"')
@glrp.rule('balanced-token : "const"')
@glrp.rule('balanced-token : "continue"')
@glrp.rule('balanced-token : "default"')
@glrp.rule('balanced-token : "do"')
@glrp.rule('balanced-token : "double"')
@glrp.rule('balanced-token : "else"')
@glrp.rule('balanced-token : "enum"')
@glrp.rule('balanced-token : "extern"')
@glrp.rule('balanced-token : "float"')
@glrp.rule('balanced-token : "for"')
@glrp.rule('balanced-token : "goto"')
@glrp.rule('balanced-token : "if"')
@glrp.rule('balanced-token : "inline"')
@glrp.rule('balanced-token : "int"')
@glrp.rule('balanced-token : "long"')
@glrp.rule('balanced-token : "register"')
@glrp.rule('balanced-token : "restrict"')
@glrp.rule('balanced-token : "return"')
@glrp.rule('balanced-token : "short"')
@glrp.rule('balanced-token : "signed"')
@glrp.rule('balanced-token : "sizeof"')
@glrp.rule('balanced-token : "static"')
@glrp.rule('balanced-token : "struct"')
@glrp.rule('balanced-token : "switch"')
@glrp.rule('balanced-token : "typedef"')
@glrp.rule('balanced-token : "union"')
@glrp.rule('balanced-token : "unsigned"')
@glrp.rule('balanced-token : "void"')
@glrp.rule('balanced-token : "volatile"')
@glrp.rule('balanced-token : "while"')
@glrp.rule('balanced-token : "_Alignas"')
@glrp.rule('balanced-token : "_Alignof"')
@glrp.rule('balanced-token : "_Atomic"')
@glrp.rule('balanced-token : "_Bool"')
@glrp.rule('balanced-token : "_Complex"')
@glrp.rule('balanced-token : "_Decimal128"')
@glrp.rule('balanced-token : "_Decimal32"')
@glrp.rule('balanced-token : "_Decimal64"')
@glrp.rule('balanced-token : "_Generic"')
@glrp.rule('balanced-token : "_Imaginary"')
@glrp.rule('balanced-token : "_Noreturn"')
@glrp.rule('balanced-token : "_Static_assert"')
@glrp.rule('balanced-token : "_Thread_local"')
@glrp.rule('balanced-token : "."')
@glrp.rule('balanced-token : "->"')
@glrp.rule('balanced-token : "++"')
@glrp.rule('balanced-token : "--"')
@glrp.rule('balanced-token : "&"')
@glrp.rule('balanced-token : "*"')
@glrp.rule('balanced-token : "+"')
@glrp.rule('balanced-token : "-"')
@glrp.rule('balanced-token : "~"')
@glrp.rule('balanced-token : "!"')
@glrp.rule('balanced-token : "/"')
@glrp.rule('balanced-token : "%"')
@glrp.rule('balanced-token : "<<"')
@glrp.rule('balanced-token : ">>"')
@glrp.rule('balanced-token : "<"')
@glrp.rule('balanced-token : ">"')
@glrp.rule('balanced-token : "<="')
@glrp.rule('balanced-token : ">="')
@glrp.rule('balanced-token : "=="')
@glrp.rule('balanced-token : "!="')
@glrp.rule('balanced-token : "^"')
@glrp.rule('balanced-token : "|"')
@glrp.rule('balanced-token : "&&"')
@glrp.rule('balanced-token : "||"')
@glrp.rule('balanced-token : "?"')
@glrp.rule('balanced-token : ":"')
@glrp.rule('balanced-token : "::"')
@glrp.rule('balanced-token : ";"')
@glrp.rule('balanced-token : "..."')
@glrp.rule('balanced-token : "="')
@glrp.rule('balanced-token : "*="')
@glrp.rule('balanced-token : "/="')
@glrp.rule('balanced-token : "%="')
@glrp.rule('balanced-token : "+="')
@glrp.rule('balanced-token : "-="')
@glrp.rule('balanced-token : "<<="')
@glrp.rule('balanced-token : ">>="')
@glrp.rule('balanced-token : "&="')
@glrp.rule('balanced-token : "^="')
@glrp.rule('balanced-token : "|="')
@glrp.rule('balanced-token : ","')
@glrp.rule('balanced-token : "identifier"')
@glrp.rule('balanced-token : "integer-constant"')
@glrp.rule('balanced-token : "floating-constant"')
@glrp.rule('balanced-token : "character-constant"')
@glrp.rule('balanced-token : "enumeration-constant"')
@glrp.rule('balanced-token : "string-literal"')
def balanced_token(self, p):
    # type: (CParser, glrp.Production) -> None
    pass


if TYPE_CHECKING:
    from ..parser import CParser