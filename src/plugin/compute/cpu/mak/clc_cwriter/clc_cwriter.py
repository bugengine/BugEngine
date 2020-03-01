from .document import Document
from be_typing import TYPE_CHECKING


class ClcCWriter:
    def __init__(self, out_file):
        # type: (BinaryIO) -> None
        self._out_file = out_file

    def create_document(self):
        # type: () -> Document
        return Document(self._out_file)


if TYPE_CHECKING:
    from typing import BinaryIO