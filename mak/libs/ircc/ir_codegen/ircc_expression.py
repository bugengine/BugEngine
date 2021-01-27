from abc import abstractmethod
from be_typing import TYPE_CHECKING


class IrccExpression:
    @abstractmethod
    def __str__(self):
        # type: () -> str
        raise NotImplementedError

    @abstractmethod
    def make_string_left(self, associativity, precedence):
        # type: (Literal['left', 'right'], int) -> str
        raise NotImplementedError

    @abstractmethod
    def make_string_right(self, associativity, precedence):
        # type: (Literal['left', 'right'], int) -> str
        raise NotImplementedError


if TYPE_CHECKING:
    from typing import Literal