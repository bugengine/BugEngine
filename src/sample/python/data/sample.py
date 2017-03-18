from py_bugengine import *

def name(type):
    constness = int(type.constness) == 0 and 'const ' or ''
    access = int(type.access) == 0 and 'const ' or ''
    if type.indirection == 0:
        return '%s%s' % (access, type.metaclass.name)
    else:
        if type.indirection == 1:
            ptr = 'raw'
        elif type.indirection == 2:
            ptr = 'weak'
        elif type.indirection == 3:
            ptr = 'ref'
        else:
            ptr = '???'
        return '%s%s<%s%s>' % (constness,
                               ptr,
                               access,
                               type.metaclass.name)


def help(klass):
    def print_method(method):
        overload = method.overloads
        while overload:
            param_list = []
            param = overload.params
            while param:
                param_list.append((name(param.type), param.name))
                param = param.next
            print('  %s %s (%s)'%(name(overload.returnType), method.name, ', '.join(('%s %s'%p for p in param_list))))
            overload = overload.next

    print('class %s' % klass.name)
    if klass.constructor:
        print('List of constructors:')
        print_method(klass.constructor)
    print('List of methods:')
    method = klass.methods
    while method:
        print_method(method)
        method = method.next
    print('List of properties:')
    property = klass.properties
    while property:
        print(' ', name(property.type), property.name)
        property = property.next
    print('List of objects:')
    object = klass.objects
    while object:
        print(' ', object.name)
        object = object.next

if __name__ == '__main__':
    help(BugEngine.RTTI.Class.ClassType.metaclass)
    help(BugEngine.text)
    help(BugEngine.DiskFolder)

