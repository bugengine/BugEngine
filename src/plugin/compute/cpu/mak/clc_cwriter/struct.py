from .method import Method


class Struct:
    def __init__(self, position, struct_id, struct_name, parent_id):
        self.position = position
        self.id = struct_id
        self.name = struct_name
        self.parent = parent_id

    def __enter__(self):
        return self

    def __exit__(self, type, value, traceback):
        pass

    def create_struct(self, position, struct_id, struct_name, parent_id):
        return Struct(position, struct_id, struct_name, parent_id)

    def create_method(self, position, method_id, method_name, overload_id):
        return Method(position, method_id, method_name, overload_id)

