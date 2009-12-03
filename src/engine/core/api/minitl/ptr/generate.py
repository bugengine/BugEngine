#! /bin/python
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
			f.write('template< %s > static inline be_create<T> create(%s) { return be_create<T>(new T(%s)); }\n' % (types, params, args));
		else:
			f.write('static inline be_create<T> create() { return be_create<T>(new T()); }\n');


for i in range(0,7):
	f = open('factory%d.inl' % i, 'w');
	template(f, i, True);
	template(f, i, False);
