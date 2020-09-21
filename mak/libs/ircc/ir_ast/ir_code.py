from be_typing import TYPE_CHECKING
from .ir_declaration import IrDeclaration


class IrInstruction(IrDeclaration):
    def __init__(self, opcode, result, metadata):
        # type: (str, Optional[IrReference], List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrDeclaration.__init__(self)
        self._opcode = opcode
        self._result = result
        self._metadata = metadata

    def terminal(self):
        # type: () -> bool
        return False

    def labels(self):
        # type: () -> List[str]
        return []

    def declare(self, scope):
        # type: (IrScope) -> None
        if self._result:
            scope.declare(self._result, self)

    def resolve(self, module):
        # type: (IrModule) -> IrInstruction
        return self


class IrCodeSegment:
    def __init__(self, label, instructions):
        # type: (str, List[IrInstruction]) -> None
        self._label = label
        self._instructions = instructions
        self._nexts = []   # type: List[IrCodeSegment]

    def resolve(self, module):
        # type: (IrModule) -> IrCodeSegment
        self._instructions = [i.resolve(module) for i in self._instructions]
        return self


class IrCodeBlock:
    def __init__(self, instructions):
        # type: (List[IrInstruction]) -> None
        self._segments = []
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

    def resolve(self, module):
        # type: (IrModule) -> IrCodeBlock
        self._segments = [s.resolve(module) for s in self._segments]
        return self

    def instantiate(self, parameters):
        # type: (List[IrMethodParameter]) -> IrCodeBlock
        return self


from . import instructions as ir_instructions
if TYPE_CHECKING:
    from typing import List, Optional, Tuple, Union
    from .ir_module import IrModule
    from .ir_metadata import IrMetadataLink
    from .ir_type import IrType
    from .ir_value import IrValue
    from .ir_expr import IrExpression
    from .ir_reference import IrReference
    from .ir_method import IrMethodParameter
    from .ir_scope import IrScope