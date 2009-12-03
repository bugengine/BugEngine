template< class A1  > static inline be_create<T> create(const A1& a1) { return be_create<T>(new T(a1)); }
template< class A1  > static inline be_create<T> create(A1& a1) { return be_create<T>(new T(a1)); }
