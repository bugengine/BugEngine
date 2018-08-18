from .scope import Scope


class Root(Scope):
    def __init__(self, parser):
        Scope.__init__(self, None, None)
        self.parser = parser

    def write_to(self, writer):
        with writer.create_document() as document:
            Scope._write_to(self, document)

    def debug_dump(self):
        Scope._debug_dump(self, '')


class Namespace(Scope):
    def __init__(self, parent, position, name):
        Scope.__init__(self, parent, position)
        self.name = name

    def get_token_type(self):
        return 'NAMESPACE_ID'

    def find_nonrecursive(self, name):
        if self.name == name:
            return self

    def write_to(self, writer):
        with writer.create_namespace(self.position, self.name) as namespace:
            Scope._write_to(self, namespace)

    def _debug_dump(self, indent):
        print('namespace %s\n%s{' % (self.name, indent))
        Scope._debug_dump(self, indent + '  ')
        print('}\n')


class AnonymousNamespace(Scope):
    def __init__(self, parent, position):
        Scope.__init__(self, parent, position)

    def get_token_type(self):
        return 'NAMESPACE_ID'

    def find_nonrecursive(self, name):
        o = self.find(name, False)
        return o

    def write_to(self, writer):
        with writer.create_namespace(self.position, None) as namespace:
            Scope._write_to(self, namespace)

    def _debug_dump(self, indent):
        print(indent + 'namespace\n%s{' % indent)
        Scope._debug_dump(self, indent)
        print(indent + '}\n%s' % indent)
