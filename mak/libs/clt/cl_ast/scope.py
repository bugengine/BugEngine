from .cppobject import CppObject


class Scope(CppObject):
    def __init__(self, parent, position):
        CppObject.__init__(self, parent, position)
        self.members = []

    def find(self, name, is_current_scope):
        for m in self.members:
            sub = m.find_nonrecursive(name)
            if sub:
                return sub

    def add(self, member):
        self.members.append(member)

    def _write_to(self, writer):
        for m in self.members:
            m.write_to(writer)

    def _debug_dump(self, indent):
        for m in self.members:
            m._debug_dump(indent)
