class Type:
    def __init__(self, name):
        self.type = name

    def add_modifier(self, modifier, position):
        self.type += ' %s' % modifier

    def __str__(self):
        return self.type


class TypeWriter:
    def __init__(self, document):
        self.document = document

    def builtin(self, typename, position):
        return Type(typename)

    def struct(self, struct_name, struct_id, position):
        return Type('struct_%d' % struct_id)

    def pointer(self, pointee, position):
        return Type(pointee.type + '*')

    def reference(self, pointee, position):
        return Type(pointee.type + '*')

    def array(self, pointee, count, position):
        if count:
            return Type(pointee.type + '[%d]'%count)
        else:
            return Type(pointee.type + '[]')
