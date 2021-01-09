from be_typing import TYPE_CHECKING, TypeVar
from .ir_scope import IrScope

DeclType = TypeVar('DeclType', bound='IrDeclaration')


class IrModule:
    def __init__(self, logger, headers, decls):
        # type: (Logger, List[IrHeader], List[Tuple[IrReference, IrDeclaration]]) -> None
        self._logger = logger
        self._headers = headers
        self._scopes = [IrScope()]
        for name, decl in decls:
            self.declare(name, decl)
        self._root_declarations = decls

    def logger(self):
        # type: () -> Logger
        return self._logger

    def declare(self, name, declaration):
        # type: (IrReference, IrDeclaration) -> None
        self._scopes[-1].declare(name, declaration)

    def push_scope(self, scope):
        #  type: (IrScope) -> None
        self._scopes.append(scope)

    def pop_scope(self):
        # type: () -> None
        self._scopes.pop(-1)

    def resolve(self, logger):
        # type: (Logger) -> None
        for _, decl in self._root_declarations:
            decl.resolve(self)
        for _, decl in self._root_declarations:
            decl.resolve_content(self)
        for _, decl in self._root_declarations:
            decl.instantiate(self)

    def get(self, reference, desired_type):
        # type: (IrReference, Type[DeclType]) -> DeclType
        for scope in self._scopes[::-1]:
            try:
                return scope.get(reference, desired_type)
            except KeyError:
                continue
        else:
            raise KeyError(reference)

    def visit(self, generators):
        # type: (Generator[IrccGenerator, None, None]) -> None
        declarations = []              # type: List[Tuple[str, IrDeclaration]]
        for name, decl in self._root_declarations:
            declarations += decl.collect(name)
        for generator in generators:
            if generator.begin_module():
                if generator.begin_headers():
                    for header in self._headers:
                        header.visit(generator)
                    generator.end_headers()
                if generator.begin_declarations():
                    seen = set([])     # type: Set[str]
                    for name, decl in declarations:
                        if name not in seen:
                            seen.add(name)
                            decl.visit(generator, name)
                    generator.end_declarations()
                generator.end_module()


if TYPE_CHECKING:
    from typing import Generator, List, Optional, Set, Tuple, Type
    from ..ir_codegen import IrccGenerator
    from ..ir_messages import Logger
    from .ir_header import IrHeader
    from .ir_reference import IrReference
    from .ir_declaration import IrDeclaration