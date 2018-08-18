from .typewriter import TypeWriter


class Variable(TypeWriter):
    def __init__(self, document, position, type, name, initial_value, attributes):
        TypeWriter.__init__(self, document)
        self.position = position
        self.type = type
        self.name = name
        self.value = initial_value
        self.attributes = attributes

    def __enter__(self):
        return self

    def __exit__(self, type, value, traceback):
        pass

    def declare(self, out_file):
        pass

    def define(self, out_file):
        pass
        #self.document.write_position(self.position)
        #if self.value:
        #    out_file.write(b'%s %s = %s;\n' % (self.type.type.encode('utf-8'),
        #                                       self.name.encode('utf-8'),
        #                                       self.value.encode('utf-8')))
        #else:
        #    out_file.write(b'%s %s;\n' % (self.type.type.encode('utf-8'),
        #                                  self.name.encode('utf-8')))
