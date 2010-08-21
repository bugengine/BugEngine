#! python
def template(f, n, const, types = '', params = '', args = ''):
	if n > 0:
		if const:
			if types:
				template(f, n-1, False, ('class A%d, ' % n)+types, ('const A%d& a%d, ' %(n,n)) + params, ('a%d, '%n) + args)
				template(f, n-1, True, ('class A%d, ' % n)+types, ('const A%d& a%d, ' %(n,n)) + params, ('a%d, '%n) + args)
			else:
				template(f, n-1, False, ('class A%d ' % n), ('const A%d& a%d' %(n,n)), ('a%d'%n))
				template(f, n-1, True, ('class A%d ' % n), ('const A%d& a%d' %(n,n)), ('a%d'%n))
		else:
			if types:
				template(f, n-1, False, ('class A%d, ' % n)+types, ('A%d& a%d, ' %(n,n)) + params, ('a%d, '%n) + args)
				template(f, n-1, True, ('class A%d, ' % n)+types, ('A%d& a%d, ' %(n,n)) + params, ('a%d, '%n) + args)
			else:
				template(f, n-1, False, ('class A%d ' % n), ('A%d& a%d' %(n,n)), ('a%d'%n))
				template(f, n-1, True, ('class A%d ' % n), ('A%d& a%d' %(n,n)), ('a%d'%n))
	elif const:
		if types :
			f.write('template< int ARENA, %s > static inline be_pointer_<T> create(%s)\n' % (types, params))
			f.write('{\n')
			f.write('    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));\n')
			f.write('    return be_pointer_<T>(new(mem) T(%s), &BugEngine::Memory<ARENA>::free);\n' % args)
			f.write('}\n');
		else:
			f.write('template< int ARENA > static inline be_pointer_<T> create()\n')
			f.write('{\n')
			f.write('    void* mem = BugEngine::Memory<ARENA>::alloc(sizeof(T), be_alignof(T));\n')
			f.write('    return be_pointer_<T>(new(mem) T, &BugEngine::Memory<ARENA>::free);\n')
			f.write('}\n');


for i in range(0,7):
	f = open('factory%d.inl' % i, 'w');
	template(f, i, True);
	template(f, i, False);
