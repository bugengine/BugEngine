from be_typing import TYPE_CHECKING
from abc import abstractmethod


class ClDocumentWriter(object):
    def __init__(self, out_stream, typewriter):
        # type: (BinaryIO, ClTypeWriter) -> None
        self._out_stream = out_stream
        self._struct_registry = {}     # type: Dict[int, ClStruct]
        self._method_registry = {}     # type: Dict[Tuple[int, int], ClMethod]
        self._variable_registry = []   # type: List[ClVariable]
        self.typewriter = typewriter

    def create_struct(self, position, name, struct_id, parent_type):
        # type: (Position, List[str], int, Optional[int]) -> ClStruct
        s = self._create_struct(position, name, struct_id, parent_type)
        self._struct_registry[struct_id] = s
        return s

    def create_method(self, position, name, method_id, overload_id, return_type):
        # type: (Position, List[str], int, int, Optional[ClType]) -> ClMethod
        m = self._create_method(position, name, method_id, overload_id, return_type)
        self._method_registry[(method_id, overload_id)] = m
        return m

    def create_variable(self, position, name, variable_type):
        # type: (Position, List[str], ClType) -> ClVariable
        v = self._create_variable(position, name, variable_type)
        self._variable_registry.append(v)
        return v

    def get_struct(self, struct_id):
        # type: (int) -> ClStruct
        return self._struct_registry[struct_id]

    def get_method(self, method_id, overload_id):
        # type: (int, int) -> ClMethod
        return self._method_registry[(method_id, overload_id)]

    @abstractmethod
    def write_position(self, position):
        # type: (Position) -> None
        raise NotImplementedError

    @abstractmethod
    def _create_struct(self, position, name, struct_id, parent_type):
        # type: (Position, List[str], int, Optional[int]) -> ClStruct
        raise NotImplementedError

    @abstractmethod
    def _create_method(self, position, name, method_id, overload_id, return_type):
        # type: (Position, List[str], int, int, Optional[ClType]) -> ClMethod
        raise NotImplementedError

    @abstractmethod
    def _create_variable(self, position, name, variable_type):
        # type: (Position, List[str], ClType) -> ClVariable
        raise NotImplementedError


if TYPE_CHECKING:
    from typing import BinaryIO, List, Dict, Optional, Tuple
    from types import TracebackType
    from ..cl_ast.position import Position
    from .type_writer import ClTypeWriter, ClType
    from .method import ClMethod
    from .variable import ClVariable
    from .struct import ClStruct