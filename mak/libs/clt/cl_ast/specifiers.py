class Specifier:
    def __init__(self, lexer, position, specifier):
        self.lexer = lexer
        self.position = position
        self.specifier = specifier
    
    def __str__(self):
        return self.specifier