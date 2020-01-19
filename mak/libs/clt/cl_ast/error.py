class CppError(Exception):
    def __init__(self, error_message, position, inner_error=None):
        # type: (str, Position, Optional[CppError]) -> None
        self.message = error_message
        self.position = position
        self.inner_error = inner_error


from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from typing import Optional
    from .position import Position
