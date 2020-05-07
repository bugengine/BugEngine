from be_typing import TYPE_CHECKING, TypeVar

DeclType = TypeVar('DeclType', bound='IrDeclaration')


class IrModule:
    def __init__(self, header, decls):
        # type: (List[IrHeader], List[Tuple[IrReference, IrDeclaration]]) -> None
        self._header = header
        self._declarations = {}
        for name, decl in decls:
            self._declarations[name] = decl

    def resolve(self):
        # type: () -> None
        for decl in self._declarations.values():
            decl.resolve(self)

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
    from typing import List, Optional, Tuple, Type
    from .ir_header import IrHeader
    from .ir_reference import IrReference
    from .ir_declaration import IrDeclaration