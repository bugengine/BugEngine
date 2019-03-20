from .typewriter import TypeWriter

class Method(TypeWriter):
    def __init__(self, document, position, method_id, method_name, overload_id):
        TypeWriter.__init__(self, document)
        self.position = position
        self.id = method_id
        self.name = method_name
        self.overload = overload_id

    def __enter__(self):
        return self

    def __exit__(self, type, value, traceback):
        pass
