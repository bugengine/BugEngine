import cpp

class ScopePrecedence(cpp.yacc.Precedence): "%left"

class keyword(cpp.yacc.Token):			pass
class PUBLISHED(keyword):				"%token"
class STRUCT(keyword):					"%token"
class CLASS(keyword):					"%token"
class ENUM(keyword):					"%token"
class BUILTIN(keyword):					"%token"
class NAMESPACE(keyword):				"%token"
class UNION(keyword):					"%token"
class USING(keyword):					"%token"
class NEW(keyword):						"%token"
class DELETE(keyword):					"%token"
class PUBLIC(keyword):					"%token"
class PROTECTED(keyword):				"%token"
class PRIVATE(keyword):					"%token"
class FRIEND(keyword):					"%token"
class SIGNED(keyword):					"%token"
class UNSIGNED(keyword):				"%token"
class SHORT(keyword):					"%token"
class CHAR(keyword):					"%token"
class LONG(keyword):					"%token"
class INT(keyword):						"%token"
class FLOAT(keyword):					"%token"
class DOUBLE(keyword):					"%token"
class EXPLICIT(keyword):				"%token"
class INLINE(keyword):					"%token"
class EXTERN(keyword):					"%token"
class STATIC(keyword):					"%token"
class MUTABLE(keyword):					"%token"
class CONST(keyword):					"%token"
class VOLATILE(keyword):				"%token"
class VIRTUAL(keyword):					"%token"
class OVERRIDE(keyword):				"%token"
class TEMPLATE(keyword):				"%token"
class TYPENAME(keyword):				"%token"
class OPERATOR(keyword):				"%token"
class TYPEDEF(keyword):					"%token"
class THROW(keyword):					"%token"
class BE_TAG(keyword):					"%token"

class ID(cpp.yacc.Token):				"%token"
class CHARCONST(cpp.yacc.Token):		"%token"
class WCHAR(cpp.yacc.Token):			"%token"
class STRING(cpp.yacc.Token):			"%token"
class WSTRING(cpp.yacc.Token):			"%token"
class FLOATING(cpp.yacc.Token):			"%token"
class DECIMAL(cpp.yacc.Token):			"%token"
class OCTAL(cpp.yacc.Token):			"%token"
class HEX(cpp.yacc.Token):				"%token"
class PLUS(cpp.yacc.Token):				"%token"
class MINUS(cpp.yacc.Token):			"%token"
class TIMES(cpp.yacc.Token):			"%token"
class DIVIDE(cpp.yacc.Token):			"%token"
class MOD(cpp.yacc.Token):				"%token"
class OR(cpp.yacc.Token):				"%token"
class AND(cpp.yacc.Token):				"%token"
class NOT(cpp.yacc.Token):				"%token"
class XOR(cpp.yacc.Token):				"%token"
class LSHIFT(cpp.yacc.Token):			"%token"
class RSHIFT(cpp.yacc.Token):			"%token"
class LOR(cpp.yacc.Token):				"%token"
class LAND(cpp.yacc.Token):				"%token"
class LNOT(cpp.yacc.Token):				"%token"
class LT(cpp.yacc.Token):				"%token"
class LE(cpp.yacc.Token):				"%token"
class GT(cpp.yacc.Token):				"%token"
class GE(cpp.yacc.Token):				"%token"
class EQ(cpp.yacc.Token):				"%token"
class NE(cpp.yacc.Token):				"%token"
class SCOPE(cpp.yacc.Token):			"%token [ScopePrecedence]"
class EQUAL(cpp.yacc.Token):			"%token"
class TIMESEQUAL(cpp.yacc.Token):		"%token"
class DIVEQUAL(cpp.yacc.Token):			"%token"
class MODEQUAL(cpp.yacc.Token):			"%token"
class PLUSEQUAL(cpp.yacc.Token):		"%token"
class MINUSEQUAL(cpp.yacc.Token):		"%token"
class LSHIFTEQUAL(cpp.yacc.Token):		"%token"
class RSHIFTEQUAL(cpp.yacc.Token):		"%token"
class ANDEQUAL(cpp.yacc.Token):			"%token"
class XOREQUAL(cpp.yacc.Token):			"%token"
class OREQUAL(cpp.yacc.Token):			"%token"
class PLUSPLUS(cpp.yacc.Token):			"%token"
class MINUSMINUS(cpp.yacc.Token):		"%token"
class ARROW(cpp.yacc.Token):			"%token"
class CONDOP(cpp.yacc.Token):			"%token"
class LPAREN(cpp.yacc.Token):			"%token"
class RPAREN(cpp.yacc.Token):			"%token"
class LBRACKET(cpp.yacc.Token):			"%token"
class RBRACKET(cpp.yacc.Token):			"%token"
class LBRACE(cpp.yacc.Token):			"%token"
class RBRACE(cpp.yacc.Token):			"%token"
class COMMA(cpp.yacc.Token):			"%token"
class PERIOD(cpp.yacc.Token):			"%token"
class SEMI(cpp.yacc.Token):				"%token"
class COLON(cpp.yacc.Token):			"%token"
class ELLIPSIS(cpp.yacc.Token):			"%token"
class DOXY_BEGIN(cpp.yacc.Token):		"%token"
class DOXY_BEGIN_LEFT(cpp.yacc.Token):	"%token"
class DOXY_END(cpp.yacc.Token):			"%token"
class DOXY_NEWLINE(cpp.yacc.Token):		"%token"
class DOXY_WORD(cpp.yacc.Token):		"%token"
class DOXY_LIST(cpp.yacc.Token):		"%token"


