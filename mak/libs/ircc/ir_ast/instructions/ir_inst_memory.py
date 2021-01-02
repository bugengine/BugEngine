from ..ir_code import IrInstruction
from ..ir_type import IrTypePtr, IrTypeStruct, IrTypePtr, IrTypeArray, IrTypeVector, IrAddressSpace
from ..ir_value import IrValueExpr
from ..ir_expr import IrExpressionConstant
from be_typing import TYPE_CHECKING


class IrInstAlloca(IrInstruction):
    def __init__(self, result, type, count, alignment, address_space, metadata):
        # type: (IrReference, IrType, Optional[IrValue], Optional[int], int, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrInstruction.__init__(self, 'alloca', result, metadata)
        self._type = type
        self._count = count
        self._alignment = alignment
        self._address_space = address_space

    def resolve(self, module):
        # type: (IrModule) -> IrInstruction
        self._type = self._type.resolve(module)
        if self._count is not None:
            self._count = self._count.resolve(module)
        self._value_type = IrTypePtr(self._type.uniquify(), self._address_space)
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
        value_type = self._source.get_type()
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

    def resolve_type(self, equivalence, return_type):
        # type: (IrAddressSpaceInference, IrType) -> None
        target_type = self._target.get_type()
        assert isinstance(target_type, IrTypePtr)
        target_type = target_type._pointee
        value_type = self._value.get_type()
        target_type.add_equivalence(equivalence, value_type)


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
        result_type = self._access[0].get_type()
        address_space = IrAddressSpace(0)
        for value in self._access[1:]:
            result_type = result_type._get_target_type()
            if isinstance(result_type, IrTypeStruct):
                assert isinstance(value, IrValueExpr)
                assert isinstance(value._expression, IrExpressionConstant)
                assert isinstance(value._expression._value, int)
                result_type = result_type.extract(value._expression._value)
            elif isinstance(result_type, IrTypePtr):
                address_space = result_type._address_space
                result_type = result_type._pointee
            elif isinstance(result_type, IrTypeArray):
                result_type = result_type._type
        self._value_type = IrTypePtr(result_type, address_space._address_space)
        return self


if TYPE_CHECKING:
    from typing import List, Optional, Tuple
    from ..ir_value import IrValue
    from ..ir_metadata import IrMetadataLink
    from ..ir_reference import IrReference
    from ..ir_module import IrModule
    from ..ir_type import IrType, IrAddressSpace, IrAddressSpaceInference