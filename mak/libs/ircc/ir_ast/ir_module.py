from be_typing import TYPE_CHECKING, TypeVar

DeclType = TypeVar('DeclType', bound='IrDeclaration')


class IrModule:
    def __init__(self, headers, decls):
        # type: (List[IrHeader], List[Tuple[IrReference, IrDeclaration]]) -> None
        self._headers = headers
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

    def visit(self, generator):
        # type: (IrccGenerator) -> None
        declarations = []
        for name, decl in self._declarations.items():
            declarations += decl.collect(name)
        generator.begin_module()
        generator.begin_headers()
        for header in self._headers:
            header.visit(generator)
        generator.end_headers()
        generator.begin_declarations()
        seen = set([])
        for name, decl in declarations:
            if name not in seen:
                seen.add(name)
                decl.visit(generator, name)
        generator.end_declarations()
        generator.end_module()

if TYPE_CHECKING:
    from typing import List, Optional, Tuple, Type
    from ..ir_codegen import IrccGenerator
    from .ir_header import IrHeader
    from .ir_reference import IrReference
    from .ir_declaration import IrDeclaration