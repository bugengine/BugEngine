from be_typing import TYPE_CHECKING


class IrScope:
    def __init__(self):
        # type: () -> None
        self._declarations = {}    # type: Dict[IrReference, IrDeclaration]

    def declare(self, name, declaration):
        # type: (IrReference, IrDeclaration) -> None
        self._declarations[name] = declaration

    def get(self, reference, desired_type):
        # type: (IrReference, Type[DeclType]) -> DeclType
        try:
            decl = self._declarations[reference]
        except KeyError:
            raise
        else:
            assert isinstance(decl, desired_type)
            return decl


if TYPE_CHECKING:
    from typing import Dict, Optional, Type
    from .ir_reference import IrReference
    from .ir_declaration import IrDeclaration
    from .ir_module import DeclType