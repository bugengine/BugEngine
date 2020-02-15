class CppError(Exception):
    def __init__(self, error, position, inner_error=None, **kw_args):
        # type: (Callable[..., Dict[str, Any]], Position, Optional[CppError], Any) -> None
        self.error = error
        self.position = position
        self.inner_error = inner_error
        self.arguments = kw_args


from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from typing import Any, Dict, Optional, Callable
    from .position import Position
