template< int ARENA, class A1, class A2  > static inline be_pointer_<T> create(A1& a1, const A2& a2)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2  > static inline be_pointer_<T> create(const A1& a1, const A2& a2)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2  > static inline be_pointer_<T> create(A1& a1, A2& a2)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2), &BugEngine::Memory<ARENA>::free);
}
template< int ARENA, class A1, class A2  > static inline be_pointer_<T> create(const A1& a1, A2& a2)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2), &BugEngine::Memory<ARENA>::free);
}
