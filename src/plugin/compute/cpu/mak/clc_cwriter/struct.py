from clt.cl_codegen import ClStruct
from be_typing import TYPE_CHECKING


class Struct(ClStruct):
    def __init__(self, document, position, struct_id, struct_name, parent_id):
        # type: (Document, Position, int, List[str], Optional[int]) -> None
        ClStruct.__init__(self, document, position)
        self._id = struct_id
        self._name = struct_name + [str(struct_id)]
        self._parent = parent_id
        self._members = []     # type: List[Tuple[ClType, str]]

    def struct_name(self):
        # type: () -> str
        return '_' + '_'.join(self._name)

    def declare(self, out_file):
        # type: (BinaryIO) -> None
        self._document.write_position(self._position)
        out_file.write(b'struct %s;\n' % self.struct_name().encode('utf-8'))
        out_file.write(b'typedef struct %s struct_%d;\n' % (self.struct_name().encode('utf-8'), self._id))

    def define(self, out_file):
        # type: (BinaryIO) -> None
        self._document.write_position(self._position)
        out_file.write(b'struct %s\n' b'{\n' % self.struct_name().encode('utf-8'))
        #for variable in self.variables:
        #    variable.define(out_file)
        out_file.write(b'};\n')


if TYPE_CHECKING:
    from typing import BinaryIO, List, Optional, Tuple
    from .document import Document
    from clt.cl_ast.position import Position
    from clt.cl_codegen import ClType