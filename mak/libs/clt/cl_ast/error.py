class CppError(Exception):
    def __init__(self, error_message, position, inner_error=None):
        self.message = error_message
        self.position = position
        self.inner_error = inner_error
