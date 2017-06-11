function name(type)
    local constness = tonumber(type.constness) == 0 and 'const ' or ''
    local access = tonumber(type.access) == 0 and 'const ' or ''
    local ptr
    if type.indirection == 0 then
        return(tostring("%s%s"):format(access, type.metaclass.name))
    else
        if type.indirection == 1 then
            ptr = "raw"
        elseif type.indirection == 2 then
            ptr = "weak"
        elseif type.indirection == 3 then
            ptr = "ref"
        else
            ptr = "???"
        end
        return(tostring("%s%s<%s%s>"):format(constness,
                                   ptr,
                                   access,
                                   type.metaclass.name))
    end
end

function help(klass)
    function print_method(method)
        local overload = method.overloads
        while overload do
            local param_list = {}
            local param = overload.params
            while param do
                param_list[#param_list+1] = tostring('%s %s'):format(name(param.type), param.name)
                param = param.next
            end
            print(tostring('  %s %s (%s)'):format(name(overload.returnType),
                                                  method.name,
                                                  table.concat(param_list, ', ')))
            overload = overload.next
        end
    end

    print(tostring('class %s'):format(klass.name))
    if klass.constructor then
        print('List of constructors:')
        print_method(klass.constructor)
    end
    print('List of methods:')
    local method = klass.methods
    while method do
        print_method(method)
        method = method.next
    end
    print('List of properties:')
    local property = klass.properties
    while property do
        print(tostring(' %s %s'):format(name(property.type), property.name))
        property = property.next
    end
    print('List of objects:')
    local object = klass.objects
    while object do
        print(tostring(' %s'):format(object.name))
        object = object.next
    end
end

help(BugEngine.RTTI.Class.ClassType.metaclass)
help(BugEngine.text)
help(BugEngine.DiskFolder)
