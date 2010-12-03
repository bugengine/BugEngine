#! python
def template(f, n, const, types = '', params = '', args = ''):
	if n > 0:
		if types:
			template(f, n-1, True, ('class A%d, ' % n)+types, ('const A%d& a%d, ' %(n,n)) + params, ('a%d, '%n) + args)
		else:
			template(f, n-1, True, ('class A%d ' % n), ('const A%d& a%d' %(n,n)), ('a%d'%n))
	else:
		if types :
			f.write('template< %s > static inline be_pointer_<T> create(::BugEngine::Allocator& allocator, %s)\n' % (types, params))
			f.write('{\n')
			f.write('    void* mem = allocator.alloc(sizeof(T), be_alignof(T));\n')
			f.write('    return be_pointer_<T>(new(mem) T(%s), allocator);\n' % args)
			f.write('}\n');
		else:
			f.write('static inline be_pointer_<T> create(::BugEngine::Allocator& allocator)\n')
			f.write('{\n')
			f.write('    void* mem = allocator.alloc(sizeof(T), be_alignof(T));\n')
			f.write('    return be_pointer_<T>(new(mem) T, allocator);\n')
			f.write('}\n');


for i in range(0,7):
	f = open('factory%d.inl' % i, 'w');
	template(f, i, True);
