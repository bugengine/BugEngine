from be_typing import TYPE_CHECKING, cast
from .ir_type import IrAddressSpaceInference, IrTypeVoid
from .ir_expr import IrExpression, IrExpressionDeclaration
from abc import abstractmethod


class IrCodeGenContext:
    def __init__(self, equivalence):
        # type: (Dict[int, int]) -> None
        self._equivalence = equivalence


class IrInstruction(IrExpression):
    def __init__(self, opcode, result, metadata):
        # type: (str, Optional[IrReference], List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrExpression.__init__(self, metadata)
        self._opcode = opcode
        self._result = result
        self._result_name = None if result is None else 'var_%s' % result[1:].replace('.', '_')
        self._segment = None   # type: Optional[IrCodeSegment]
        self._phi_nodes = []   # type: List[IrInstruction]

    def collect_phi_exprs(self):
        # type: () -> List[Tuple[IrExpression, str]]
        return []

    def get_type(self, suggested_type):
        # type: (IrType) -> IrType
        return IrTypeVoid()

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

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> IrPosition
        return IrExpression.resolve(self, module, position)

    def create_instance(self, equivalence):
        # type: (Dict[int, int]) -> None
        pass

    def register_stack_data(self, generator, context):
        # type: (IrccGenerator, IrCodeGenContext) -> None
        if not self.is_inline():
            assert self._result_name is not None
            generator.declare_variable(
                self._result_name,
                self.get_type(IrTypeVoid()).create_generator_type(generator, context._equivalence)
            )

    def is_inline(self):
        # type: () -> bool
        if self._result_name is not None:
            assert len(self._usage) != 0
            return len(self._usage) == 1
        else:
            return True


class IrAssignInstruction(IrInstruction):
    def __init__(self, result, type, expr):
        # type: (IrReference, IrType, IrExpression) -> None
        IrInstruction.__init__(self, 'phi_assign', None, [])
        self._type = type
        self._expression = expr

    def get_type(self, suggested_type):
        # type: (IrType) -> IrType
        return self._type


class IrCodeSegment:
    def __init__(self, label, instructions):
        # type: (str, List[IrInstruction]) -> None
        self._label = label
        self._instructions = instructions
        self._successors = []      # type: List[IrCodeSegment]
        self._predecessors = []    # type: List[IrCodeSegment]
        for i in self._instructions:
            i._segment = self

    def resolve(self, module, position, equivalence, return_type, return_position):
        # type: (IrModule, IrPosition, IrAddressSpaceInference, IrType, IrPosition) -> IrPosition
        for i in self._instructions:
            position = i.resolve(module, position)
        for i in self._instructions:
            i.resolve_type(equivalence, return_type, return_position)
        return position

    def _create_instance(self, equivalence):
        # type: (Dict[int, int]) -> None
        for i in self._instructions:
            i.create_instance(equivalence)

    def visit(self, generator, context):
        # type: (IrccGenerator, IrCodeGenContext) -> None
        for instruction in self._instructions:
            instruction.register_stack_data(generator, context)


class IrCodeBlock:
    def __init__(self, instructions):
        # type: (List[IrInstruction]) -> None
        self._segments = []    # type: List[IrCodeSegment]
        self._equivalence = IrAddressSpaceInference()
        label = 'start'
        stream = []            # type: List[IrInstruction]
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
            successors = terminal.labels()
            s._successors = [self._find_segment(n) for n in successors]
            for n in s._successors:
                if s not in n._predecessors:
                    n._predecessors.append(s)

    def _find_segment(self, name):
        # type: (str) -> IrCodeSegment
        for s in self._segments:
            if s._label == name:
                return s
        else:
            raise NotImplementedError

    def resolve(self, module, position, return_type, return_position):
        # type: (IrModule, IrPosition, IrType, IrPosition) -> None
        """
            Resolving a method also resolves generic address spaces.
            The resolution step will bucket together types that are constrained into the same generic address space.
            It expects the argument types to seed the buckets.
            The kernel parameters can't use the generic address space; when a kernel method is resolved, it will
            allow to infer the correct address space of all types. Any type whose generic address space can't be
            deduced at compile time will trigger a compile error.
        """
        for s in self._segments:
            position = s.resolve(module, position, self._equivalence, return_type, return_position)
        self._resolve_phi_nodes()

    def _resolve_phi_nodes(self):
        # type: () -> None
        for s in self._segments:
            for inst in s._instructions:
                phi_exprs = inst.collect_phi_exprs()
                if len(phi_exprs) == 0:
                    break
                assert inst._result is not None
                for expr, label in phi_exprs:
                    segment = self._find_segment(label[1:])
                    assert segment in s._predecessors
                    segment._instructions.append(IrAssignInstruction(inst._result, inst.get_type(IrTypeVoid()), expr))

    def _create_instance(self, equivalence):
        # type: (Dict[int, int]) -> None
        for s in self._segments:
            s._create_instance(equivalence)

    def visit(self, generator, equivalence):
        # type: (IrccGenerator, Dict[int, int]) -> None
        context = IrCodeGenContext(equivalence)
        for s in self._segments:
            s.visit(generator, context)


from . import instructions as ir_instructions
if TYPE_CHECKING:
    from typing import Dict, List, Optional, Tuple, Union
    from .ir_module import IrModule
    from .ir_metadata import IrMetadataLink
    from .ir_type import IrType
    from .ir_value import IrValue
    from .ir_expr import IrExpression
    from .ir_reference import IrReference
    from .ir_method import IrMethodParameter
    from .ir_scope import IrScope
    from ..ir_codegen import IrccGenerator
    from ..ir_position import IrPosition