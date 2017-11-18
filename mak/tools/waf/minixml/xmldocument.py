
class XmlDocument:
	def __init__(self, file, encoding, processing_instructions = []):
		self.file = file
		self.file.write('<?xml version="1.0" encoding="%s" standalone="no"?>\n' % encoding)
		for instruction in processing_instructions:
			self.file.write('<?%s %s?>\n' % instruction)
		self.closed = False
		self.empty = False
		self.current = None
		self.indent = -1

	def __enter__(self):
		return self

	def __exit__(self, type, value, traceback):
		self.close()

	def close(self):
		self.file.close()
