from .cppobject import CppObject
from .error import CppError
from .position import Position
from abc import abstractmethod


class CastError(CppError):
    pass


class CastOptions:
    CAST_NONE = 0
    CAST_ATTRIB = 1
    CAST_IMPLICIT = 2
    CAST_STATIC = 3
    CAST_UPCAST = 4
    CAST_REINTERPRET = 5
    CAST_UNRELATED = 6

    def __init__(self, allowed_cast, template_parameter_matches={}, template_bindings={}, current_template=None):
        # type: (int, Dict[int, CppObject], Dict[BaseTemplateParameter, Tuple[int, BaseTemplateObject]], Optional[Template]) -> None
        self.allowed_cast = allowed_cast
        self.template_parameter_matches = template_parameter_matches
        self.template_bindings = template_bindings
        self.current_template = current_template


class Type(CppObject):
    class Distance:
        def __init__(self, variant=0, cast=0, attribute=0, matches={}):
            # type: (int, int, int, Dict[int, CppObject]) -> None
            self.distance = (variant, cast, attribute)
            self.matches = {}
            for a, m in matches.items():
                self.matches[a] = m

        def refine(self):
            # type: () -> Type.Distance
            if self.distance[0] > 0:
                self.distance = (self.distance[0] - 1, self.distance[1], self.distance[2])
            return self

        def __bool__(self):
            # type: () -> bool
            return self.distance[0] >= 0 and self.distance[1] >= 0 and self.distance[2] >= 0

        def exact_match(self):
            # type: () -> bool
            return self.distance[0] == 0 and self.distance[1] == 0 and self.distance[2] == 0

        def match_attributes(self, allowed_cast, typeref_from, typeref_to):
            # type: (int, TypeRef, TypeRef) -> Type.Distance
            attrs1 = typeref_from.qualifiers
            attrs2 = typeref_to.qualifiers
            if allowed_cast < CastOptions.CAST_IMPLICIT:
                for a in attrs1:
                    if a not in attrs2:
                        raise CastError(
                            typeref_from.lexer.logger.C0305,
                            typeref_to.position,
                            from_type=typeref_from,
                            to_type=typeref_to,
                            qualifier=a
                        )
            cast_cost = 0
            for a in attrs2:
                if a not in attrs1:
                    cast_cost += 1
            if not allowed_cast:
                if cast_cost:
                    raise CastError(
                        typeref_from.lexer.logger.C0300,
                        typeref_from.position,
                        from_type=typeref_from,
                        to_type=typeref_to
                    )
            self.distance = (self.distance[0], self.distance[1], self.distance[2] + cast_cost)
            return self

        def __eq__(self, other):
            # type: (object) -> bool
            if not isinstance(other, Type.Distance):
                raise TypeError
            return self.distance == other.distance

        def __ne__(self, other):
            # type: (object) -> bool
            if not isinstance(other, Type.Distance):
                raise TypeError
            return self.distance != other.distance

        def __lt__(self, other):
            # type: (object) -> bool
            if not isinstance(other, Type.Distance):
                raise TypeError
            return self.distance < other.distance

        def __le__(self, other):
            # type: (object) -> bool
            if not isinstance(other, Type.Distance):
                raise TypeError
            return self.distance <= other.distance

        def __gt__(self, other):
            # type: (object) -> bool
            if not isinstance(other, Type.Distance):
                raise TypeError
            return self.distance > other.distance

        def __ge__(self, other):
            # type: (object) -> bool
            if not isinstance(other, Type.Distance):
                raise TypeError
            return self.distance >= other.distance

        def __iadd__(self, other):
            # type: (object) -> Type.Distance
            if not isinstance(other, Type.Distance):
                raise TypeError
            for k, v in other.matches.items():
                if k in self.matches:
                    d = v.distance(self.matches[k], CastOptions(CastOptions.CAST_NONE))
                    if not d.exact_match():
                        # TODO!
                        raise CastError(None, Position('', 0, 0, 0, ''))
                else:
                    self.matches[k] = v
            self.distance = (
                self.distance[0] + other.distance[0],
                self.distance[1] + other.distance[1],
                self.distance[2] + other.distance[2],
            )
            return self

        def __repr__(self):
            # type: () -> str
            return 'd[%s,%s,%s]' % self.distance

        def __str__(self):
            # type: () -> str
            return 'variant=%d / attribute = %d / cast = %d' % (self.distance[0], self.distance[1], self.distance[2])

    def get_type(self):
        # type: () -> Type
        return self

    def get_unresolved_parameters(self):
        # type: () -> List[BaseTemplateParameter]
        return []

    def simplify(self):
        # type: ()  -> Union[Value, Type]
        return self

    @abstractmethod
    def _distance(self, cast_to, cast_options, typeref_from, typeref_to):
        # type: (Type, CastOptions, TypeRef, TypeRef) -> Type.Distance
        raise NotImplementedError

    def is_void(self):
        # type: () -> bool
        return False

    def write_to(self, writer):
        # type: (ClDocumentWriter) -> None
        raise NotImplementedError


from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from typing import Dict, List, Optional, Tuple, Union
    from ..cl_lexer import ClLexer
    from ..cl_document_writer import ClDocumentWriter
    from .ast_templates import BaseTemplateParameter, BaseTemplateObject, Template
    from .argument_list import ArgumentList
    from .typeref import TypeRef
    from .value import Value