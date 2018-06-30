class Scope:
    def __init__(self, position):
        self.members = []
        self.position = position

    def find(self, name, is_current_scope):
        for m in self.members:
            sub = m.find_nonrecursive(name)
            if sub:
                return sub

    def add(self, member):
        self.members.append(member)

    def instantiate(self, template_arguments):
        return self

    def _write_to(self, writer):
        for m in self.members:
            m.write_to(writer)
