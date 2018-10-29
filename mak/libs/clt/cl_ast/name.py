class Name:
    def __init__(self, lexer, name, position, target = None, targets = None, qualified = False,
                 dependent = False, resolved = True, data=None):
        self.lexer = lexer
        self.name = name
        self.position = position
        self.target = target
        self.targets = targets or ((target, tuple(), target),)
        self.qualified = qualified
        self.dependent = dependent
        self.resolved = resolved
        self.absolute = False
        self.data = data

    def __add__(self, other):
        return Name(self.lexer,
                    self.name + other.name,
                    other.position,
                    target = other.target,
                    targets = self.targets + other.targets,
                    qualified = True,
                    dependent = self.dependent or other.dependent,
                    resolved = self.resolved and other.resolved,
                    data = other.data)

    def instantiate(self, parent, template_arguments):
        result = self.Name(self.lexer, self.name, self.position,
                           self.target.instantiate(parent, template_arguments),
                           ((t[0].instantiate(parent, template_arguments),
                             [p.instantiate(parent, template_arguments) for p in t[1]],
                             t[2].instantiate(parent, template_arguments)) for t in self.targets),
                           self.qualified,
                           self.dependent,
                           self.resolved,
                           self.data.instantiate(parent, template_argumnents))
