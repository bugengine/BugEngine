from py_bugengine import BugEngine

def help(klass):
    print('class %s' % klass.name)
    print('List of methods:')
    method = klass.methods
    while method:
        overload = method.overloads
        while overload:
            param_list = []
            param = overload.params
            while param:
                param_list.append((param.type.metaclass.name, param.name))
                param = param.next
            print('  %s %s (%s)'%(overload.returnType.metaclass.name, method.name, ', '.join(('%s %s'%p for p in param_list))))
            overload = overload.next
        method = method.next
    print('List of properties:')
    property = klass.properties
    while property:
        print(' ', property.type.metaclass.name, property.name)
        property = property.next
help(BugEngine.RTTI.Class)
help(BugEngine.RTTI.Type)

t = BugEngine.RTTI.Type.makeType(BugEngine.RTTI.Class,
                                 BugEngine.RTTI.Type.RawPtr,
                                 BugEngine.RTTI.Type.Const,
                                 BugEngine.RTTI.Type.Const)
