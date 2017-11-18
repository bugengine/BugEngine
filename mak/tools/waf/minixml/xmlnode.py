
def xmlify(s):
	s = s.replace("&", "&amp;") # do this first
	s = s.replace("'", "&apos;")
	s = s.replace('"', "&quot;")
	return s


class XmlNode:
	def __init__(self, parent, name, text='', attributes = None):
		assert(not parent.closed)
		assert(not parent.current)
		parent.current = self
		if (parent.empty):
			parent.begin()
		self.parent = parent
		self.current = None
		self.name = name
		self.indent = parent.indent+1
		self.file = parent.file
		self.closed = False
		self.empty = True
		parent.empty = False
		self.open(text, attributes)

	def __enter__(self):
		return self

	def __exit__(self, type, value, traceback):
		if not self.closed:
			self.close()

	def open(self, text, attributes):
		self.file.write('%s<%s' % (' '*self.indent, self.name))
		if attributes:
			indent = self.indent + 4
			if isinstance(attributes, dict):
				for key, value in attributes.items():
					self.file.write(' %s="%s"' % (key, value))
			else:
				for key, value in attributes:
					self.file.write(' %s="%s"' % (key, value))
		if isinstance(text, dict):
			indent = self.indent + 4
			for key, value in text.items():
				self.file.write(' %s="%s"' % (key, xmlify(value)))
		elif isinstance(text, list) or isinstance(text, tuple):
			indent = self.indent + 4
			for key, value in text:
				self.file.write(' %s="%s"' % (key, xmlify(value)))
		elif isinstance(text, str) and text:
			self.file.write('>%s</%s>\n' % (xmlify(text), self.name))
			assert(self.parent.current == self)
			self.parent.current = None
			self.closed = True
			self.empty = False

	def close(self):
		if not self.closed:
			assert(self.parent.current == self)
			self.parent.current = None
			if self.empty:
				self.file.write(' />\n')
			else:
				self.file.write('%s</%s>\n' % (' '*self.indent, self.name))

	def begin(self):
		self.file.write('>\n')
		self.empty = False
