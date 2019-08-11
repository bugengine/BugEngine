class Scope:
    def __init__(self, owner, position, visibility='published'):
        self.owner = owner
        self.position = position
        self.visibility = visibility
        self.items = []

    def is_definition_scope(self):
        return True

    def add(self, element):
        self.items.append((self.visibility, element))

    def empty(self):
        return len(self.items) == 0

    def __getitem__(self, index):
        return self.items[index]

    def find(self, name, position, is_current_scope):
        for _, element in self.items:
            result = element.find(name)
            if result:
                return result
        return None

    def seal(self):
        self.owner.seal()

    def debug_dump(self, indent):
        for visibility, element in self.items:
            print('%s%s:' % (indent, visibility))
            element.debug_dump(indent + '  ')

    def create_template_instance(self, target_scope, template, arguments, position):
        for visibility, element in self.items:
            target_scope.items.append((visibility, element.create_template_instance(template, arguments, position)))

    def write_to(self, document):
        for _, object in self.items:
            object.write_to(document)
