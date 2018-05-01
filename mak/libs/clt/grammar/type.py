from .. import cl_ast

def p_type_builtin(p):
    """
        type :  I8
             |  U8
             |  I16
             |  U16
             |  I32
             |  U32
             |  I64
             |  U64
             |  FLOAT
             |  DOUBLE
             |  HALF
             |  BOOL
             |  SIZE_T
             |  PTRDIFF_T
             |  INTPTR_T
             |  UINTPTR_T
             |  BYTE2
             |  BYTE3
             |  BYTE4
             |  BYTE8
             |  BYTE16
             |  UBYTE2
             |  UBYTE3
             |  UBYTE4
             |  UBYTE8
             |  UBYTE16
             |  SHORT2
             |  SHORT3
             |  SHORT4
             |  SHORT8
             |  SHORT16
             |  USHORT2
             |  USHORT3
             |  USHORT4
             |  USHORT8
             |  USHORT16
             |  INT2
             |  INT3
             |  INT4
             |  INT8
             |  INT16
             |  UINT2
             |  UINT3
             |  UINT4
             |  UINT8
             |  UINT16
             |  BIGINT2
             |  BIGINT3
             |  BIGINT4
             |  BIGINT8
             |  BIGINT16
             |  BIGUINT2
             |  BIGUINT3
             |  BIGUINT4
             |  BIGUINT8
             |  BIGUINT16
             |  FLOAT2
             |  FLOAT3
             |  FLOAT4
             |  FLOAT8
             |  FLOAT16
             |  DOUBLE2
             |  DOUBLE3
             |  DOUBLE4
             |  DOUBLE8
             |  DOUBLE16
    """
    p[0] = cl_ast.Type(cl_ast.Builtin(p[1], p.position(1)))


def p_type_builtin_modifier_list_deprecated(p):
    """
        type_modifier_deprecated : CHAR
                                 | SHORT
                                 | INT
                                 | LONG
                                 | UCHAR
                                 | USHORT
                                 | UINT
                                 | ULONG
                                 | SIGNED
                                 | UNSIGNED
    """
    p[0] = (p[1], p.position(1))
    p.set_position(0, 1)


def p_type_builtin_deprecated(p):
    """
        type_modifier_list_deprecated : type_modifier_deprecated type_modifier_list_deprecated
                                      | type_modifier_deprecated
    """
    p[0] = [p[1]]
    p.set_position(0, 1)
    if len(p) > 2:
        p[0] += p[2]


def p_type_deprecated(p):
    """
        type : type_modifier_list_deprecated
    """
    result = 'i32'
    kw_data = {
        'char':     ('%(r)s8',  ['char', 'short', 'int', 'long', 'uchar', 'ushort', 'uint', 'ulong']),
        'short':    ('%(r)s16', ['char', 'short', 'long', 'uchar', 'ushort', 'uint', 'ulong']),
        'int':      ('%(r)s%(b)s',['char', 'int', 'uchar', 'ushort', 'uint', 'ulong']),
        'long':     ('%(r)s64', ['char', 'short', 'uchar', 'ushort', 'uint', 'ulong']),
        'uchar':    ('%(r)s8',  ['char', 'short', 'int', 'long', 'uchar', 'ushort', 'uint', 'ulong', 'signed', 'unsigned']),
        'ushort':   ('%(r)s16', ['char', 'short', 'int', 'long', 'uchar', 'ushort', 'uint', 'ulong', 'signed', 'unsigned']),
        'uint':     ('%(r)s32', ['char', 'short', 'int', 'long', 'uchar', 'ushort', 'uint', 'ulong', 'signed', 'unsigned']),
        'ulong':    ('%(r)s64', ['char', 'short', 'int', 'long', 'uchar', 'ushort', 'uint', 'ulong', 'signed', 'unsigned']),
        'signed':   ('i%(b)s',  ['uchar', 'ushort', 'uint', 'ulong', 'unsigned']),
        'unsigned': ('u%(b)s',  ['uchar', 'ushort', 'uint', 'ulong', 'signed']),
    }
    if len(p[1]) > 1 or p[1][0][0] != 'int':
        p.lexer._warning("old style C type", p.position(1))
    for s in p[1]:
        rebuild = { 'r': result[:1], 'b': result[1:]}
        rebuild_pattern, incompatible_kw = kw_data[s[0]]
        for s2 in p[1]:
            if s2 == s:
                break
            if s2[0] in incompatible_kw:
                p.lexer._error("cannot combine with previous '%s' declaration specifier" % s2[0], s[1])
        result = rebuild_pattern%rebuild
    p[0] = cl_ast.Type(cl_ast.Builtin(result, p.position(1)))


def p_type_type_name(p):
    """
        type : type_name
    """
    p[0] = cl_ast.Type(p[1][2])


def p_type_type_decl(p):
    """
        type : typedecl
    """
    p[0] = cl_ast.Type(p[1])


def p_type_ptr(p):
    """
        type : type TIMES
    """
    p[0] = cl_ast.Type(cl_ast.Pointer(p[1], p.position(2)))


def p_type_void_ptr(p):
    """
        type : VOID TIMES
    """
    p[0] = cl_ast.Type(cl_ast.Pointer(cl_ast.Builtin(p[1], p.position(1)), p.position(2)))

def p_type_const(p):
    """
        type : type CONST                                                       %prec TYPEMODIFIER
             | type __CONSTANT                                                  %prec TYPEMODIFIER
             | type __GLOBAL                                                    %prec TYPEMODIFIER
             | type __LOCAL                                                     %prec TYPEMODIFIER
             | type __PRIVATE                                                   %prec TYPEMODIFIER
             | type RESTRICT                                                    %prec TYPEMODIFIER
             | type __RESTRICT                                                  %prec TYPEMODIFIER
             | type VOLATILE                                                    %prec TYPEMODIFIER
    """
    p[0] = p[1]
    p[1].add_modifier(p[2], p.position(2))


def p_type_const_pre(p):
    """
        type : CONST type                                                       %prec TYPEMODIFIER
             | __CONSTANT type                                                  %prec TYPEMODIFIER
             | __GLOBAL type                                                    %prec TYPEMODIFIER
             | __LOCAL type                                                     %prec TYPEMODIFIER
             | __PRIVATE type                                                   %prec TYPEMODIFIER
             | RESTRICT type                                                    %prec TYPEMODIFIER
             | __RESTRICT type                                                  %prec TYPEMODIFIER
             | VOLATILE type                                                    %prec TYPEMODIFIER
    """
    p[0] = p[2]
    p[2].add_modifier(p[1], p.position(1))
