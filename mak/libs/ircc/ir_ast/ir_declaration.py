from be_typing import TYPE_CHECKING
from abc import abstractmethod


class IrDeclaration:
    @abstractmethod
    def resolve(self, module):
        # type: (IrModule) -> IrDeclaration
        raise NotImplementedError


if TYPE_CHECKING:
    from typing import List
    from .ir_module import IrModule
    from .ir_reference import IrReference
