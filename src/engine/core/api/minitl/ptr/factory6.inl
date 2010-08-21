template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(A1& a1, A2& a2, A3& a3, A4& a4, A5& a5, const A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(const A1& a1, A2& a2, A3& a3, A4& a4, A5& a5, const A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(A1& a1, const A2& a2, A3& a3, A4& a4, A5& a5, const A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(const A1& a1, const A2& a2, A3& a3, A4& a4, A5& a5, const A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(A1& a1, A2& a2, const A3& a3, A4& a4, A5& a5, const A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(const A1& a1, A2& a2, const A3& a3, A4& a4, A5& a5, const A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(A1& a1, const A2& a2, const A3& a3, A4& a4, A5& a5, const A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(const A1& a1, const A2& a2, const A3& a3, A4& a4, A5& a5, const A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(A1& a1, A2& a2, A3& a3, const A4& a4, A5& a5, const A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(const A1& a1, A2& a2, A3& a3, const A4& a4, A5& a5, const A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(A1& a1, const A2& a2, A3& a3, const A4& a4, A5& a5, const A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(const A1& a1, const A2& a2, A3& a3, const A4& a4, A5& a5, const A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(A1& a1, A2& a2, const A3& a3, const A4& a4, A5& a5, const A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(const A1& a1, A2& a2, const A3& a3, const A4& a4, A5& a5, const A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(A1& a1, const A2& a2, const A3& a3, const A4& a4, A5& a5, const A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(const A1& a1, const A2& a2, const A3& a3, const A4& a4, A5& a5, const A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(A1& a1, A2& a2, A3& a3, A4& a4, const A5& a5, const A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(const A1& a1, A2& a2, A3& a3, A4& a4, const A5& a5, const A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(A1& a1, const A2& a2, A3& a3, A4& a4, const A5& a5, const A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(const A1& a1, const A2& a2, A3& a3, A4& a4, const A5& a5, const A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(A1& a1, A2& a2, const A3& a3, A4& a4, const A5& a5, const A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(const A1& a1, A2& a2, const A3& a3, A4& a4, const A5& a5, const A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(A1& a1, const A2& a2, const A3& a3, A4& a4, const A5& a5, const A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(const A1& a1, const A2& a2, const A3& a3, A4& a4, const A5& a5, const A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(A1& a1, A2& a2, A3& a3, const A4& a4, const A5& a5, const A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(const A1& a1, A2& a2, A3& a3, const A4& a4, const A5& a5, const A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(A1& a1, const A2& a2, A3& a3, const A4& a4, const A5& a5, const A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(const A1& a1, const A2& a2, A3& a3, const A4& a4, const A5& a5, const A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(A1& a1, A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(const A1& a1, A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(A1& a1, A2& a2, A3& a3, A4& a4, A5& a5, A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(const A1& a1, A2& a2, A3& a3, A4& a4, A5& a5, A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(A1& a1, const A2& a2, A3& a3, A4& a4, A5& a5, A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(const A1& a1, const A2& a2, A3& a3, A4& a4, A5& a5, A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(A1& a1, A2& a2, const A3& a3, A4& a4, A5& a5, A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(const A1& a1, A2& a2, const A3& a3, A4& a4, A5& a5, A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(A1& a1, const A2& a2, const A3& a3, A4& a4, A5& a5, A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(const A1& a1, const A2& a2, const A3& a3, A4& a4, A5& a5, A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(A1& a1, A2& a2, A3& a3, const A4& a4, A5& a5, A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(const A1& a1, A2& a2, A3& a3, const A4& a4, A5& a5, A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(A1& a1, const A2& a2, A3& a3, const A4& a4, A5& a5, A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(const A1& a1, const A2& a2, A3& a3, const A4& a4, A5& a5, A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(A1& a1, A2& a2, const A3& a3, const A4& a4, A5& a5, A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(const A1& a1, A2& a2, const A3& a3, const A4& a4, A5& a5, A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(A1& a1, const A2& a2, const A3& a3, const A4& a4, A5& a5, A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(const A1& a1, const A2& a2, const A3& a3, const A4& a4, A5& a5, A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(A1& a1, A2& a2, A3& a3, A4& a4, const A5& a5, A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(const A1& a1, A2& a2, A3& a3, A4& a4, const A5& a5, A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(A1& a1, const A2& a2, A3& a3, A4& a4, const A5& a5, A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(const A1& a1, const A2& a2, A3& a3, A4& a4, const A5& a5, A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(A1& a1, A2& a2, const A3& a3, A4& a4, const A5& a5, A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(const A1& a1, A2& a2, const A3& a3, A4& a4, const A5& a5, A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(A1& a1, const A2& a2, const A3& a3, A4& a4, const A5& a5, A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(const A1& a1, const A2& a2, const A3& a3, A4& a4, const A5& a5, A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(A1& a1, A2& a2, A3& a3, const A4& a4, const A5& a5, A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(const A1& a1, A2& a2, A3& a3, const A4& a4, const A5& a5, A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(A1& a1, const A2& a2, A3& a3, const A4& a4, const A5& a5, A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(const A1& a1, const A2& a2, A3& a3, const A4& a4, const A5& a5, A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(A1& a1, A2& a2, const A3& a3, const A4& a4, const A5& a5, A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(const A1& a1, A2& a2, const A3& a3, const A4& a4, const A5& a5, A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2, class A3, class A4, class A5, class A6  > static inline be_pointer_<T> create(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, A6& a6)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4, a5, a6), &BugEngine::Memory<ARENA>::free);
}
