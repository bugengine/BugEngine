from py_bugengine import *
import ctypes

p=Plugin("sample.python")
print(p.BugEngine.TestCases.Enum.Value1)
print(p.BugEngine.TestCases.Enum.Value2)
print(p.BugEngine.TestCases.Enum.Value3)

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
        for overload in method.overloads:
            param_list = []
            for param in overload.params:
                param_list.append((name(param.type), param.name))
            print('  %s %s (%s)'%(name(overload.returnType), method.name, ', '.join(('%s %s'%p for p in param_list))))

    print('class %s' % klass.name)
    if klass.constructor:
        print('List of constructors:')
        print_method(klass.constructor)
    print('List of methods:')
    for method in klass.methods:
        print_method(method)
    print('List of properties:')
    for property in klass.properties:
        print(' ', name(property.type), property.name)
    print('List of objects:')
    object = klass.objects
    while object:
        print(' ', object.name)
        object = object.next

if __name__ == '__main__':
    help(BugEngine.RTTI.Class.ClassType.metaclass)
    help(BugEngine.RTTI.Class)
    help(BugEngine.text)
    help(BugEngine.DiskFolder)

    sample = Plugin('sample.python')
    c = sample.TestCases.Class(y1=1, x1=3)
    c.doStuff(1, 2, True)
    print('%d - %d' % (c.x1, c.y1))

