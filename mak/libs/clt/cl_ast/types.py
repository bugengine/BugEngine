from .cppobject import CppObject
from .error import CppError
from .scope import Scope, ScopeError
from .name import Name


class CastError(CppError):
    pass




class CastOptions:
    CAST_NONE=0
    CAST_ATTRIB=1
    CAST_IMPLICIT=2
    CAST_STATIC=3
    CAST_UPCAST=4
    CAST_REINTERPRET=5
    CAST_UNRELATED=6
    def __init__(self, allowed_cast, template_parameter_matches={}, template_bindings=None, current_template=None):
        self.allowed_cast = allowed_cast
        self.template_parameter_matches = template_parameter_matches
        self.template_bindings = template_bindings
        self.current_template = current_template


class Type(CppObject):
    class Distance:
        def __init__(self, variant=0, cast=0, attribute=0, matches={}):
            self.distance = (variant, cast, attribute)
            self.matches = {}
            for a, m in matches.items():
                self.matches[a] = m

        def refine(self):
            if self.distance[0] > 0:
                self.distance = (self.distance[0]-1, self.distance[1], self.distance[2])
            return self

        def __bool__(self):
            return self.distance[0] >= 0 and self.distance[1] >= 0 and self.distance[2] >= 0
        
        def exact_match(self):
            return self.distance[0] == 0 and self.distance[1] == 0 and self.distance[2] == 0

        def match_attributes(self, allowed_cast, typeref_from, typeref_to):
            attrs1 = typeref_from.qualifiers
            attrs2 = typeref_to.qualifiers
            for a in attrs1:
                if a not in attrs2:
                    raise CastError("invalid cast from '%s' to '%s': cannot discard '%s' qualifier" % (typeref_from, typeref_to, a),
                                    typeref_to.position)
            cast_cost = 0
            for a in attrs2:
                if a not in attrs1:
                    cast_cost += 1
            if not allowed_cast:
                if cast_cost:
                    raise CastError("invalid cast from '%s' to '%s'" % (typeref_from, typeref_to), typeref_to.position)
            self.distance = (self.distance[0], self.distance[1], self.distance[2]+cast_cost)
            return self

        def __eq__(self, other):
            return self.distance == other.distance

        def __ne__(self, other):
            return self.distance != other.distance

        def __lt__(self, other):
            return self.distance < other.distance

        def __le__(self, other):
            return self.distance <= other.distance

        def __gt__(self, other):
            return self.distance > other.distance

        def __ge__(self, other):
            return self.distance >= other.distance
        
        def __iadd__(self, other):
            for k, v in other.matches.items():
                if k in self.matches:
                    d = v.distance(self.matches[k], CastOptions(CastOptions.CAST_NONE))
                    if d != type.Distance():
                        raise CastError('', ('', 0, 0, 0))
                else:
                    self.matches[k] = v
            self.distance = (self.distance[0] + other.distance[0],
                             self.distance[1] + other.distance[1],
                             self.distance[2] + other.distance[2],)
            return self
        
        def __repr__(self):
            return 'd[%s,%s,%s]' % self.distance

        def __str__(self):
            return 'variant=%d / attribute = %d / cast = %d' % (self.distance[0], self.distance[1], self.distance[2])

    def __init__(self, *args):
        CppObject.__init__(self, *args)

    def get_type(self):
        return self

    def get_unresolved_parameters(self):
        return []


class TypeDef(Type):
    class INITIAL_SCOPE(Scope):
        def find(self, name, position, source_context, is_current_scope):
            if is_current_scope:
                scope = self.owner.type.type.scope
                try:
                    return scope.find(name, position, source_context, is_current_scope)
                except CppError as e:
                    raise ScopeError("with '%s' defined as %s" % (self.owner.name, self.owner.type.type.pretty_name()), self.position, e)
            else:
                return None

    def __init__(self, lexer, position, name, type):
        Type.__init__(self, lexer, position, name)
        self.type = type

    def get_type(self):
        return self.type.get_type()

    def __str__(self):
        return str(self.type)

    def get_token_type(self):
        return 'TYPENAME_ID'

    def _create_template_instance(self, template, arguments, position):
        return TypeDef(self.lexer, self.position, self.name,
                       self.type.create_template_instance(template, arguments, position))

    def write_to(self, writer):
        pass
    

class Pointer(Type):
    def __init__(self, lexer, position, type):
        Type.__init__(self, lexer, position)
        self.type = type

    def __str__(self):
        return str(self.type)+'*'
    
    def _distance(self, cast_to, cast_options, typeref_from, typeref_to):
        if isinstance(cast_to, Pointer):
            allowed_cast = cast_options.allowed_cast
            if cast_options.allowed_cast in (cast_options.CAST_STATIC, cast_options.CAST_UPCAST):
                cast_options.allowed_cast = cast_options.CAST_UPCAST
            elif cast_options.allowed_cast in (cast_options.CAST_REINTERPRET, cast_options.CAST_UNRELATED):
                cast_options.allowed_cast = cast_options.CAST_UNRELATED
            d = self.type.distance(cast_to.type, cast_options).refine()
            return d.match_attributes(allowed_cast, typeref_from, typeref_to)
        elif cast_options.allowed_cast == cast_options.CAST_UNRELATED:
            return Type.Distance(variant=-1)
        else:
            raise CastError('type %s is not compatible with %s' % (typeref_from, typeref_to), self.position)

    def _create_template_instance(self, template, arguments, position):
        return Pointer(self.lexer, self.position,
                       self.type.create_template_instance(template, arguments, self.position))

    def signature(self):
        return 'P'+self.type.signature()


class Reference(Type):
    def __init__(self, lexer, position, type):
        Type.__init__(self, lexer, position)
        self.type = type

    def __str__(self):
        return str(self.type)+'&'

    def _distance(self, cast_to, cast_options, typeref_from, typeref_to):
        if isinstance(cast_to, Reference):
            allowed_cast = cast_options.allowed_cast
            if cast_options.allowed_cast in (cast_options.CAST_STATIC, cast_options.CAST_UPCAST):
                cast_options.allowed_cast = cast_options.CAST_UPCAST
            elif cast_options.allowed_cast in (cast_options.CAST_REINTERPRET, cast_options.CAST_UNRELATED):
                cast_options.allowed_cast = cast_options.CAST_UNRELATED
            d = self.type.distance(cast_to.type, cast_options).refine()
            return d.match_attributes(allowed_cast, typeref_from, typeref_to)
        elif cast_options.allowed_cast == cast_options.CAST_UNRELATED:
            return Type.Distance(variant=-1)
        else:
            raise CastError('type %s is not compatible with %s' % (typeref_from, typeref_to), self.position)

    def _create_template_instance(self, template, arguments, position):
        return Reference(self.lexer, self.position,
                         self.type.create_template_instance(template, arguments, self.position))

    def signature(self):
        return 'R'+self.type.signature()


class Array(Type):
    def __init__(self, lexer, position, type, size):
        Type.__init__(self, lexer, position)
        self.type = type
        self.size = size

    def __str__(self):
        return '%s[%s]' % (str(self.type), self.size or '')

    def _distance(self, cast_to, cast_options, typeref_from, typeref_to):
        if isinstance(cast_to, Array):
            allowed_cast = cast_options.allowed_cast
            if cast_options.allowed_cast in (cast_options.CAST_STATIC, cast_options.CAST_UPCAST):
                cast_options.allowed_cast = cast_options.CAST_UPCAST
            elif cast_options.allowed_cast in (cast_options.CAST_REINTERPRET, cast_options.CAST_UNRELATED):
                cast_options.allowed_cast = cast_options.CAST_UNRELATED
            d = self.type.distance(cast_to.type, cast_options).refine()
            return d.match_attributes(allowed_cast, typeref_from, typeref_to)
        elif cast_options.allowed_cast == cast_options.CAST_UNRELATED:
            return Type.Distance(variant=-1)
        else:
            raise CastError('type %s is not compatible with %s' % (typeref_from, typeref_to), self.position)

    def _create_template_instance(self, template, arguments, position):
        return Array(self.lexer, self.position,
                     self.type.create_template_instance(template, arguments, self.position),
                     self.size and self.size.create_template_instance(template, arguments, self.position))

    def signature(self):
        return 'P'+self.type.signature()


class StructScope(Scope):
    def __init__(self, owner, position, visibility, parent_visibility, parent):
        from .templates import TemplateTypenameParameter
        Scope.__init__(self, owner, position, visibility)
        if parent:
            assert isinstance(parent, Type)
            if not (isinstance(parent, Struct)
                 or isinstance(parent, DependentTypeName)
                 or isinstance(parent, TemplateTypenameParameter)):
                owner.lexer.error('expected struct name', position)
        self.parent_visibility = parent_visibility
        self.parent = parent
        self.constructor = None
        self.destructor = None
        self.casts = []
        self.operators = []

    def debug_dump(self, indent):
        if self.constructor:
            self.constructor.debug_dump(indent)
        if self.destructor:
            self.destructor.debug_dump(indent)
        #for _, cast in self.casts:
        #    cast.debug_dump(indent)
        Scope.debug_dump(self, indent)

    def find(self, name, position, source_context, is_current_scope):
        if is_current_scope and name == self.owner.name and self.constructor:
            return self.constructor
        if name == '~%s' % self.owner.name:
            return self.destructor
        try:
            result = Scope.find(self, name, position, source_context, is_current_scope)
        except ScopeError:
            if self.parent:
                return self.parent.scope.find(name, position, source_context, is_current_scope)
            else:
                raise
        else:
            return result or (self.parent and self.parent.scope.find(name, position, source_context, is_current_scope))


class Struct(Type):
    INITIAL_SCOPE = CppObject.NotDefinedScope

    global_index = 0

    def __init__(self, lexer, position, struct_type, name):
        Type.__init__(self, lexer, position, name)
        self.struct_type = struct_type
        self.default_visibility = (struct_type == 'class') and 'protected' or 'public'
        self.index = Struct.global_index
        Struct.global_index += 1

    def get_token_type(self):
        return 'STRUCT_ID'

    def define(self, parent_visibility, parent, position):
        if self.struct_type == 'union' and parent:
            self.lexer.error('unions cannot have a base class', position)
        self.push_scope_recursive(position, StructScope(self, position, self.default_visibility,
                                                        parent_visibility or self.default_visibility,
                                                        parent))

    def _create_template_instance(self, template, arguments, position):
        return Struct(self.lexer, self.position, self.struct_type, self.name)

    def _complete_template_instance(self, result, template, arguments, position):
        if self.scope:
            parent = self.scope.parent and self.scope.parent.create_template_instance(template, arguments, position)
            result.define(self.scope.parent_visibility, parent and parent.get_type(), position)
            try:
                self.scope.create_template_instance(result.scope, template, arguments, position)
            except Exception as e:
                result.pop_scope_recursive()
                raise
            else:
                result.pop_scope_recursive()
        return result

    def _distance(self, cast_to, cast_options, typeref_from, typeref_to):
        if isinstance(cast_to, Struct):
            if cast_options.allowed_cast <= cast_options.CAST_ATTRIB:
                if cast_to != self:
                    raise CastError('type %s is not compatible with %s' % (typeref_from, typeref_to), self.position)
                else:
                    d = Type.Distance()
                    return d.match_attributes(cast_options.allowed_cast, typeref_from, typeref_to)
            cast = 0
            variant = 0
            p = self
            while p:
                if p == cast_to:
                    break
                cast += 1
                p = p.scope and p.scope.parent
            else:
                if cast_options.allowed_cast >= cast_options.CAST_UPCAST:
                    cast = 0
                    p = cast_to
                    while p:
                        if p == self:
                            break
                        cast -= 1
                    else:
                        if cast_options.allowed_cast < cast_options.CAST_UNRELATED:
                            raise CastError('type %s is not compatible with %s' % (typeref_from, typeref_to), self.position)
                        variant = -1
                        cast = 0
                else:
                    raise CastError('type %s is not compatible with %s' % (typeref_from, typeref_to), self.position)
            d = Type.Distance(variant=variant, cast=cast)
            return d.match_attributes(cast_options.allowed_cast, typeref_from, typeref_to)
        elif cast_options.allowed_cast == cast_options.CAST_UNRELATED:
            return Type.Distance(variant = -1)
        else:
            raise CastError('type %s is not compatible with %s' % (typeref_from, typeref_to), self.position)

    def __str__(self):
        return self.name or '<anonymous>'

    def pretty_name(self):
        return "%s '%s'" % (self.struct_type, str(self))

    def signature(self):
        return '%s_%d' % (self.name or 'anonymous', self.index)

    def write_to(self, writer):
        with writer.create_struct(self.position, self.index, self.name,
                                  self.scope.parent_visibility,
                                  self.scope.parent and self.scope.parent.index) as struct:
            if self.scope:
                self.scope.write_to(struct)


class BuiltIn(Type):
    def __init__(self, lexer, position, builtin):
        Type.__init__(self, lexer, position)
        self.builtin = builtin

    def __str__(self):
        return self.builtin

    def _distance(self, other, cast_options, typeref, other_typeref):
        if isinstance(other, BuiltIn):
            if self.builtin != other.builtin:
                if cast_options.allowed_cast >= cast_options.CAST_IMPLICIT:
                    d = Type.Distance(cast=1)
                    return d.match_attributes(cast_options.allowed_cast, typeref, other_typeref)
                else:
                    raise CastError('type %s is not compatible with %s' % (self, other), self.position)
            else:
                d = Type.Distance(cast=0)
                return d.match_attributes(cast_options.allowed_cast, typeref, other_typeref)
        elif cast_options.allowed_cast == cast_options.CAST_UNRELATED:
            d = Type.Distance(variant = -1)
            return d.match_attributes(cast_options.allowed_cast, typeref, other_typeref)
        else:
            raise CastError('type %s is not compatible with %s' % (self, other), self.position)

    def pretty_name(self):
        return "builtin type '%s'" % self.builtin

    def _create_template_instance(self, template, arguments, position):
        return self
    
    def signature(self):
        return '%d%s' % (len(self.builtin), self.builtin)


class Void(Type):
    def __init__(self, lexer, position):
        Type.__init__(self, lexer, position)

    def __str__(self):
        return 'void'

    def _distance(self, other, cast_options, typeref, other_typeref):
        if isinstance(other, Void):
            d = Type.Distance(cast=0)
            return d.match_attributes(cast_options.allowed_cast, typeref, other_typeref)
        elif cast_options.allowed_cast == cast_options.CAST_UNRELATED:
            d = Type.Distance(variant = -1)
            return d.match_attributes(cast_options.allowed_cast, typeref, other_typeref)
        else:
            raise CastError('type %s is not compatible with %s' % (self, other), self.position)

    def _create_template_instance(self, template, arguments, position):
        return self

    def signature(self):
        return '4void'

    def pretty_name(self):
        return "builtin type 'void'"


class DependentTypeName(Type):
    class INITIAL_SCOPE(Scope):
        def find(self, name, position, source_context, is_current_scope):
            return None

    def __init__(self, lexer, position, name):
        Type.__init__(self, lexer, position)
        self.name = name

    def __eq__(self, other):
        return isinstance(other, DependentTypeName) and self.name == other.name

    def __str__(self):
        return 'typename(%s)'%self.name

    def get_unresolved_parameters(self):
        result = []
        n = self.name
        while n:
            if n.target and n.target != self:
                result += n.target.get_unresolved_parameters()
            for a in n.arguments or []:
                result += a.get_unresolved_parameters()
            if n.template:
                result += n.template.get_unresolved_parameters()
            n = n.parent
        return result

    def _resolve(self, name, instance_template, instance_arguments, instance_position):
        from .templates import Template
        if name.parent:
            current_object = self._resolve(name.parent, instance_template, instance_arguments, instance_position)
            if current_object:
                try:
                    result = current_object.scope.find(name.name, name.position, self, True)
                except CppError as e:
                    raise Template.InstantiationError(e.message, e.position, e.inner_error)
            else:
                return None
        else:
            _, result = self.lexer.lookup_by_name(name.name, name.position)
        if result:
            result = result.create_template_instance(instance_template, instance_arguments, instance_position)
            if name.arguments:
                try:
                    result = result.instantiate([a.create_template_instance(instance_template, instance_arguments, instance_position) for a in name.arguments], name.position)
                except Template.InstantiationError as e:
                    raise Template.InstantiationError("in instantiation of template '%s' requested here"%name, name.position, e)
        return result

    def _create_partial_template_instance(self, template, arguments, position):
        def create_name(n):
            if n:
                return Name(n.lexer, n.name, n.position,
                            target=n.target and n.target.create_template_instance(template, arguments, position),
                            template=n.template and n.template.create_template_instance(template, arguments, position),
                            arguments=[a.create_template_instance(template, arguments, position) for a in n.arguments or []],
                            parent=create_name(n.parent))
        self.name.target = None
        name = create_name(self.name)
        name.target = DependentTypeName(self.lexer, self.position, name)
        return name.target

    def _create_template_instance(self, template, arguments, position):
        from .templates import Template, TemplateScope
        result = self._resolve(self.name, template, arguments, position)
        if not result:
            for s in self.lexer.owner_scopes[::-1]:
                if isinstance(s, TemplateScope) and s.owner != template:
                    return self._create_partial_template_instance(template, arguments, position)
                elif not s.owner.parent:
                    raise Template.InstantiationError('no', position)
            else:
                assert False
        return result

    def simplify(self):
        from .templates import Template
        def _simplify(name):
            if not name.dependent:
                return name.target
            else:
                if name.parent:
                    result = _simplify(name.parent)
                    if result:
                        try:
                            result = result.scope.find(name.name, name.position, self, True)
                        except CppError as e:
                            self.lexer.error(e.message, e.position)
                    else:
                        return None
                else:
                    _, result = self.lexer.lookup_by_name(name.name, name.position)
                if name.arguments is not None:
                    result = result.find_instance(name.template_bindings and name.template_bindings.parameter_binds,
                                                  name.arguments, name.position)
                return result
        try:
            return _simplify(self.name) or self
        except Template.InstantiationError as e:
            def _print_exception(e):
                if e.inner_error:
                    _print_exception(e.inner_error)
                    self.lexer.note(e.message, e.position)
                else:
                    self.lexer.error(e.message, e.position)
            _print_exception(e)
            return self

    def _distance(self, other, cast_options, typeref, other_typeref):
        if isinstance(other, DependentTypeName):
            if not self.name.equals(other.name, cast_options.template_bindings):
                if cast_options.allowed_cast == cast_options.CAST_UNRELATED:
                    d = Type.Distance(variant = -1)
                    return d.match_attributes(cast_options.allowed_cast, typeref, other_typeref)
                else:
                    raise CastError('type %s is not compatible with %s' % (self, other), self.position)
            else:
                d = Type.Distance(cast=0)
                return d.match_attributes(cast_options.allowed_cast, typeref, other_typeref)
        elif cast_options.allowed_cast == cast_options.CAST_UNRELATED:
            d = Type.Distance(variant = -1)
            return d.match_attributes(cast_options.allowed_cast, typeref, other_typeref)
        else:
            raise CastError('type %s is not compatible with %s' % (self, other), self.position)
    
    def signature(self):
        return '<%s>'%self.name


class TypeRef(CppObject):
    def __init__(self, lexer, position, type, template=None):
        CppObject.__init__(self, lexer, position)
        self.type = type
        self.qualifiers = set([])
        self.template_origin = template

    def get_type(self):
        return self.type.get_type()

    def is_void(self):
        return isinstance(self.type.get_type(), Void)

    def add_qualifier(self, qualifier):
        self.qualifiers.add(qualifier)

    def clone(self):
        result = TypeRef(self.lexer, self.position, self.type, self.template_origin)
        for q in self.qualifiers:
            result.qualifiers.add(q)
        return result

    def instantiate(self, *args, **kw):
        assert self.template_origin
        self.template_origin.instantiate(*args, **kw)

    def distance(self, other, cast_options):
        from .templates import TemplateTypenameParameter, TemplateTemplateParameter
        if isinstance(self.type, TypeDef):
            return self.type.type.distance(other, cast_options)
        elif isinstance(other, TypeRef):
            if isinstance(other.type, TypeDef):
                return self.distance(other.type.type, cast_options)
            elif isinstance(other.type, TemplateTypenameParameter):
                return other.type.template_parameter_match(self.type, cast_options, self, other) 
            else:
                return self.type._distance(other.type, cast_options, self, other)
        elif isinstance(other, TemplateTemplateParameter):
            return other.template_parameter_match(self, cast_options, self, other) 
        else:
            raise CastError('type %s is not compatible with %s' % (self, other), self.position)

    def _create_template_instance(self, template, arguments, position):
        type = self.type.create_template_instance(template, arguments, position)
        if isinstance(type, TypeRef):
            result = type.clone()
            result.qualifiers |= self.qualifiers
        else:
            result = self.clone()
            result.type = type
        return result

    def __eq__(self, other):
        if self.__class__ != other.__class__:
            return False
        return self.type == other.type and self.qualifiers == other.qualifiers

    def __str__(self):
        return str(self.type) + (self.qualifiers and ' '+' '.join(self.qualifiers) or '')
    __repr__ = __str__
    pretty_name = __str__

    def get_unresolved_parameters(self):
        if self.type:
            return self.type.get_unresolved_parameters()
        else:
            return self.template_origin.get_unresolved_parameters()

    def signature(self):
        s = self.type.signature()
        for a in self.qualifiers:
            if a == 'const':
                s = 'K'+s
            elif a == 'volatile':
                s = 'V'+s
            else:
                assert False
        return s
