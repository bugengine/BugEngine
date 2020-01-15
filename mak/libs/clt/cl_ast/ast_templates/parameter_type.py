from .base_template_object import BaseTemplateParameter
from ..type import Type, CastOptions, CastError
from ..scope import Scope
from ..typeref import TypeRef
from be_typing import cast, TYPE_CHECKING


class TemplateTypenameParameter(BaseTemplateParameter, Type):
    class INITIAL_SCOPE(Scope):
        def find(self, name, position, source_context, is_current_scope):
            # type: (str, Position, Optional[CppObject], bool) -> Optional[CppObject]
            return None

    def __init__(self, lexer, position, name, default_value):
        # type: (ClLexer, Position, Optional[str], Optional[TypeRef]) -> None
        Type.__init__(self, lexer, position, name)
        BaseTemplateParameter.__init__(self, default_value)
        self.scope = lexer.UnknownScope()

    def get_token_type(self):
        # type: () -> str
        return 'TYPENAME_ID'

    def __str__(self):
        # type: () -> str
        return 'typename%s' % (self.name and ' ' + self.name or '')

    def get_template_parameter_type(self):
        # type: () -> str
        return 'typename'

    def is_compatible(self, argument):
        # type: (CppObject) -> bool
        return isinstance(argument, TypeRef)

    def template_parameter_match(self, type, cast_options, typeref_from, typeref_typename):
        # type: (Type, CastOptions, TypeRef, TypeRef) -> Type.Distance
        def old_match():
            # type: () -> Type.Distance
            assert self.parameter_bind
            tref = cast_options.template_parameter_matches[self.parameter_bind[0]]
            assert isinstance(tref, TypeRef)
            new_match = tref.clone()
            for a in typeref_from.qualifiers:
                if a in new_match.qualifiers:
                    new_match.qualifiers.remove(a)
            return typeref_from.distance(
                new_match,
                CastOptions(
                    CastOptions.CAST_NONE, cast_options.template_parameter_matches, cast_options.template_bindings,
                    cast_options.current_template
                )
            )

        def make_match():
            # type: () -> Type.Distance
            assert self.parameter_bind
            match = typeref_from.clone()
            d = Type.Distance(variant=100000, matches={self.parameter_bind[0]: match})
            for a in typeref_typename.qualifiers:
                if a in typeref_from.qualifiers:
                    match.qualifiers.remove(a)
                    d = d.refine()
                elif cast_options.allowed_cast == cast_options.CAST_UNRELATED:
                    return Type.Distance(variant=-1)
                else:
                    raise CastError(
                        'type %s is not compatible with %s' % (typeref_from, typeref_typename),
                        typeref_typename.position
                    )
            #for a in typeref_from.qualifiers:
            #    if a not in typeref_typename.qualifiers:
            #        raise CastError('type %s is not compatible with %s' % (typeref_from, typeref_typename), self.position)
            return d

        if isinstance(type, TemplateTypenameParameter):
            type_parameter_bind = type.parameter_bind or cast_options.template_bindings.get(type)
            if self.parameter_bind and self.parameter_bind == type_parameter_bind:
                match = typeref_from.clone()
                for a in typeref_from.qualifiers:
                    if a in match.qualifiers:
                        match.qualifiers.remove(a)
                d = Type.Distance(matches={self.parameter_bind[0]: match})
                return d.match_attributes(cast_options.allowed_cast, typeref_from, typeref_typename)
            elif self.parameter_bind and self.parameter_bind[1] == cast_options.current_template:
                if self.parameter_bind[0] in cast_options.template_parameter_matches:
                    return old_match()
                elif self.parameter_bind:
                    return make_match()
            elif cast_options.allowed_cast == cast_options.CAST_UNRELATED:
                return Type.Distance(variant=-1)
            else:
                raise CastError(
                    'type %s is not compatible with %s' % (typeref_from, typeref_typename), typeref_typename.position
                )
        elif self.parameter_bind and self.parameter_bind[0] in cast_options.template_parameter_matches:
            return old_match()
        elif self.parameter_bind:
            return make_match()
        else:
            raise CastError(
                'type %s is not compatible with %s' % (typeref_from, typeref_typename), typeref_typename.position
            )

    def _distance(self, cast_to, cast_options, typeref_from, typeref_to):
        # type: (Type, CastOptions, TypeRef, TypeRef) -> Type.Distance
        raise CastError('type %s is not compatible with %s' % (typeref_from, typeref_to), self.position)

    def _create_template_instance(self, template, arguments, position):
        # type: (Template, ArgumentList, Position) -> TemplateTypenameParameter
        from .template import Template
        assert self.parameter_bind
        if template == self.parameter_bind[1]:
            return arguments[self.parameter_bind[0]] # type: ignore
        else:
            template_parent = self.parameter_bind[1].create_template_instance(template, arguments, position)
            assert isinstance(template_parent, Template)
            default_value = None
            if self.default_value:
                default_value = cast(TypeRef,
                                     self.default_value).create_template_instance(template, arguments, position)
            result = TemplateTypenameParameter(self.lexer, position, self.name, default_value)
            result.bind(self.parameter_bind[0], template_parent)
            return result


if TYPE_CHECKING:
    from typing import Dict, List, Optional, Tuple, Union
    from ...cl_lexer import ClLexer
    from ...cl_document_writer import ClDocumentWriter
    from ..position import Position
    from ..value import Value
    from ..argument_list import ArgumentList
    from ..cppobject import CppObject
    from .template import Template
