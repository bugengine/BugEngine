from abc import abstractmethod
from be_typing import TYPE_CHECKING


class ClMethod:
    def __init__(self, document, position):
        # type: (ClDocumentWriter, Position) -> None
        self._document = document
        self._position = position

    @abstractmethod
    def add_parameter(self, type, name):
        # type: (ClType, Optional[str]) -> None
        raise NotImplementedError


if TYPE_CHECKING:
    from typing import Optional
    from .type_writer import ClType
    from .document_writer import ClDocumentWriter
    from ..cl_ast.position import Position