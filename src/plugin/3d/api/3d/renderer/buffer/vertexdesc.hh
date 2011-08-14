/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_3D_RENDERER_BUFFER_VERTEXDESC_HH_
#define BE_3D_RENDERER_BUFFER_VERTEXDESC_HH_
/*****************************************************************************/

namespace BugEngine { namespace Graphics
{


enum VertexSemantics
{
    gPosition,
    gNormal,
    gDiffuse,
    gSpecular,
    gUV0,
    gUV1,
    gUV2,
    gUV3,
    gUV4,
    gUV5,
    gUV6,
    gUV7
};

enum VertexUsage
{
    VuDynamic,
    VuStatic
};

struct VertexElementInfo
{
    enum VertexElementType
    {
        float1,
        float2,
        float3,
        float4,
        color32,
        byte4,
        short2,
        short4,
        ushort2,
        ushort4
    };
    
    size_t              offset;
    VertexElementType   type;
    VertexSemantics     semantic;
};

template< typename T >
struct VertexElementTypeMapToIndex;
#define MAKETYPEINDEX(type,index)       \
    template<> struct VertexElementTypeMapToIndex<type> { enum { Index = VertexElementInfo::index }; }
MAKETYPEINDEX(float,  float1);
MAKETYPEINDEX(float2, float2);
MAKETYPEINDEX(float3, float3);
MAKETYPEINDEX(float4, float4);
MAKETYPEINDEX(color32, color32);
MAKETYPEINDEX(byte4, byte4);
MAKETYPEINDEX(short2, short2);
MAKETYPEINDEX(short4, short4);
#undef MAKETYPEINDEX

struct VertexBufferFlags
{
    u32                       vertexSize;
    u32                       elementCount;
    const VertexElementInfo*  info;
};

template< typename T, int sem >
struct  VertexSemantic;

#define MAKESEMANTIC(sem,_name)                                                     \
template<typename T> struct VertexSemantic<T,sem>                                   \
{                                                                                   \
    T _name;                                                                        \
    static const char *name() { return #_name; }                                    \
}

MAKESEMANTIC(gPosition, position);
MAKESEMANTIC(gNormal,   normal);
MAKESEMANTIC(gDiffuse,  diffuse);
MAKESEMANTIC(gSpecular, specular);
MAKESEMANTIC(gUV0,      uv0);
MAKESEMANTIC(gUV1,      uv1);
MAKESEMANTIC(gUV2,      uv2);
MAKESEMANTIC(gUV3,      uv3);
MAKESEMANTIC(gUV4,      uv4);
MAKESEMANTIC(gUV5,      uv5);
MAKESEMANTIC(gUV6,      uv6);
MAKESEMANTIC(gUV7,      uv7);
#undef MAKESEMANTIC

template< typename T, int _semantic, typename _queue >
struct VertexSemanticList
{
    typedef T VertexSemanticType;
    enum { semantic = _semantic };
    typedef _queue queue;
};

template< typename Queue >
struct VertexDescription :  public VertexDescription< typename Queue::queue >
                         ,  public VertexSemantic< typename Queue::VertexSemanticType, Queue::semantic>
{
    typedef VertexDescription< typename Queue::queue >    parent;
    enum
    {
        VertexComponents = parent::VertexComponents + 1
    };
    static VertexBufferFlags getFlags()
    {
        static VertexBufferFlags flags;
        static VertexElementInfo desc[VertexComponents];
        static VertexBufferFlags pflags = parent::getFlags();
        desc[VertexComponents-1].offset = pflags.vertexSize;
        desc[VertexComponents-1].type = VertexElementInfo::VertexElementType(VertexElementTypeMapToIndex<typename Queue::VertexSemanticType>::Index);
        desc[VertexComponents-1].semantic = VertexSemantics(Queue::semantic);
        if (VertexComponents > 1)
            memcpy(desc, pflags.info, sizeof(VertexElementInfo)*(VertexComponents-1));

        flags.vertexSize = sizeof(typename Queue::VertexSemanticType) + pflags.vertexSize;
        flags.elementCount = VertexComponents;
        flags.info = desc;
        return flags;
    }
};

template< >
struct VertexDescription<void>
{
    enum
    {
        VertexComponents = 0
    };
    static VertexBufferFlags getFlags()
    {
        static VertexBufferFlags flags;
        flags.info = 0;
        flags.elementCount = 0;
        flags.vertexSize = 0;
        return flags;
    }
};





#define MAKEVERTEXLIST1(t0, s0)    \
    VertexSemanticList<t0,s0,void>
#define MAKEVERTEXLIST2(t0, s0, t1, s1)    \
    VertexSemanticList< t1,s1,MAKEVERTEXLIST1(t0, s0) > 
#define MAKEVERTEXLIST3(t0, s0, t1, s1, t2, s2)    \
    VertexSemanticList< t2,s2,MAKEVERTEXLIST2(t0, s0, t1, s1) > 
#define MAKEVERTEXLIST4(t0, s0, t1, s1, t2, s2, t3, s3)    \
    VertexSemanticList< t3,s3,MAKEVERTEXLIST3(t0, s0, t1, s1, t2, s2) > 
#define MAKEVERTEXLIST5(t0, s0, t1, s1, t2, s2, t3, s3, t4, s4)    \
    VertexSemanticList< t4,s4,MAKEVERTEXLIST4(t0, s0, t1, s1, t2, s2, t3, s3) > 
#define MAKEVERTEXLIST6(t0, s0, t1, s1, t2, s2, t3, s3, t4, s4, t5, s5)    \
    VertexSemanticList< t5,s5,MAKEVERTEXLIST5(t0, s0, t1, s1, t2, s2, t3, s3, t4, s4) > 
#define MAKEVERTEXLIST7(t0, s0, t1, s1, t2, s2, t3, s3, t4, s4, t5, s5, t6, s6)    \
    VertexSemanticList< t6,s6,MAKEVERTEXLIST6(t0, s0, t1, s1, t2, s2, t3, s3, t4, s4, t5, s5) > 
#define MAKEVERTEXLIST8(t0, s0, t1, s1, t2, s2, t3, s3, t4, s4, t5, s5, t6, s6, t7, s7)    \
    VertexSemanticList< t7,s7,MAKEVERTEXLIST7(t0, s0, t1, s1, t2, s2, t3, s3, t4, s4, t5, s5, t6, s6) > 
#define MAKEVERTEXLIST9(t0, s0, t1, s1, t2, s2, t3, s3, t4, s4, t5, s5, t6, s6, t7, s7, t8, s8)    \
    VertexSemanticList< t8,s8,MAKEVERTEXLIST8(t0, s0, t1, s1, t2, s2, t3, s3, t4, s4, t5, s5, t6, s6, t7, s7) > 
#define MAKEVERTEXLIST10(t0, s0, t1, s1, t2, s2, t3, s3, t4, s4, t5, s5, t6, s6, t7, s7, t8, s8, t9, s9)    \
    VertexSemanticList< t9,s9,MAKEVERTEXLIST9(t0, s0, t1, s1, t2, s2, t3, s3, t4, s4, t5, s5, t6, s6, t7, s7, t8, s8) > 
#define MAKEVERTEXLIST11(t0, s0, t1, s1, t2, s2, t3, s3, t4, s4, t5, s5, t6, s6, t7, s7, t8, s8, t9, s9, t10, s10)    \
    VertexSemanticList< t10,s10,MAKEVERTEXLIST10(t0, s0, t1, s1, t2, s2, t3, s3, t4, s4, t5, s5, t6, s6, t7, s7, t8, s8, t9, s9) > 
#define MAKEVERTEXLIST12(t0, s0, t1, s1, t2, s2, t3, s3, t4, s4, t5, s5, t6, s6, t7, s7, t8, s8, t9, s9, t10, s10, t11, s11)    \
    VertexSemanticList< t11,s11,MAKEVERTEXLIST11(t0, s0, t1, s1, t2, s2, t3, s3, t4, s4, t5, s5, t6, s6, t7, s7, t8, s8, t9, s9, t10, s10) > 
#define MAKEVERTEXLIST13(t0, s0, t1, s1, t2, s2, t3, s3, t4, s4, t5, s5, t6, s6, t7, s7, t8, s8, t9, s9, t10, s10, t11, s11, t12, s12)    \
    VertexSemanticList< t2,s12,MAKEVERTEXLIST12(t0, s0, t1, s1, t2, s2, t3, s3, t4, s4, t5, s5, t6, s6, t7, s7, t8, s8, t9, s9, t10, s10, t11, s11) > 
#define MAKEVERTEXLIST14(t0, s0, t1, s1, t2, s2, t3, s3, t4, s4, t5, s5, t6, s6, t7, s7, t8, s8, t9, s9, t10, s10, t11, s11, t12, s12, t13, s13)    \
    VertexSemanticList< t13,s13,MAKEVERTEXLIST13(t0, s0, t1, s1, t2, s2, t3, s3, t4, s4, t5, s5, t6, s6, t7, s7, t8, s8, t9, s9, t10, s10, t11, s11, t12, s12) > 
#define MAKEVERTEXLIST15(t0, s0, t1, s1, t2, s2, t3, s3, t4, s4, t5, s5, t6, s6, t7, s7, t8, s8, t9, s9, t10, s10, t11, s11, t12, s12, t13, s13, t14, s14)    \
    VertexSemanticList< t14,s14,MAKEVERTEXLIST14(t0, s0, t1, s1, t2, s2, t3, s3, t4, s4, t5, s5, t6, s6, t7, s7, t8, s8, t9, s9, t10, s10, t11, s11, t12, s12, t13, s13) > 
#define MAKEVERTEXLIST16(t0, s0, t1, s1, t2, s2, t3, s3, t4, s4, t5, s5, t6, s6, t7, s7, t8, s8, t9, s9, t10, s10, t11, s11, t12, s12, t13, s13, t14, s14, t15, s15)    \
    VertexSemanticList< t15,s15,MAKEVERTEXLIST15(t0, s0,t1, s1, t2, s2, t3, s3, t4, s4, t5, s5, t6, s6, t7, s7, t8, s8, t9, s9, t10, s10, t11, s11, t12, s12, t13, s13, t14, s14) > 




struct IndexBufferFlags
{
    enum IndexBufferFormat
    {
        Short,
        Long
    };

    IndexBufferFormat format;

    inline IndexBufferFlags(IndexBufferFormat f) : format(f) {}
};

enum IndexUsage
{
    IuDynamic,
    IuStatic
};

}}

/*****************************************************************************/
#endif
