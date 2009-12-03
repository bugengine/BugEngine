template< class A1, class A2  > static inline be_create<T> create(A1& a1, const A2& a2) { return be_create<T>(new T(a1, a2)); }
template< class A1, class A2  > static inline be_create<T> create(const A1& a1, const A2& a2) { return be_create<T>(new T(a1, a2)); }
template< class A1, class A2  > static inline be_create<T> create(A1& a1, A2& a2) { return be_create<T>(new T(a1, a2)); }
template< class A1, class A2  > static inline be_create<T> create(const A1& a1, A2& a2) { return be_create<T>(new T(a1, a2)); }
