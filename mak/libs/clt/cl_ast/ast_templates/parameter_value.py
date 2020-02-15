from .base_template_object import BaseTemplateParameter
from .template import Template
from ..value import Value
from ..type import Type, CastOptions, CastError
from be_typing import cast


class TemplateValueParameter(BaseTemplateParameter, Value):
    def __init__(self, lexer, position, name, type, default_value):
        # type: (ClLexer, Position, Optional[str], TypeRef, Optional[Value]) -> None
        Value.__init__(self, lexer, position, name)
        BaseTemplateParameter.__init__(self, default_value)
        self.type = type

    def get_token_type(self):
        # type: () -> str
        return 'VARIABLE_ID'

    def __str__(self):
        # type: () -> str
        return '%s%s' % (self.type, self.name and ' ' + self.name or '')

    def get_template_parameter_type(self):
        # type: () -> str
        return str(self.type)

    def is_compatible(self, argument):
        # type: (CppObject) -> bool
        return isinstance(argument, Value)

    def template_parameter_match(self, value, cast_options):
        # type: (Value, CastOptions) -> Type.Distance
        assert self.parameter_bind
        t = value.return_type()
        if isinstance(value, TemplateValueParameter):
            value_parameter_bind = value.parameter_bind or cast_options.template_bindings.get(value)
            if self.parameter_bind and self.parameter_bind == value_parameter_bind:
                return Type.Distance(matches={self.parameter_bind[0]: value})
            elif self.parameter_bind and self.parameter_bind[1] == cast_options.current_template:
                if self.parameter_bind[0] in cast_options.template_parameter_matches:
                    d = t.distance(
                        self.type,
                        CastOptions(
                            CastOptions.CAST_IMPLICIT, cast_options.template_parameter_matches,
                            cast_options.template_bindings, cast_options.current_template
                        )
                    )
                    return d
            else:
                # TODO!
                raise CastError(self.lexer.logger.C0000, Position('', 0, 0, 0, ''))
        d = t.distance(
            self.type,
            CastOptions(
                CastOptions.CAST_IMPLICIT, cast_options.template_parameter_matches, cast_options.template_bindings,
                cast_options.current_template
            )
        )
        d += Type.Distance(100000, matches={self.parameter_bind[0]: value})
        return d

    def _create_template_instance(self, template, arguments, position):
        # type: (Template, ArgumentList, Position) -> TemplateValueParameter
        assert self.parameter_bind
        if template == self.parameter_bind[1]:
            return arguments[self.parameter_bind[0]] # type: ignore
        else:
            template_parent = self.lexer.scopes[-1].owner
            assert isinstance(template_parent, Template)
            default_value = None
            if self.default_value:
                default_value = cast(Value, self.default_value).create_template_instance(template, arguments, position)
            type = self.type.create_template_instance(template, arguments, position)
            result = TemplateValueParameter(self.lexer, position, self.name, type, default_value)
            result.bind(self.parameter_bind[0], template_parent)
            return result

    def return_type(self):
        # type: () -> TypeRef
        return self.type

    def simplify(self):
        # type: () -> Value
        return self


from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from typing import Dict, List, Optional, Tuple, Union
    from ...cl_lexer import ClLexer
    from ...cl_document_writer import ClDocumentWriter
    from ..cppobject import CppObject
    from ..position import Position
    from ..argument_list import ArgumentList
    from ..typeref import TypeRef