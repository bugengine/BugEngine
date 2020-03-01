from be_typing import TYPE_CHECKING


class ClVariable:
    def __init__(self, document, position):
        # type: (ClDocumentWriter, Position) -> None
        self._document = document
        self._position = position


if TYPE_CHECKING:
    from .document_writer import ClDocumentWriter
    from ..cl_ast.position import Position