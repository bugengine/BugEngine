from clt.cl_codegen import ClVariable
from be_typing import TYPE_CHECKING


class Variable(ClVariable):
    def __init__(self, document, position, type, name):
        # type: (Document, Position, ClType, List[str]) -> None
        ClVariable.__init__(self, document, position)
        self._type = type
        self._name = name

    def declare(self, out_file):
        # type: (BinaryIO) -> None
        pass

    def define(self, out_file):
        # type: (BinaryIO) -> None
        pass
        #self.document.write_position(self.position)
        #if self.value:
        #    out_file.write(b'%s %s = %s;\n' % (self.type.type.encode('utf-8'),
        #                                       self.name.encode('utf-8'),
        #                                       self.value.encode('utf-8')))
        #else:
        #    out_file.write(b'%s %s;\n' % (self.type.type.encode('utf-8'),
        #                                  self.name.encode('utf-8')))


if TYPE_CHECKING:
    from typing import BinaryIO, List, Optional
    from .document import Document
    from clt.cl_ast.position import Position
    from clt.cl_codegen import ClType