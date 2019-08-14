
class Name:
    def __init__(self, lexer, name, position, target = None, targets = None, qualified = False,
                 dependent = False, data=None, positions=None):
        from .types import DependentTypeName
        if targets:
            assert isinstance(targets[-1], tuple)
        self.lexer = lexer
        self.name = name
        self.position = position
        self.positions = positions or (position,)
        if dependent:
            #print(target)
            #assert not target or isinstance(target, DependentTypeName)
            self.target = DependentTypeName(lexer, position, self)
        else:
            self.target = target
        self.targets = targets or ((target, tuple(), target),)
        self.qualified = qualified
        self.dependent = dependent
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
                    data = other.data,
                    positions = self.positions + (other.position,))
    
    def __str__(self):
        return '::'.join(self.name)
