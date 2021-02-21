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

from be_typing import TYPE_CHECKING


def p_attribute_specifier_seq(p):
    # type: (YaccProduction) -> None
    """
        attribute-specifier-seq : attribute-specifier
                                | attribute-specifier-seq attribute-specifier
    """


def p_attribute_specifier(p):
    # type: (YaccProduction) -> None
    """
        attribute-specifier : LBRACKET LBRACKET attribute-list RBRACKET RBRACKET
                            | alignment-specifier
    """


def p_alignment_specifier(p):
    # type: (YaccProduction) -> None
    """
        alignment-specifier : KW_ALIGNAS LPAREN type-id ELLIPSIS? RPAREN
                            | KW_ALIGNAS LPAREN constant-expression ELLIPSIS? RPAREN
    """


def p_attribute_list(p):
    # type: (YaccProduction) -> None
    """
        attribute-list : attribute?
                       | attribute-list COMMA attribute?
                       | attribute ELLIPSIS
                       | attribute-list COMMA attribute ELLIPSIS
    """


def p_attribute(p):
    # type: (YaccProduction) -> None
    """
        attribute : attribute-token attribute-argument-clause?
    """


def p_attribute_token(p):
    # type: (YaccProduction) -> None
    """
        attribute-token : IDENTIFIER
                        | attribute-scoped-token
    """


def p_attribute_scoped_token(p):
    # type: (YaccProduction) -> None
    """
        attribute-scoped-token : attribute-namespace OP_SCOPE IDENTIFIER
    """


def p_attribute_namespace(p):
    # type: (YaccProduction) -> None
    """
        attribute-namespace : IDENTIFIER
    """


def p_attribute_argument_clause(p):
    # type: (YaccProduction) -> None
    """
        attribute-argument-clause : LPAREN balanced-token-seq RPAREN
    """


def p_balanced_token_seq(p):
    # type: (YaccProduction) -> None
    """
        balanced-token-seq : balanced-token
                           | balanced-token-seq balanced-token
    """


def p_balanced_token(p):
    # type: (YaccProduction) -> None
    """
        balanced-token : LPAREN balanced-token-seq RPAREN
                       | LBRACKET balanced-token-seq RBRACKET
                       | LBRACE balanced-token-seq RBRACE
                       | token
    """


def p_token(p):
    # type: (YaccProduction) -> None
    """
        token : IDENTIFIER
              | FLOATING_LITERAL
              | INTEGER_LITERAL
              | STRING_LITERAL
              | OP_PLUS
              | OP_MINUS
              | OP_TIMES
              | OP_DIVIDE
              | OP_MOD
              | OP_OR
              | OP_AND
              | OP_NOT
              | OP_XOR
              | OP_LSHIFT
              | OP_RSHIFT
              | OP_LOR
              | OP_LAND
              | OP_LNOT
              | OP_LT
              | OP_GT
              | OP_LE
              | OP_GE
              | OP_EQ
              | OP_NE
              | OP_EQUALS
              | OP_TIMESEQUAL
              | OP_DIVEQUAL
              | OP_MODEQUAL
              | OP_PLUSEQUAL
              | OP_MINUSEQUAL
              | OP_LSHIFTEQUAL
              | OP_RSHIFTEQUAL
              | OP_ANDEQUAL
              | OP_OREQUAL
              | OP_XOREQUAL
              | OP_PLUSPLUS
              | OP_MINUSMINUS
              | OP_ARROW
              | OP_COND
              | OP_SCOPE
              | OP_ARROWMEMBER
              | OP_PERIODMEMBER
              | COMMA
              | PERIOD
              | SEMI
              | COLON
              | ELLIPSIS
              | KW_ALIGNAS
              | KW_ALIGNOF
              | KW_ASM
              | KW_AUTO
              | KW_BOOL
              | KW_BREAK
              | KW_CASE
              | KW_CATCH
              | KW_CHAR
              | KW_CHAR16_T
              | KW_CHAR32_T
              | KW_CLASS
              | KW_CONST
              | KW_CONSTEXPR
              | KW_CONST_CAST
              | KW_CONTINUE
              | KW_DECLTYPE
              | KW_DEFAULT
              | KW_DELETE
              | KW_DO
              | KW_DOUBLE
              | KW_DYNAMIC_CAST
              | KW_ELSE
              | KW_ENUM
              | KW_EXPLICIT
              | KW_EXTERN
              | KW_FALSE
              | KW_FINAL
              | KW_FLOAT
              | KW_FOR
              | KW_FRIEND
              | KW_GOTO
              | KW_IF
              | KW_INLINE
              | KW_INT
              | KW_LONG
              | KW_MUTABLE
              | KW_NAMESPACE
              | KW_NEW
              | KW_NOEXCEPT
              | KW_NULLPTR
              | KW_OPERATOR
              | KW_OVERRIDE
              | KW_PRIVATE
              | KW_PROTECTED
              | KW_PUBLIC
              | KW_REGISTER
              | KW_REINTERPRET_CAST
              | KW_RETURN
              | KW_SHORT
              | KW_SIGNED
              | KW_SIZEOF
              | KW_STATIC
              | KW_STATIC_ASSERT
              | KW_STATIC_CAST
              | KW_STRUCT
              | KW_SWITCH
              | KW_TEMPLATE
              | KW_THIS
              | KW_THREAD_LOCAL
              | KW_THROW
              | KW_TRUE
              | KW_TRY
              | KW_TYPEDEF
              | KW_TYPEID
              | KW_TYPENAME
              | KW_UNION
              | KW_UNSIGNED
              | KW_USING
              | KW_VIRTUAL
              | KW_VOID
              | KW_VOLATILE
              | KW_WCHAR_T
              | KW_WHILE
              | KW_U8
              | KW_U16
              | KW_U32
              | KW_U64
              | KW_I8
              | KW_I16
              | KW_I32
              | KW_I64
              | KW_PUBLISHED
    """


if TYPE_CHECKING:
    from ply.yacc import YaccProduction