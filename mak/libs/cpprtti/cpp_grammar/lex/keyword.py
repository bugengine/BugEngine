"""
keyword:
  	alignas     C++0x
  	alignof     C++0x
  	asm
  	auto
  	bool
  	break
  	case
  	catch
  	char
  	char16_t     C++0x
  	char32_t     C++0x
  	class
  	const
  	constexpr     C++0x
  	const_cast
  	continue
  	decltype     C++0x
  	default
  	delete
  	do
  	double
  	dynamic_cast
  	else
  	enum
  	explicit
  	export     C++0x - Reserved for future use
  	extern
  	false
  	float
  	for
  	friend
  	goto
  	if
  	inline
  	int
  	long
  	mutable
  	namespace
  	new
  	noexcept     C++0x
  	nullptr     C++0x
  	operator
  	private
  	protected
  	public
  	register
  	reinterpret_cast
  	return
  	short
  	signed
  	sizeof
  	static
  	static_assert     C++0x
  	static_cast
  	struct
  	switch
  	template
  	this
  	thread_local     C++0x
  	throw
  	true
  	try
  	typedef
  	typeid
  	typename
  	union
  	unsigned
  	using
  	virtual
  	void
  	volatile
  	wchar_t
  	while
"""

keywords = (
    'alignas',             # C++0x
    'alignof',             # C++0x
    'asm',
    'auto',
    'bool',
    'break',
    'case',
    'catch',
    'char',
    'char16_t',            # C++0x
    'char32_t',            # C++0x
    'class',
    'const',
    'constexpr',           # C++0x
    'const_cast',
    'continue',
    'decltype',            # C++0x
    'default',
    'delete',
    'do',
    'double',
    'dynamic_cast',
    'else',
    'enum',
    'explicit',
                           #'export',              # C++0x - Reserved for future use
    'extern',
    'false',
    'final',
    'float',
    'for',
    'friend',
    'goto',
    'if',
    'inline',
    'int',
    'long',
    'mutable',
    'namespace',
    'new',
    'noexcept',            # C++0x
    'nullptr',             # C++0x
    'operator',
    'override',
    'private',
    'protected',
    'public',
    'register',
    'reinterpret_cast',
    'return',
    'short',
    'signed',
    'sizeof',
    'static',
    'static_assert',       # C++0x
    'static_cast',
    'struct',
    'switch',
    'template',
    'this',
    'thread_local',        # C++0x
    'throw',
    'true',
    'try',
    'typedef',
    'typeid',
    'typename',
    'union',
    'unsigned',
    'using',
    'virtual',
    'void',
    'volatile',
    'wchar_t',
    'while',
    'u8',                  # BugEngine extension
    'u16',                 # BugEngine extension
    'u32',                 # BugEngine extension
    'u64',                 # BugEngine extension
    'i8',                  # BugEngine extension
    'i16',                 # BugEngine extension
    'i32',                 # BugEngine extension
    'i64',                 # BugEngine extension
    'published',           # BugEngine extension
)