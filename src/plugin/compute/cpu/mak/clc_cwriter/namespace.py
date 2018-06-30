from .struct import Struct
from .method import Method


class Namespace:
    def __init__(self, name):
        self.name = name

    def __enter__(self):
        return self

    def __exit__(self, type, value, traceback):
        pass

    def create_namespace(self, position, namespace_name):
        return Namespace(self.out_file)

    def create_struct(self, position, struct_id, struct_name, parent_id):
        return Struct(position, struct_id, struct_name, parent_id)

    def create_method(self, position, method_id, method_name, overload_id):
        return Method(position, method_id, method_name, overload_id)
