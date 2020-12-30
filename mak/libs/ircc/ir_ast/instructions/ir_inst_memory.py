from ..ir_code import IrInstruction
from ..ir_type import IrTypePtr, IrTypeStruct, IrTypePtr, IrTypeArray, IrTypeVector
from ..ir_value import IrValueExpr
from ..ir_expr import IrExpressionConstant
from be_typing import TYPE_CHECKING


class IrInstAlloca(IrInstruction):
    def __init__(self, result, type, count, alignment, metadata):
        # type: (IrReference, IrType, Optional[IrValue], Optional[int], List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'alloca', result, metadata)
        self._type = type
        self._count = count

    def resolve(self, module):
        # type: (IrModule) -> IrInstruction
        self._type = self._type.resolve(module)
        if self._count is not None:
            self._count = self._count.resolve(module)
        self._value_type = IrTypePtr(self._type.uniquify(), 0)
        return self


class IrInstLoad(IrInstruction):
    def __init__(self, result, source, type, metadata):
        # type: (IrReference, IrValue, IrType, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'load', result, metadata)
        self._source = source
        self._type = type

    def resolve(self, module):
        # type: (IrModule) -> IrInstruction
        self._source = self._source.resolve(module)
        self._type = self._type.resolve(module)
        value_type = self._source.get_type()[0]
        assert isinstance(value_type, IrTypePtr)
        self._value_type = value_type._pointee
        return self


class IrInstStore(IrInstruction):
    def __init__(self, target, value, metadata):
        # type: (IrValue, IrValue, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'store', None, metadata)
        self._target = target
        self._value = value

    def resolve(self, module):
        # type: (IrModule) -> IrInstruction
        self._target = self._target.resolve(module)
        self._value = self._value.resolve(module)
        return self


class IrInstGetElementPtr(IrInstruction):
    def __init__(self, result, type, access, metadata):
        # type: (IrReference, IrType, List[IrValue], List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'getelementptr', result, metadata)
        self._type = type
        self._access = access

    def resolve(self, module):
        # type: (IrModule) -> IrInstruction
        self._type = self._type.resolve(module)
        self._access = [access.resolve(module) for access in self._access]
        result_type, address_space = self._access[0].get_type()
        for value in self._access[1:]:
            result_type = result_type._get_target_type()
            if isinstance(result_type, IrTypeStruct):
                assert isinstance(value, IrValueExpr)
                assert isinstance(value._expression, IrExpressionConstant)
                assert isinstance(value._expression._value, int)
                result_type = result_type.extract(value._expression._value)
            elif isinstance(result_type, IrTypePtr):
                result_type = result_type._pointee
            elif isinstance(result_type, IrTypeArray):
                result_type = result_type._type
        self._value_type = IrTypePtr(result_type, int(address_space))
        return self


if TYPE_CHECKING:
    from typing import List, Optional, Tuple
    from ..ir_value import IrValue
    from ..ir_metadata import IrMetadataLink
    from ..ir_reference import IrReference
    from ..ir_module import IrModule
    from ..ir_type import IrType, IrAddressSpace, IrAddressSpaceInference