from .typewriter import TypeWriter
from .struct import Struct
from .method import Method
from .variable import Variable


class Namespace(TypeWriter):
    def __init__(self, document, name):
        TypeWriter.__init__(self, document)
        self.name = name

    def __enter__(self):
        return self

    def __exit__(self, type, value, traceback):
        pass

    def create_namespace(self, position, namespace_name):
        return Namespace(self.document, self.name + [namespace_name])

    def create_struct(self, position, struct_id, struct_name, parent_visibility, parent_id):
        return Struct(self.document, position, struct_id, self.name + [struct_name], parent_visibility, parent_id)

    def create_method(self, position, method_id, method_name, overload_id):
        return Method(self.document, position, method_id, self.name + [method_name], overload_id)

    def create_variable(self, position, type, name, initial_value, attributes):
        print(attributes)
        return Variable(self.document, position, type, name, initial_value, attributes)
