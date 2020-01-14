from .scope import Scope, ScopeError
from .argument_list import ArgumentList
from .position import Position
from typing import cast, List, Tuple, TypeVar, TYPE_CHECKING
from abc import abstractmethod

TemplateCacheList = List[Tuple['ArgumentList', 'Position', 'Template', 'CppObject']]
T = TypeVar('T', bound='CppObject')


class CppObject(object):
    class NoContainerScope(Scope):
        def find(self, name, position, source_context, is_current_scope):
            # type: (str, Position, Optional[CppObject], bool) -> Optional[CppObject]
            if is_current_scope:
                raise ScopeError("%s is not a class, namespace or enumeration" % (self.owner.pretty_name()), position)
            else:
                return None

    class NotDefinedScope(Scope):
        parent = None  # type: Optional[Scope]
        parent_visibility = 'public'

        def find(self, name, position, source_context, is_current_scope):
            # type: (str, Position, CppObject, bool) -> Optional[CppObject]
            if is_current_scope:
                raise ScopeError(
                    "forward declaration of %s" % (self.owner.pretty_name()), self.owner.position,
                    ScopeError("incomplete type '%s' named in nested name specifier" % (self.owner.name), position)
                )
            else:
                return None

    INITIAL_SCOPE = NoContainerScope   # type: ClassVar[Type[Scope]]

    def __init__(self, lexer, position, name=None):
        # type: (ClLexer, Position, Optional[str]) -> None
        from .ast_templates import Template
        self.lexer = lexer
        self.position = position
        self.name = name
        self.parent_scope = self.lexer.scopes and self.lexer.scopes[-1] or None # type: Optional[Scope]
        self.parent = self.parent_scope and self.parent_scope.owner or self     # type: CppObject
        self.scope = self.INITIAL_SCOPE(self, self.position)                    # type: Scope
        self.templates = []                                                     # type: List[Template]
        self.instances = []                                                     # type:  TemplateCacheList
        parent = self.parent
        while id(parent) != id(parent.parent):
            if isinstance(parent, Template):
                self.templates.append(parent.back_link)
            parent = parent.parent

    def create_template_instance(self, template, arguments, position):
        # type: (T, Template, ArgumentList, Position) -> T
        if template not in [t.back_link for t in self.templates]:
            return self
        instance = self._get_cached_instance(arguments)
        if not instance:
            instance = self._create_template_instance(template, arguments, position)
            self.instances.append((arguments, position, template, instance))
            self._complete_template_instance(instance, template, arguments, position)
        return instance

    @abstractmethod
    def _create_template_instance(self, template, arguments, position):
        # type: (T, Template, ArgumentList, Position) -> T
        raise NotImplementedError

    def seal(self):
        # type: () -> None
        pass
        # for arguments, position, template, instance in self.instances:
        #    self._complete_template_instance(instance, template, arguments, position)

    def _get_cached_instance(self, arguments):
        # type: (T, ArgumentList) -> Optional[T]
        s = arguments.signature()
        for args, _, __, instance in self.instances:
            #assert len(args) == len(arguments)
            if s == args.signature():
                return cast(T, instance)
        return None

    def _complete_template_instance(self, result, template, arguments, position):
        # type: (T, T, Template, ArgumentList, Position) -> None
        pass

    def pretty_name(self):
        # type: () -> str
        return "'%s'" % (self.name or '<anonymous object>')

    def register(self):
        # type: () -> None
        assert self.parent_scope
        self.parent_scope.add(self)

    def push_scope(self, position, scope=None, owner_scope=True):
        # type: (Position, Optional[Scope], bool) -> None
        if isinstance(self.scope, self.INITIAL_SCOPE):
            self.scope = scope or Scope(self, position)
        elif scope and scope != self.scope:
            self.lexer.error('redefinition of object %s' % self.name, position)
            self.lexer.note('first defined here', self.scope.position)
            self.lexer.note('first declared here', self.position)
            self.scope = scope
        self.lexer.push_scope(self.scope, owner_scope)

    def push_scope_recursive(self, position, scope=None, owner_scope=True):
        # type: (Position, Optional[Scope], bool) -> None
        if id(self.parent) != id(self):
            self.parent.push_scope_recursive(position, owner_scope=False)
        self.push_scope(position, scope or self.scope, owner_scope)

    def pop_scope(self):
        # type: () -> None
        self.lexer.pop_scope(self.scope)

    def pop_scope_recursive(self):
        # type: () -> None
        self.lexer.pop_scope(self.scope)
        if id(self.parent) != id(self):
            self.parent.pop_scope_recursive()

    def _error(self, message):
        # type: (str) -> None
        self.lexer.error(message, self.position)

    def _note(self, message):
        # type: (str) -> None
        self.lexer.note(message, self.position)

    def find(self, name):
        # type: (str) -> Optional[CppObject]
        if self.name == name:
            return self
        else:
            return None

    def get_token_type(self):
        # type: () -> str
        raise NotImplementedError

    def get_token_type_raw(self):
        # type: () -> str
        return self.get_token_type()

    def debug_dump(self, indent=''):
        # type: (str) -> None
        print('%s%s%s [%s]' % (indent, self.__class__.__name__, self.name and (' %s' % self.name) or '', self.position))
        self.scope.debug_dump(indent)

    def signature(self, template_bindings={}):
        # type: (Dict[BaseTemplateParameter, Tuple[int, Template]]) -> str
        return '?%s' % id(self)

    @abstractmethod
    def get_unresolved_parameters(self):
        # type: () -> List[BaseTemplateParameter]
        raise NotImplementedError

    @abstractmethod
    def write_to(self, writer):
        # type: (ClDocumentWriter) -> None
        raise NotImplementedError

    def distance(self, other, cast_options):
        # type: (CppObject, CastOptions) -> AstType.Distance
        raise NotImplementedError

    def is_compatible(self, other):
        # type: (CppObject) -> bool
        raise NotImplementedError


if TYPE_CHECKING:
    from typing import Dict, Optional, Type, ClassVar
    from ..cl_lexer import ClLexer
    from ..cl_document_writer import ClDocumentWriter
    from .type import Type as AstType, CastOptions
    from .ast_templates import BaseTemplateParameter, Template
