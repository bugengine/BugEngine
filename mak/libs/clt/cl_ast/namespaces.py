from .cppobject import CppObject


class Namespace(CppObject):
    def __init__(self, lexer, position, name):
        CppObject.__init__(self, lexer, position, name)
        self.register()

    def get_token_type(self):
        return 'NAMESPACE_ID'


class AnonymousNamespace(Namespace):
    def __init__(self, lexer, position):
        Namespace.__init__(self, lexer, position, None)
        self.register()

    def find(self, name):
        return self.scope and self.scope.find(name)


class RootNamespace(CppObject):
    def __init__(self, lexer):
        CppObject.__init__(self, lexer, (lexer.filename, 1, 1, 1), None)

    def write_to(self, writer):
        pass
