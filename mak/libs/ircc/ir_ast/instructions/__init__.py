from .ir_inst_label import IrInstLabel
from .ir_inst_aggregate import IrInstExtractValue, IrInstInsertValue
from .ir_inst_terminate import IrInstBranch, IrInstConditionalBranch, IrInstRet, IrInstSwitch, IrInstUnreachable
from .ir_inst_binary_op import IrInstBinaryOp, IrInstFloatBinaryOp
from .ir_inst_unary_op import IrInstFloatUnaryOp
from .ir_inst_conversion import IrInstCast
from .ir_inst_memory import IrInstAlloca, IrInstLoad, IrInstStore, IrInstGetElementPtr
from .ir_inst_vector import IrInstExtractElement, IrInstInsertElement, IrInstShuffleVector
from .ir_inst_other import IrInstCall, IrInstIntegerCompare, IrInstFloatCompare, IrInstPhi, IrInstSelect