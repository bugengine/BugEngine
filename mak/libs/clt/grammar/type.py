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
             |  INT
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
    p[0] = cl_ast.Builtin(p[1], p.position(1))


def p_type_ptr(p):
    """
        type : type TIMES
             | typedecl TIMES
    """
    p[0] = cl_ast.Pointer(p[1], p.position(2))


def p_type_void_ptr(p):
    """
        type : VOID TIMES
    """
    p[0] = cl_ast.Pointer(cl_ast.Builtin(p[1]), p.position(2))

