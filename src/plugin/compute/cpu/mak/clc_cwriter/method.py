class Method:
    def __init__(self, position, method_id, method_name, overload_id):
        self.position = position
        self.id = method_id
        self.name = method_name
        self.overload = overload_id

    def __enter__(self):
        return self

    def __exit__(self, type, value, traceback):
        pass
