local function help(klass)
    print(string.format("class %s", klass.name))
    print("List of methods:")
    method = klass.methods
    while method do
        overload = method.overloads
        while overload do
            param = overload.params
            param_str = ''
            while param do
                param_str = param_str .. tostring(param.type.metaclass.name) .. ' ' .. tostring(param.name)
                param = param.next
                if param then
                    param_str = param_str .. ', '
                end
            end
            print(string.format('  %s %s (%s)', overload.returnType.metaclass.name, method.name, param_str))
            overload = overload.next
        end
        method = method.next
    end
    print('List of properties:')
    property = klass.properties
    while property do
        print(string.format(' %s %s', property.type.metaclass.name, property.name))
        property = property.next
    end
    print('List of objects:')
    object = klass.objects
    while object do
        print(string.format(' %s', object.name))
        object = object.next
    end
end

help(BugEngine.RTTI.Class)
