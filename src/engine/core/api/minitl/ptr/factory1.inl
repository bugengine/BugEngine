template< int ARENA, class A1  > static inline be_pointer_<T> create(const A1& a1)
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1), &BugEngine::Memory<ARENA>::free);
}
