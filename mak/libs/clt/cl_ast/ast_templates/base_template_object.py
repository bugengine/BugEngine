from ..cppobject import CppObject
from abc import abstractmethod


class BaseTemplateObject(CppObject):
    @abstractmethod
    def find_instance(self, parameter_binds, arguments, position):
        # type: (Dict[BaseTemplateParameter, Tuple[int, Template]], List[Union[Value, TypeRef]], Position) -> Optional[CppObject]
        raise NotImplementedError

    @abstractmethod
    def instantiate(self, arguments, position):
        # type: (List[Union[Value, TypeRef]], Position) -> Optional[CppObject]
        raise NotImplementedError


class BaseTemplateParameter(CppObject):
    def __init__(self, default_value):
        # type: (Optional[CppObject]) -> None
        self.parameter_bind = None     # type: Optional[Tuple[int, Template]]
        self.default_value = default_value

    def __eq__(self, other):
        # type: (object) -> bool
        if not isinstance(other, self.__class__):
            return False
        if self.parameter_bind:
            return self.parameter_bind == other.parameter_bind
        return False

    def __hash__(self):
        # type: () -> int
        if self.parameter_bind:
            return hash(self.parameter_bind)
        else:
            return id(self)

    def get_unresolved_parameters(self):
        # type: () -> List[BaseTemplateParameter]
        return [self]

    def is_bound(self):
        # type: () -> bool
        return self.parameter_bind != None

    def bind(self, argument_position, template):
        # type: (int, Template) -> None
        self.parameter_bind = (argument_position, template)

    def write_to(self, writer):
        # type: (ClDocumentWriter) -> None
        raise NotImplementedError

    def signature(self, template_bindings={}):
        # type: (Dict[BaseTemplateParameter, Tuple[int, Template]]) -> str
        parameter_bind = self.parameter_bind or template_bindings.get(self)
        if parameter_bind:
            return '<%s:%d>' % (parameter_bind[1].signature(), parameter_bind[0])
        else:
            return '<%s>' % id(self)

    @abstractmethod
    def get_template_parameter_type(self):
        # type: () -> str
        raise NotImplementedError


from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from typing import Dict, List, Optional, Tuple, Union
    from ...cl_lexer import ClLexer
    from ...cl_document_writer import ClDocumentWriter
    from ..position import Position
    from ..value import Value
    from ..typeref import TypeRef
    from .template import Template
