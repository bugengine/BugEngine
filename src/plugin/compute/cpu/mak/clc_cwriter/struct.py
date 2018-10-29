from .typewriter import TypeWriter
from .method import Method
from .variable import Variable


class Struct(TypeWriter):
    def __init__(self, document, position, struct_id, struct_name, parent_id):
        TypeWriter.__init__(self, document)
        self.position = position
        self.id = struct_id
        self.name = struct_name + [str(struct_id)]
        self.parent = parent_id
        if self.parent != None:
            self.document.register_dependencies(struct_id, [self.parent])
        self.document.register_struct(struct_id, self)
        self.variables = []

    def __enter__(self):
        return self

    def __exit__(self, type, value, traceback):
        pass

    def struct_name(self):
        return '_' + '_'.join(self.name)

    def create_struct(self, position, struct_id, struct_name, parent_id):
        return Struct(self.document, position, struct_id, self.name + [struct_name], parent_id)

    def create_method(self, position, method_id, method_name, overload_id):
        return Method(self.document, position, method_id, self.name + [method_name], overload_id)

    def create_variable(self, position, type, name, initial_value, attributes):
        result = Variable(self.document, position, type, name, initial_value, attributes)
        self.variables.append(result)
        return result

    def declare(self, out_file):
        self.document.write_position(self.position)
        out_file.write(b'struct %s;\n' % self.struct_name().encode('utf-8'))
        out_file.write(b'typedef struct %s struct_%d;\n' % (self.struct_name().encode('utf-8'),
                                                            self.id))

    def define(self, out_file):
        self.document.write_position(self.position)
        out_file.write(b'struct %s\n'
                       b'{\n' % self.struct_name().encode('utf-8'))
        for variable in self.variables:
            variable.define(out_file)
        out_file.write(b'};\n')
