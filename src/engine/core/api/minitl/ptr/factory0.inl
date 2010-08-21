template< int ARENA > static inline be_pointer_<T> create()
{
    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));
    return be_pointer_<T>(new(mem) T, &BugEngine::Memory<ARENA>::free);
}
