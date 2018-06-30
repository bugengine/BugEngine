from .scope import Scope


class Root(Scope):
    def write_to(self, writer):
        with writer.create_document() as document:
            Scope._write_to(self, document)


class Namespace(Scope):
    def __init__(self, name, position):
        Scope.__init__(self, position)
        self.name = name

    def get_token_type(self):
        return 'NAMESPACE_ID'

    def find_nonrecursive(self, name):
        if self.name == name:
            return self

    def write_to(self, writer):
        with writer.create_namespace(self.position, self.name) as namespace:
            Scope._write_to(self, namespace)


class AnonymousNamespace(Scope):
    def __init__(self, position):
        Scope.__init__(self, position)

    def get_token_type(self):
        return 'NAMESPACE_ID'

    def find_nonrecursive(self, name):
        o = self.find(name, False)
        return o

    def write_to(self, writer):
        with writer.create_namespace(self.position, None) as namespace:
            Scope._write_to(self, namespace)
