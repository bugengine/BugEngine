from abc import abstractmethod
from be_typing import TYPE_CHECKING


class ClType:
    @abstractmethod
    def __str__(self):
        # type: () -> str
        raise NotImplementedError


class ClTypeWriter(object):
    @abstractmethod
    def builtin(self, typename):
        # type: (str) -> ClType
        raise NotImplementedError

    @abstractmethod
    def struct(self, struct_name, struct_id):
        # type: (Optional[str], int) -> ClType
        raise NotImplementedError

    @abstractmethod
    def enum(self, enum_name, enum_id):
        # type: (Optional[str], int) -> ClType
        raise NotImplementedError

    @abstractmethod
    def pointer(self, pointee):
        # type: (ClType) -> ClType
        raise NotImplementedError

    @abstractmethod
    def reference(self, pointee):
        # type: (ClType) -> ClType
        raise NotImplementedError

    @abstractmethod
    def array(self, pointee, count):
        # type: (ClType, Optional[int]) -> ClType
        raise NotImplementedError


if TYPE_CHECKING:
    from typing import List, Optional
    from .struct import ClStruct
    from .struct import ClEnum