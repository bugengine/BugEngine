from clt.cl_codegen import ClMethod
from be_typing import TYPE_CHECKING


class Method(ClMethod):
    def __init__(self, document, position, method_id, overload_id, method_name, return_type):
        # type: (Document, Position, int, int, List[str], Optional[ClType]) -> None
        ClMethod.__init__(self, document, position)
        self._id = (method_id, overload_id)
        self._name = method_name
        self._return_type = return_type
        self._parameters = []  # type: List[Tuple[ClType, Optional[str]]]

    def add_parameter(self, type, name):
        # type: (ClType, Optional[str]) -> None
        self._parameters.append((type, name))

    def declare(self, out_file):
        # type: (BinaryIO) -> None
        out_file.write(b'/* %s */\n' % '_'.join(self._name).encode('utf-8'))
        name = 'method_{}_{}'.format(*self._id)

    def define(self, out_file):
        # type: (BinaryIO) -> None
        pass


if TYPE_CHECKING:
    from typing import BinaryIO, List, Optional, Tuple
    from .document import Document
    from clt.cl_codegen import ClType
    from clt.cl_ast.position import Position