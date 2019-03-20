from .cppobject import CppObject

class Specifier(CppObject):
    def __init__(self, owner, position, specifier):
        CppObject.__init__(self, owner, position)
        self.specifier = specifier
