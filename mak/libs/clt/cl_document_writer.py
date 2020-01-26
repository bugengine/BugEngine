from .cl_ast.position import Position
from be_typing import TYPE_CHECKING


class ClDocumentWriter(object):
    def __init__(self):
        # type: () -> None
        pass

    def __enter__(self):
        # type: () -> ClDocumentWriter
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        # type: (Optional[Type[BaseException]], Optional[BaseException], Optional[TracebackType]) -> None
        pass

    def create_document(self):
        # type: () -> ClDocumentWriter
        raise NotImplementedError

    def create_namespace(self, position, namespace_name):
        # type: (Position, str) -> ClDocumentWriter
        raise NotImplementedError

    def create_struct(self, position, struct_id, struct_name, parent_visibility, parent_id):
        # type: (Position, int, str, Optional[str], Optional[int]) -> ClDocumentWriter
        raise NotImplementedError

    def create_method(self, position, method_id, method_name, overload_id):
        # type: (Position, int, str, int) -> ClDocumentWriter
        raise NotImplementedError

    def create_variable(self, position, type, name, initial_value, attributes):
        # type: (Position, str, str, str, List[str]) -> ClDocumentWriter
        raise NotImplementedError


if TYPE_CHECKING:
    from typing import List, Optional, Type
    from types import TracebackType
