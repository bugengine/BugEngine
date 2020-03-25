from be_typing import TYPE_CHECKING


class ClStruct:
    def __init__(self, document, position):
        # type: (ClDocumentWriter, Position) -> None
        self._document = document
        self._position = position
        self._dependencies = []    # type: List[ClStruct]

    def add_dependency(self, dependency_id):
        # type: (int) -> None
        struct = self._document.get_struct(dependency_id)
        self._dependencies.append(struct)


class ClEnum:
    def __init__(self, document, position):
        # type: (ClDocumentWriter, Position) -> None
        self._document = document
        self._position = position


if TYPE_CHECKING:
    from typing import List
    from .document_writer import ClDocumentWriter
    from ..cl_ast.position import Position