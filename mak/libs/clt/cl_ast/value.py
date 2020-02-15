from .cppobject import CppObject
from .type import CastError, Type
from abc import abstractmethod
from be_typing import TYPE_CHECKING


class Value(CppObject):
    def distance(self, other, cast_options):
        # type: (CppObject, CastOptions) -> Type.Distance
        from .ast_templates import TemplateValueParameter
        if isinstance(other, TemplateValueParameter):
            return other.template_parameter_match(self, cast_options)
        elif isinstance(other, Value):
            if self == other:
                return Type.Distance()
            else:
                raise CastError(self.lexer.logger.C0300, self.position, from_type=self, to_type=other)
        else:
            raise CastError(self.lexer.logger.C0300, self.position, from_type=self, to_type=other)

    @abstractmethod
    def simplify(self):
        # type: () -> Value
        raise NotImplementedError

    @abstractmethod
    def return_type(self):
        # type: () -> TypeRef
        raise NotImplementedError


if TYPE_CHECKING:
    from .typeref import TypeRef, CastOptions