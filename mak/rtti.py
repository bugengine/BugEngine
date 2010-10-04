
class Container:
	namespace = '::'
	def __init__(self):
		self.objects = []

	def addObject(self, object):
		self.objects.append(object)

	def dump(self, file):
		for o in self.objects:
			o.dump(file)
			file.write('\n')

class Namespace(Container):
	def __init__(self, name):
		Container.__init__(self)
		self.name = name

	def dump(self, file):
		Container.namespace += self.name + '::'
		Container.dump(self, file)

class Type:
	def __init__(self, name):
		Container.__init__(self, name)
		self.attribs = set()

	def attrib(self, attribute):
		self.attribs.insert(attribute)

class Member(Container):
	def __init__(self, type, name):
		Container.__init__(self, name)
		self.attribs = set()
