from .cppobject import CppObject
from .scope import Scope
from .types import TypeRef, Type, BuiltIn, CAST_NONE, CAST_ATTRIB, CAST_UNRELATED, CastError


class TemplateValueParameter(CppObject):
    def __init__(self, lexer, position, name, type, value):
        CppObject.__init__(self, lexer, position, name)
        self.type = type
        self.value = value
        self.parameter_bind = None

    def get_parameter(self):
        return self.parameter_bind and self.parameter_bind[1] or self

    def is_bound(self):
        return self.parameter_bind != None

    def bind(self, argument_position, template):
        self.parameter_bind = (argument_position, template)

    def get_token_type(self):
        return 'VARIABLE_ID'

    def __str__(self):
        return '%s%s' % (self.type, self.name and ' '+self.name or '')

    def get_type(self):
        return self.type.name()

    def get_template_param_dependencies(self):
        return [self]

    def _create_template_instance(self, template, arguments, position):
        if template == self.parameter_bind[1]:
            return arguments[self.parameter_bind[0]]
        else:
            template_parent = self.lexer.scopes[-1].owner
            assert isinstance(template_parent, Template)
            value = self.value and self.value.create_template_instance(template, arguments, position)
            type = self.type.create_template_instance(template, arguments, position)
            result = TemplateValueParameter(self.lexer, position, self.name, type, value)
            result.bind(self.parameter_bind[0], template_parent)
            return result


class TemplateTypenameParameter(Type):
    def __init__(self, lexer, position, name, value):
        Type.__init__(self, lexer, position, name)
        self.value = value
        self.parameter_bind = None

    def get_parameter(self):
        return self.parameter_bind and self.parameter_bind[1] or self

    def is_bound(self):
        return self.parameter_bind != None

    def bind(self, argument_position, template):
        self.parameter_bind = (argument_position, template)

    def get_token_type(self):
        return 'TYPENAME_ID'

    def __str__(self):
        return 'typename%s' % (self.name and ' '+self.name or '')

    def get_type(self):
        return 'typename'
    
    def is_compatible(self, argument):
        return isinstance(argument, TypeRef)

    def get_template_param_dependencies(self):
        return [self]

    def _distance(self, other, matches, typeref, other_typeref, allowed_cast):
        if self.parameter_bind[0] in matches:
            new_match = matches[self.parameter_bind[0]]
            for a in typeref.qualifiers:
                #assert a not in new_match.qualifiers
                if a in new_match.qualifiers:
                    raise CastError
                new_match.qualifiers.add(a)
            return new_match.distance(other_typeref, CAST_NONE, matches)
        else:
            match = other_typeref.clone()
            d = Type.Distance(variant=100000, matches={self.parameter_bind[0]: match})
            for a in typeref.qualifiers:
                if a in other_typeref.qualifiers:
                    match.qualifiers.remove(a)
                    d = d.refine()
                elif allowed_cast == CAST_UNRELATED:
                    return Type.Distance(variant=-1)
                else:
                    raise CastError()
            return d

    def _create_template_instance(self, template, arguments, position):
        if template == self.parameter_bind[1]:
            return arguments[self.parameter_bind[0]]
        else:
            template_parent = self.parameter_bind[1].create_template_instance(template, arguments, position)
            value = self.value and self.value.create_template_instance(template, arguments, position)
            result = TemplateTypenameParameter(self.lexer, position, self.name, value)
            result.bind(self.parameter_bind[0], template_parent)
            return result


class TemplateScope(Scope):
    def __init__(self, owner):
        Scope.__init__(self, owner)
        self.parameters = []

    def add(self, element):
        assert self.owner.back_link == self.owner
        if isinstance(element, TemplateValueParameter) or isinstance(element, TemplateTypenameParameter):
            self.parameters.append(element)
            self.owner.add_parameter(element)
        else:
            Scope.add(self, element)

    def find(self, name, is_current_scope):
        for element in self.parameters:
            result = element.find(name)
            if result:
                return result
        else:
            if self.owner.back_link != self.owner:
                return self.owner.back_link.scope.find(name, is_current_scope)
            else:
                return Scope.find(self, name, is_current_scope)


class Template(CppObject):
    class InstantiationError(Exception):
        def __init__(self, position, error_msg, inner_error=None):
            Exception.__init__(self)
            self.message = error_msg
            self.position = position
            self.error = inner_error

    def __init__(self, lexer, position):
        CppObject.__init__(self, lexer, position)
        self.back_link = self
        self.siblings = []
        self.parameters = []
        self.specializations = []
        self.push_scope(TemplateScope(self))

    def add_parameter(self, parameter):
        if isinstance(parameter, TemplateTypenameParameter):
            self.parameters.append(TypeRef(self.lexer, parameter.position, parameter))
        else:
            self.parameters.append(parameter)

    def get_token_type_raw(self):
        if self.back_link == self:
            assert self.scope
            assert self.scope[0]
            return self.scope[0][1].get_token_type_raw()
        else:
            return self.back_link.get_token_type_raw()

    def get_token_type(self):
        if self.back_link == self:
            assert self.scope
            assert self.scope[0]
            return 'TEMPLATE_' + self.scope[0][1].get_token_type_raw()
        else:
            return self.back_link.get_token_type_raw()

    def bind(self, template):
        assert self.back_link == self
        self.back_link = template
        if self.back_link != self:
            self.siblings.append(template)
        for i, p in enumerate(self.scope.parameters):
            p.bind(i, template)

    def find(self, name):
        if self.scope and self.scope.items:
            return self.scope[0][1].find(name) and self or None
        elif self.back_link != self:
            return self.back_link.find(name)
        else:
            return None

    def make_match(self, arguments):
        result = {}
        for i, a in enumerate(arguments):
            result[i] = a
        return result

    def instantiate(self, arguments, position):
        #self.lexer._note('creating instance of template %s'%self.scope[0][1].name, position)
        matches, specialization = self.find_specialization(position, None, arguments) or (self.make_match(arguments), self.scope[0][1])
        return specialization.create_template_instance(self, matches, position)

    def find_instance(self, template_on_stack, arguments, position):
        arguments = self.match(arguments, position)
        assert len(arguments) == len(self.parameters)
        if template_on_stack:
            template_on_stack.bind(self)
            for i, p in enumerate(arguments):
                if not isinstance(p, TypeRef):
                    break
                if not (isinstance(p.type, TemplateTypenameParameter)
                     or isinstance(p.type, TemplateValueParameter)):
                    break
                if p.qualifiers:
                    break
                if p.type not in template_on_stack.scope.parameters:
                    break
                if p.type.parameter_bind[0] != i:
                    break
            else:
                return self.scope[0][1]
            return self.find_exact_specialization(position, template_on_stack, arguments)
        else:
            return self.instantiate(arguments, position)

    def _create_template_instance(self, template, arguments, position):
        assert self == self.back_link
        return Template(self.lexer, position)

    def _complete_template_instance(self, result, template, arguments, position):
        parameters = [p.create_template_instance(template, arguments, position) for p in self.scope.parameters]
        for p in parameters:
            result.scope.add(p)
        for s in self.siblings:
            for i, p in enumerate(s.scope.parameters):
                p.create_template_instance(template, arguments, position).bind(i, result)
        if self.scope:
            result.scope.add(self.scope[0][1].create_template_instance(template, arguments, position))
        for specialization_parameters, specialization in self.specializations:
            params = [p.create_template_instance(template, arguments, position) for p in specialization_parameters]
            result.specializations.append((params, specialization.create_template_instance(template, arguments, position)))
        self.lexer.pop_scope()
        return result

    def create_specialization(self, arguments, specialization):
        assert self == self.back_link
        self.specializations.append((arguments, specialization))

    def find_exact_specialization(self, position, template_on_stack, arguments):
        for specialization_arguments, specialization in self.specializations:
            try:
                matches, scores = self.argument_match(specialization_arguments, arguments)
            except CastError:
                pass
            else:
                for score in scores:
                    if score == Type.Distance():
                        return None
                else:
                    return specialization

    def find_specialization(self, position, template_on_stack, arguments):
        specializations = []
        for specialization_arguments, specialization in self.specializations:
            try:
                matches, score = self.argument_match(specialization_arguments, arguments)
            except CastError:
                pass
            else:
                specializations.append((score, matches, specialization))
        if specializations:
            specializations = sorted(specializations, key=lambda x: x[0])
            score, matches, result = specializations[0]
            if len(specializations) > 1 and specializations[0][0] == specializations[1][0]:
                args = ', '.join(str(x) for x in arguments)
                self.lexer._error('ambiguous partial specializations of %s<%s>' % (self.scope[0][1].name, args),
                                  position)
                for s, m, r in specializations:
                    if s == score:
                        match_str = ', '.join('%s = %s' % (r.parent.scope.parameters[i].name or '<anonymous>', v) for i,v in sorted(m.items()))
                        self.lexer._note('partial specialization matches [%s]'%match_str, r.position)
                    else:
                        break
            return matches, result
        return None

    def argument_match(self, parameters, arguments):
        assert len(arguments) == len(parameters)
        matches = { }
        result = [ ]
        for a, p in zip(arguments, parameters):
            score = p.distance(a, CAST_NONE, matches)
            for k, v in score.matches.items():
                assert k not in matches
                matches[k] = v
            result.append(score)
        return matches, result

    def debug_dump(self, indent=''):
        if self.scope and not self.scope.empty():
            params = ', '.join(str(p) for p in self.scope.parameters)
            print('%s%s<%s> [%s]' % (indent, self.__class__.__name__,
                                    params, self.position))
            self.scope[0][-1].debug_dump(indent)
            for args, pos, _, instance in self.scope[0][-1].instances:
                print('%s<%s> [%s]' % (indent, ', '.join(str(a) for a in args), pos))
                instance.debug_dump(indent + '* ')
        for specialization_arguments, specialization in self.specializations:
            params = ', '.join(str(p) for p in specialization_arguments)
            print('%sspecialization: %s<%s> [%s]' % (indent, self.__class__.__name__,
                                        params, self.position))
            specialization.debug_dump(indent+' | ')
            for args, pos, _, instance in specialization.instances:
                print('%s<%s> [%s]' % (indent+' | ', ', '.join(str(a) for a in args), pos))
                instance.debug_dump(indent + ' | * ')
            print(indent + ' `-- end specialization')

    def match(self, arguments, position):
        if len(arguments) > len(self.scope.parameters):
            raise self.InstantiationError(arguments[len(self.scope.parameters)].position,
                                          'Too many template arguments')
        for missing_parameter in self.scope.parameters[len(arguments):]:
            if not missing_parameter.value:
                raise self.InstantiationError(position, 'too few template arguments')
            arguments.append(missing_parameter.value)
        for p, a in zip(self.scope.parameters, arguments):
            if not p.is_compatible(a):
                raise self.InstantiationError(a.position,
                                              'Invalid template argument: expected %s, got %s' % (p.get_type(),
                                                                                                  a.__class__.__name__))
        return arguments
