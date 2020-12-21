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
        return self

    def _get_type(self, signature):
        # type: (str) -> Optional[IrType]
        assert self._type.is_defined()
        return self._type  #TODO: alloca always __private?


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
        return self

    def _get_type(self, signature):
        # type: (str) -> Optional[IrType]
        t = self._source.get_type(signature)
        assert isinstance(t, IrTypePtr)
        return t._pointee


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
        return self


if TYPE_CHECKING:
    from typing import List, Optional, Tuple
    from ..ir_type import IrType
    from ..ir_value import IrValue
    from ..ir_metadata import IrMetadataLink
    from ..ir_reference import IrReference
    from ..ir_module import IrModule