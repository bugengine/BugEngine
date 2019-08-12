from .cppobject import CppObject


class Namespace(CppObject):
    INITIAL_SCOPE = CppObject.NotDefinedScope

    def __init__(self, lexer, position, name):
        CppObject.__init__(self, lexer, position, name)
        self.register()

    def get_token_type(self):
        return 'NAMESPACE_ID'

    def write_to(self, writer):
        with writer.create_namespace(self.position, self.name) as namespace:
            self.scope.write_to(namespace)

    def pretty_name(sef):
        return "namespace '%s'" % self.name


class AnonymousNamespace(Namespace):
    def __init__(self, lexer, position):
        Namespace.__init__(self, lexer, position, None)
        self.register()

    def find(self, name):
        return self.scope and self.scope.find(name)

    def pretty_name(sef):
        return "anonymous namespace"


class RootNamespace(CppObject):
    def __init__(self, lexer):
        CppObject.__init__(self, lexer, (lexer.filename, 1, 1, 1), None)

    def write_to(self, writer):
        with writer.create_document() as document:
            self.scope.write_to(document)

    def pretty_name(sef):
        return "the global namespace"
