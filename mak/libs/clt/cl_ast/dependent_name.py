from .type import Type, CastError
from .value import Value
from .name import Name
from .scope import Scope
from .error import CppError
from .ast_templates import BaseTemplateObject, Template


class DependentName(Type, Value):
    class INITIAL_SCOPE(Scope):
        def find(self, name, position, source_context, is_current_scope):
            # type: (str, Position, CppObject, bool) -> Optional[CppObject]
            return None

    def __init__(self, lexer, position, name):
        # type: (ClLexer, Position, Name) -> None
        Type.__init__(self, lexer, position)
        self.qualified_name = name

    def __eq__(self, other):
        # type: (object) -> bool
        return isinstance(other, DependentName) and self.qualified_name == other.qualified_name

    def __str__(self):
        # type: () -> str
        return 'typename(%s)' % self.name

    def get_unresolved_parameters(self):
        # type: () -> List[BaseTemplateParameter]
        result = []                # type: List[BaseTemplateParameter]
        n = self.qualified_name    # type: Optional[Name]
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
        # type: (Name, Template, ArgumentList, Position) -> Optional[CppObject]
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
                    assert isinstance(result, BaseTemplateObject)
                    result = result.instantiate(
                        [
                            a.create_template_instance(instance_template, instance_arguments, instance_position)
                            for a in name.arguments
                        ], name.position
                    )
                except Template.InstantiationError as e:
                    raise Template.InstantiationError(
                        "in instantiation of template '%s' requested here" % name, name.position, e
                    )
        return result

    def _create_partial_template_instance(self, template, arguments, position):
        # type: (Template, ArgumentList, Position) -> DependentName
        def create_name(n):
            # type: (Optional[Name]) -> Optional[Name]
            if n:
                return Name(
                    n.lexer,
                    n.name,
                    n.position,
                    target=n.target and n.target.create_template_instance(template, arguments, position),
                    template=n.template and n.template.create_template_instance(template, arguments, position),
                    arguments=[a.create_template_instance(template, arguments, position) for a in n.arguments or []],
                    parent=create_name(n.parent)
                )
            else:
                return None

        self.qualified_name.target = None
        name = create_name(self.qualified_name)
        assert name
        name.target = DependentName(self.lexer, self.position, name)
        return name.target

    def _create_template_instance(self, template, arguments, position):
        # type: (Template, ArgumentList, Position) -> DependentName
        result = self._resolve(self.qualified_name, template, arguments, position)
        if not result:
            for s in self.lexer.owner_scopes[::-1]:
                if isinstance(s.owner, Template) and s.owner != template:
                    return self._create_partial_template_instance(template, arguments, position)
                elif not s.owner.parent:
                    raise Template.InstantiationError('no', position)
            else:
                assert False
        return result  # type: ignore

    def simplify(self):
        # type: () -> Union[Value, Type]
        def _simplify(name):
            # type: (Name) -> Optional[CppObject]
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
                if result is not None:
                    if name.arguments is not None:
                        assert isinstance(result, BaseTemplateObject)
                        result = result.find_instance(
                            name.template_bindings and name.template_bindings.parameter_binds or {}, name.arguments,
                            name.position
                        )
                return result

        try:
            return _simplify(self.qualified_name) or self # type: ignore
        except Template.InstantiationError as e:
            self.lexer.log_cpperror(e)
            return self

    def _distance(self, other, cast_options, typeref, other_typeref):
        # type: (Type, CastOptions, TypeRef, TypeRef) -> Type.Distance
        if isinstance(other, DependentName):
            if not self.qualified_name.equals(other.qualified_name, cast_options.template_bindings):
                if cast_options.allowed_cast == cast_options.CAST_UNRELATED:
                    d = Type.Distance(variant=-1)
                    return d.match_attributes(cast_options.allowed_cast, typeref, other_typeref)
                else:
                    raise CastError('type %s is not compatible with %s' % (self, other), self.position)
            else:
                d = Type.Distance(cast=0)
                return d.match_attributes(cast_options.allowed_cast, typeref, other_typeref)
        elif cast_options.allowed_cast == cast_options.CAST_UNRELATED:
            d = Type.Distance(variant=-1)
            return d.match_attributes(cast_options.allowed_cast, typeref, other_typeref)
        else:
            raise CastError('type %s is not compatible with %s' % (self, other), self.position)

    def signature(self, template_bindings={}):
        # type: (Dict[BaseTemplateParameter, Tuple[int, Template]]) -> str
        return '<%s>' % self.qualified_name

    def return_type(self):
        # type: () -> TypeRef
        # TODO
        raise NotImplementedError


from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from typing import Dict, List, Optional, Tuple, Union
    from ..cl_lexer import ClLexer
    from ..cl_document_writer import ClDocumentWriter
    from .cppobject import CppObject
    from .position import Position
    from .argument_list import ArgumentList
    from .type import CastOptions
    from .typeref import TypeRef
    from .ast_templates import BaseTemplateParameter, Template
