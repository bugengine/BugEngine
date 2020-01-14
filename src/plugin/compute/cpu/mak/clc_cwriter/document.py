from .struct import Struct
from .method import Method
from .variable import Variable
from .namespace import Namespace
from .typewriter import TypeWriter


class Document(TypeWriter):
    def __init__(self, out_file):
        TypeWriter.__init__(self, self)
        self.out_file = out_file
        self.methods = {}
        self.structs = {}
        self.struct_dependencies = {}

    def __enter__(self):
        self.out_file.write(b'#include <kernel/stdafx.h>\n')
        return self

    def __exit__(self, type, value, traceback):
        for struct in self.structs.values():
            struct.declare(self.out_file)
        seen = set([])

        def define_struct(struct_id, struct):
            if struct_id not in seen:
                seen.add(struct_id)
                for dependency_id in self.struct_dependencies.get(struct_id, []):
                    define_struct(
                        dependency_id, self.get_struct(dependency_id))
                struct.define(self.out_file)
        for struct_id, struct in self.structs.items():
            define_struct(struct_id, struct)

    def write_position(self, position):
        self.out_file.write(b'#line %d "%s"\n' % (
            position.line_number, position.filename.encode('utf-8')))

    def create_namespace(self, position, namespace_name):
        return Namespace(self, namespace_name and [namespace_name] or [])

    def create_struct(self, position, struct_id, struct_name, parent_visibility, parent_id):
        return Struct(self, position, struct_id, [struct_name], parent_visibility, parent_id)

    def create_method(self, position, method_id, method_name, overload_id):
        return Method(self, position, method_id, [method_name], overload_id)

    def create_variable(self, position, type, name, initial_value, attributes):
        print(attributes)
        return Variable(self.document, position, type, name, initial_value, attributes)

    def register_struct(self, struct_id, struct):
        self.structs[struct_id] = struct

    def register_dependencies(self, struct_id, dependencies):
        try:
            self.struct_dependencies[struct_id] += dependencies
        except KeyError:
            self.struct_dependencies[struct_id] = dependencies

    def get_struct(self, struct_id):
        return self.structs[struct_id]
