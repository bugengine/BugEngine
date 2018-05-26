from .scope import Scope


class Root(Scope):
    def write_to(self, writer):
        writer.begin_document()
        writer.end_document()


class Namespace(Scope):
    def __init__(self, name, position):
        Scope.__init__(self, position)
        self.name = name

    def get_token_type(self):
        return 'NAMESPACE_ID'

    def dump(self, indent=''):
        print('namespace %s\n{' % self.name)
        for m in self.members:
            m.dump(indent)
        print('}')

    def find_nonrecursive(self, name):
        if self.name == name:
            return self


class AnonymousNamespace(Scope):
    def __init__(self, position):
        Scope.__init__(self, position)

    def get_token_type(self):
        return 'NAMESPACE_ID'

    def find_nonrecursive(self, name):
        o = self.find(name)
        return o
