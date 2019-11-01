from .scope import Scope, ScopeError


class CppObject:
    class NoContainerScope(Scope):
        def find(self, name, position, source_context, is_current_scope):
            if is_current_scope:
                raise ScopeError("%s is not a class, namespace or enumeration" % (self.owner.pretty_name()), position)
            else:
                return None

    class NotDefinedScope(Scope):
        def __init__(self, *args, **kw):
            Scope.__init__(self, *args, **kw)
            self.parent = None
            self.parent_visibility = 'public'

        def find(self, name, position, source_context, is_current_scope):
            if is_current_scope:
                raise ScopeError("forward declaration of %s" % (self.owner.pretty_name()), self.owner.position,
                                 ScopeError("incomplete type '%s' named in nested name specifier" % (self.owner.name), position))
            else:
                return None

    INITIAL_SCOPE = NoContainerScope
    def __init__(self, lexer, position, name=None):
        from .templates import Template
        self.lexer = lexer
        self.position = position
        self.parent_scope = self.lexer.scopes and self.lexer.scopes[-1] or None
        self.parent = self.parent_scope and self.parent_scope.owner
        self.name = name
        self.scope = self.INITIAL_SCOPE(self, self.position)
        self.templates = []
        parent = self.parent
        while parent:
            if isinstance(parent, Template):
                self.templates.append(parent.back_link)
            parent = parent.parent
        self.instances = []

    def create_template_instance(self, template, arguments, position):
        if template not in [t.back_link for t in self.templates]:
            return self
        instance = self._get_cached_instance(arguments)
        if not instance:
            instance = self._create_template_instance(template, arguments, position)
            self.instances.append((arguments, position, template, instance))
            self._complete_template_instance(instance, template, arguments, position)
        return instance

    def seal(self):
        pass
        #for arguments, position, template, instance in self.instances:
        #    self._complete_template_instance(instance, template, arguments, position)

    def _get_cached_instance(self, arguments):
        for args, _, _, instance in self.instances:
            assert len(args) == len(arguments)
            if CppObject.equal_parameters(args, arguments, { }):
                return instance
        return None
    
    def _complete_template_instance(self, result, template, arguments, position):
        pass

    def pretty_name(self):
        return "'%s'" % (self.name or '<anonymous object>')

    def register(self):
        self.parent_scope.add(self)

    def push_scope(self, position, scope = None):
        if isinstance(self.scope, self.INITIAL_SCOPE):
            self.scope = scope or Scope(self, position)
        elif scope and scope != self.scope:
            self.lexer.error('redefinition of object %s'%self.name, position)
            self.lexer.note('first defined here', self.scope.position)
            self.lexer.note('first declared here', self.position)
            self.scope = scope
        self.lexer.push_scope(self.scope)

    def push_scope_recursive(self, position, scope=None):
        if self.parent:
            self.parent.push_scope_recursive(position)
        self.push_scope(position, scope or self.scope)
        
    def pop_scope_recursive(self):
        self.lexer.pop_scope(self.scope)
        if self.parent:
            self.parent.pop_scope_recursive()

    def _error(self, message):
        self.lexer.error(message, self.position)

    def _note(self, message):
        self.lexer.note(message, self.position)

    def find(self, name):
        if self.name == name:
            return self
        else:
            return None

    def get_token_type(self):
        assert False, '%s should redefine get_token_type'%self

    def get_token_type_raw(self):
        return self.get_token_type()

    def debug_dump(self, indent=''):
        print('%s%s%s [%s]' % (indent, self.__class__.__name__,
                               self.name and (' %s'%self.name) or '',
                               self.position))
        self.scope.debug_dump(indent)

    def simplify(self):
        return self

    @classmethod
    def equal_parameters(self, parameters1, parameters2, bindings):
        from . import types
        if len(parameters1) != len(parameters2):
            return False
        for i in range(0, len(parameters1)):
            if not parameters1[i]:
                return False
            if not parameters2[i]:
                return False
            try:
                d = parameters2[i].distance(parameters1[i], types.CAST_NONE, template_bindings=bindings)
            except types.CastError:
                return False
            else:
                if not d.exact_match():
                    return False
        else:
            return True
