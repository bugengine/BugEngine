from be_typing import TYPE_CHECKING
from .ir_type import IrAddressSpace, IrAddressSpaceInference, IrTypeVoid
from .ir_expr import IrExpression, IrExpressionDeclaration
from abc import abstractmethod


class IrInstruction(IrExpression):
    def __init__(self, opcode, result, metadata):
        # type: (str, Optional[IrReference], List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrExpression.__init__(self)
        self._opcode = opcode
        self._result = result
        self._metadata = metadata
        self._value_type = None    # type: Optional[IrType]

    def get_type(self):
        # type: () -> Tuple[IrType, IrAddressSpace]
        if self._result is not None:
            assert self._value_type is not None
            return self._value_type, IrAddressSpace(0)
        else:
            return IrTypeVoid(), IrAddressSpace(0)

    def terminal(self):
        # type: () -> bool
        return False

    def labels(self):
        # type: () -> List[str]
        return []

    def declare(self, scope):
        # type: (IrScope) -> None
        if self._result:
            scope.declare(self._result, IrExpressionDeclaration(self))

    def resolve(self, module):
        # type: (IrModule) -> IrInstruction
        return self

    def resolve_type(self, equivalence, return_type):
        # type: (IrAddressSpaceInference, IrType) -> None
        return


class IrCodeSegment:
    def __init__(self, label, instructions):
        # type: (str, List[IrInstruction]) -> None
        self._label = label
        self._instructions = instructions
        self._nexts = []   # type: List[IrCodeSegment]

    def resolve(self, module, equivalence, return_type):
        # type: (IrModule, IrAddressSpaceInference, IrType) -> IrCodeSegment
        self._instructions = [i.resolve(module) for i in self._instructions]
        for i in self._instructions:
            i.resolve_type(equivalence, return_type)
        return self


class IrCodeBlock:
    def __init__(self, instructions):
        # type: (List[IrInstruction]) -> None
        self._segments = []
        self._equivalence = IrAddressSpaceInference()
        label = 'start'
        stream = []    # type: List[IrInstruction]
        for instruction in instructions:
            if instruction._opcode == 'label':
                assert len(stream) == 0
                assert isinstance(instruction, ir_instructions.IrInstLabel)
                label = instruction._label
            else:
                stream.append(instruction)
                if instruction.terminal():
                    self._segments.append(IrCodeSegment(label, stream))
                    stream = []
                    label = '!!!'
        for s in self._segments:
            terminal = s._instructions[-1]
            nexts = terminal.labels()
            s._nexts = [self._find_segment(n) for n in s._instructions[-1].labels()]

    def _find_segment(self, name):
        # type: (str) -> IrCodeSegment
        for s in self._segments:
            if s._label == name:
                return s
        else:
            raise NotImplementedError

    def resolve(self, module, return_type):
        # type: (IrModule, IrType) -> IrCodeBlock
        """
            Resolving a method also resolves generic address spaces.
            The resolution step will bucket together types that are constrained into the same generic address space.
            It expects the argument types to seed the buckets.
            The kernel parameters can't use the generic address space; when a kernel method is resolved, it will
            allow to infer the correct address space of all types. Any type whose generic address space can't be
            deduced at compile time will trigger a compile error.
        """
        self._segments = [s.resolve(module, self._equivalence, return_type) for s in self._segments]
        return self


from . import instructions as ir_instructions
if TYPE_CHECKING:
    from typing import Dict, List, Optional, Set, Tuple, Union
    from .ir_module import IrModule
    from .ir_metadata import IrMetadataLink
    from .ir_type import IrType
    from .ir_value import IrValue
    from .ir_expr import IrExpression
    from .ir_reference import IrReference
    from .ir_method import IrMethodParameter
    from .ir_scope import IrScope