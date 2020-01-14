from .cppobject import CppObject
from .error import CppError
from .scope import Scope, ScopeError
from abc import abstractmethod


class TypeRef(CppObject):
    class INITIAL_SCOPE(Scope):
        def find(self, name, position, source_context, is_current_scope):
            # type: (str, Position, CppObject, bool) -> Optional[CppObject]
            assert isinstance(self.owner, TypeRef)
            return self.owner.type and self.owner.type.scope.find(name, position, source_context, is_current_scope)

    def __init__(self, lexer, position, type, template=None):
        # type: (ClLexer, Position, Optional[Type], Optional[Template]) -> None
        CppObject.__init__(self, lexer, position)
        self.type = type
        self.qualifiers = set([])  # type: Set[str]
        self.template_origin = template

    def simplify(self):
        # type: () -> TypeRef
        return self

    def get_type(self):
        # type: () -> Type
        assert self.type
        return self.type.get_type()

    def get_operation_type(self, operand_type, operation):
        # type: (TypeRef, str) -> TypeRef
        return self

    def is_void(self):
        # type: () -> bool
        assert self.type
        return self.type.get_type().is_void()

    def add_qualifier(self, qualifier):
        # type: (str) -> None
        self.qualifiers.add(qualifier)

    def clone(self):
        # type: () -> TypeRef
        result = TypeRef(self.lexer, self.position, self.type, self.template_origin)
        for q in self.qualifiers:
            result.qualifiers.add(q)
        return result

    def instantiate(self, arguments, position):
        # type: (List[Union[Value, TypeRef]], Position) -> Optional[CppObject]
        assert self.template_origin
        return self.template_origin.instantiate(arguments, position)

    def distance(self, other, cast_options):
        # type: (CppObject, CastOptions) -> Type.Distance
        from .ast_types.typedef import TypeDef
        from .ast_templates import TemplateTypenameParameter, TemplateTemplateParameter
        if isinstance(other, TypeRef):
            if self.type is None:
                raise CastError('type %s is not compatible with %s' % (self, other), self.position)
            if isinstance(other.type, TypeDef):
                d = self.distance(other.type.type, cast_options)
                return d.match_attributes(cast_options.allowed_cast, self, other)
            elif isinstance(other.type, TemplateTypenameParameter):
                return other.type.template_parameter_match(self.type, cast_options, self, other)
            else:
                assert other.type
                return self.type._distance(other.type, cast_options, self, other)
        elif isinstance(other, TemplateTemplateParameter):
            return other.template_parameter_match(self, cast_options, self, other)
        else:
            raise CastError('type %s is not compatible with %s' % (self, other), self.position)

    def _create_template_instance(self, template, arguments, position):
        # type: (Template, ArgumentList, Position) -> TypeRef
        assert self.type
        type = self.type.create_template_instance(template, arguments, position)
        if isinstance(type, TypeRef):
            result = type.clone()
            result.qualifiers |= self.qualifiers
        else:
            result = self.clone()
            result.type = type
        return result

    def __eq__(self, other):
        # type: (object) -> bool
        if not isinstance(other, TypeRef):
            return False
        return self.type == other.type and self.qualifiers == other.qualifiers

    def __str__(self):
        # type: () -> str
        return str(self.type) + (self.qualifiers and ' ' + ' '.join(self.qualifiers) or '')

    __repr__ = __str__
    pretty_name = __str__

    def get_unresolved_parameters(self):
        # type: () -> List[BaseTemplateParameter]
        if self.type:
            return self.type.get_unresolved_parameters()
        else:
            assert self.template_origin
            return self.template_origin.get_unresolved_parameters()

    def signature(self, template_bindings={}):
        # type: (Dict[BaseTemplateParameter, Tuple[int, Template]]) -> str
        assert self.type
        s = self.type.signature(template_bindings)
        for a in self.qualifiers:
            if a == 'const':
                s = 'K' + s
            elif a == 'volatile':
                s = 'V' + s
            else:
                assert False
        return s

    def write_to(self, writer):
        # type: (ClDocumentWriter) -> None
        raise NotImplementedError


from typing import TYPE_CHECKING
if TYPE_CHECKING:
    from typing import Dict, List, Optional, Set, Tuple, Union
    from ..cl_lexer import ClLexer
    from ..cl_document_writer import ClDocumentWriter
    from .ast_templates import BaseTemplateParameter, Template
    from .argument_list import ArgumentList
    from .position import Position
    from .value import Value
    from .type import Type, CastOptions, CastError