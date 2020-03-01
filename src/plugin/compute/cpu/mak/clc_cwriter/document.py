from clt.cl_codegen import ClDocumentWriter
from .struct import Struct
from .method import Method
from .variable import Variable
from .typewriter import TypeWriter
from be_typing import TYPE_CHECKING


class Document(ClDocumentWriter):
    def __init__(self, out_stream):
        # type: (BinaryIO) -> None
        ClDocumentWriter.__init__(self, out_stream, TypeWriter())

    def write_position(self, position):
        # type: (Position) -> None
        self._out_stream.write(b'#line %d "%s"\n' % (position.line_number, position.filename.encode('utf-8')))

    def _create_struct(self, position, name, struct_id, parent_type):
        # type: (Position, List[str], int, Optional[int]) -> Struct
        return Struct(self, position, struct_id, name, parent_type)

    def _create_method(self, position, name, method_id, overload_id, return_type):
        # type: (Position, List[str], int, int, Optional[ClType]) -> Method
        return Method(self, position, method_id, overload_id, name, return_type)

    def _create_variable(self, position, name, variable_type):
        # type: (Position, List[str], ClType) -> Variable
        return Variable(self, position, variable_type, name)


if TYPE_CHECKING:
    from typing import BinaryIO, Dict, List, Optional, Set, Tuple
    from clt.cl_ast import Position
    from clt.cl_codegen import ClType