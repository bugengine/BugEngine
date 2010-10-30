template< int ARENA, class A1, class A2, class A3  > static inline be_pointer_<T> create(const A1& a1, const A2& a2, const A3& a3)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3), &BugEngine::Memory<ARENA>::free);
}
