template< class A1, class A2, class A3, class A4  > static inline be_pointer_<T> create(::BugEngine::Allocator& allocator, const A1& a1, const A2& a2, const A3& a3, const A4& a4)
{
    void* mem = allocator.alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T(a1, a2, a3, a4), allocator);
}
