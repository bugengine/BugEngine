static inline be_pointer_<T> create(::BugEngine::Allocator& allocator)
{
    void* mem = allocator.alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T, allocator);
}
