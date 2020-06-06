from be_typing import TYPE_CHECKING
from abc import abstractmethod


class IrDeclaration:
    def __init__(self):
        pass

    @abstractmethod
    def resolve(self, module):
        # type: (IrModule) -> IrDeclaration
        raise NotImplementedError

    @abstractmethod
    def collect(self, ir_name):
        # type: (str) -> List[Tuple[str, IrDeclaration]]
        return []

    def visit(self, generator, ir_name):
        # type: (IrccGenerator, str) -> None
        pass

if TYPE_CHECKING:
    from typing import List, Tuple
    from .ir_module import IrModule
    from .ir_reference import IrReference
    from ..ir_codegen import IrccGenerator