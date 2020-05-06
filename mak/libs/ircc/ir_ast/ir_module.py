from be_typing import TYPE_CHECKING


class IrModule:
    def __init__(self, header, decls):
        # type: (List[IrHeader], List[IrDeclaration]) -> None
        self._header = header
        self._declarations = {}
        for decl in decls:
            self._declarations[decl.get_name()] = decl

    def resolve(self):
        # type: () -> None
        for decl in self._declarations.values():
            decl.resolve(self)

    def get(self, reference):
        # type: (IrReference) -> Optional[IrDeclaration]
        return self._declarations.get(reference, None)


if TYPE_CHECKING:
    from typing import List, Optional
    from .ir_header import IrHeader
    from .ir_reference import IrReference
    from .ir_declaration import IrDeclaration