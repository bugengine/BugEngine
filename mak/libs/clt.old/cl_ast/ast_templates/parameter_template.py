from .base_template_object import BaseTemplateParameter, BaseTemplateObject
from ..typeref import TypeRef
from ..type import Type, CastError
from be_typing import cast


class TemplateTemplateParameter(BaseTemplateParameter, BaseTemplateObject):
    def __init__(self, lexer, position, name, parameters, default_value):
        # type: (ClLexer, Position, Optional[str], List[Union[Value, BaseTemplateObject, TypeRef]], Optional[CppObject]) -> None
        BaseTemplateObject.__init__(self, lexer, position, name)
        BaseTemplateParameter.__init__(self, default_value)
        self.template_params = parameters[:]

    def get_token_type(self):
        # type: () -> str
        return 'TEMPLATE_TYPENAME_ID'

    def __str__(self):
        # type: () -> str
        params = ', '.join(str(x) for x in self.template_params)
        return 'template<%s> typename%s' % (params, self.name and ' ' + self.name or '')

    def get_template_parameter_type(self):
        # type: () -> str
        return 'template typename'

    def is_compatible(self, argument):
        # type: (CppObject) -> bool
        return (
            isinstance(argument, BaseTemplateObject)
            or (isinstance(argument, TypeRef) and argument.template_origin is not None)
        )

    def find_instance(self, parameter_binds, arguments, position):
        # type: (Dict[BaseTemplateParameter, Tuple[int, BaseTemplateObject]], List[Union[Value, BaseTemplateObject, TypeRef]], Position) -> Optional[CppObject]
        return None

    def instantiate(self, arguments, position):
        # type: (List[Union[Value, BaseTemplateObject, TypeRef]], Position) -> Optional[CppObject]
        return None

    def distance(self, cast_to, cast_options):
        # type: (CppObject, CastOptions) -> Type.Distance
        raise CastError(self.lexer.logger.C0300, self.position, from_type=self, to_type=cast_to)

    def template_parameter_match(self, type, cast_options, typeref_from, typeref_typename):
        # type: (Union[TypeRef, BaseTemplateObject], CastOptions, TypeRef, BaseTemplateObject) -> Type.Distance
        from .template import Template

        def _distance(parameters, value):
            # type: (List[Union[BaseTemplateObject, Value, TypeRef]], BaseTemplateObject) -> Type.Distance
            assert self.parameter_bind
            if len(self.template_params) != len(parameters):
                raise CastError(
                    self.lexer.logger.C0300,
                    typeref_typename.position,
                    from_type=typeref_from,
                    to_type=typeref_typename
                )
            d = Type.Distance(matches={self.parameter_bind[0]: value})
            for p1, p2 in zip(self.template_params, parameters):
                # d += p1.distance(p2, cast, matches, template_bindings)
                pass
            return d

        if isinstance(type, Template):
            return _distance(type.parameters, type)
        elif isinstance(type, TypeRef) and type.template_origin:
            return self.template_parameter_match(type.template_origin, cast_options, typeref_from, typeref_typename)
        elif isinstance(type, TemplateTemplateParameter):
            type_parameter_bind = type.parameter_bind or cast_options.template_bindings.get(type)
            if type_parameter_bind and type_parameter_bind == self.parameter_bind:
                return Type.Distance(matches={self.parameter_bind[0]: type})
            elif type_parameter_bind and type_parameter_bind[1] == cast_options.current_template:
                return _distance(type.template_params, self)
            else:
                raise CastError(
                    self.lexer.logger.C0300,
                    typeref_typename.position,
                    from_type=typeref_from,
                    to_type=typeref_typename
                )
        else:
            raise CastError(
                self.lexer.logger.C0300, typeref_typename.position, from_type=typeref_from, to_type=typeref_typename
            )

    def _create_template_instance(self, template, arguments, position):
        # type: (Template, ArgumentList, Position) -> TemplateTemplateParameter
        assert self.parameter_bind
        if template == self.parameter_bind[1]:
            return arguments[self.parameter_bind[0]]                                            # type: ignore
        else:
            template_parent = self.parameter_bind[1].create_template_instance(template, arguments, position)
            assert isinstance(template_parent, Template)
            parameters = [p.create_template_instance(template, arguments, position) for p in self.template_params]
            default_value = self.default_value and self.default_value.create_template_instance(
                template, arguments, position
            )
            result = TemplateTemplateParameter(self.lexer, position, self.name, parameters, default_value)
            result.bind(self.parameter_bind[0], template_parent)
            return result

    def expand_template_arg_list(self, argument_list):
        # type: (List[Union[Value, BaseTemplateObject, TypeRef]]) -> List[Union[Value, BaseTemplateObject, TypeRef]]
        result = argument_list[:]
        #for i in range(len(argument_list), len(self.template_params)):
        #    scope = cast(Template.Scope, self.scope)
        #    value = scope.parameters[i].default_value
        #    assert value is not None
        #    assert isinstance(value, Value) or isinstance(value, TypeRef)
        #    result.append(value)
        return result

    def simplify(self, cpp_object):
        # type: (Union[Value, BaseTemplateObject, TypeRef]) -> Union[Value, BaseTemplateObject, TypeRef]
        return cpp_object

    def transform(self, writer):
        # type: (ClTypeWriter) -> ClType
        raise NotImplementedError


from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from typing import Dict, List, Optional, Tuple, Union
    from ...cl_lexer import ClLexer
    from ...cl_codegen import ClDocumentWriter, ClTypeWriter, ClType
    from ..cppobject import CppObject
    from ..position import Position
    from ..argument_list import ArgumentList
    from ..value import Value
    from ..type import CastOptions
    from .template import BaseTemplateObject, Template
