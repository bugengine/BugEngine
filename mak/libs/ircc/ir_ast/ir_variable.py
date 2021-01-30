from .ir_declaration import IrDeclaration
from .ir_expr import IrExpression, IrExpressionDeclaration
from .ir_type import IrAddressSpace, IrTypePtr
from .ir_code import IrCodeGenContext
from be_typing import TYPE_CHECKING, cast


class IrVariableDeclaration(IrExpressionDeclaration):
    def __init__(self, variable):
        # type: (IrVariable) -> None
        IrExpressionDeclaration.__init__(self, variable)

    def collect(self, ir_name):
        # type: (str) -> List[Tuple[str, IrDeclaration]]
        return [(ir_name, self)]

    def visit(self, generator, ir_name):
        # type: (IrccGenerator, str) -> None
        assert isinstance(self._expression, IrVariable)
        context = IrCodeGenContext({}, None)
        if self._expression._initial_value is not None:
            generator.declare_variable(
                self._expression._variable_name,
                generator.make_address_space(
                    self._expression._type.create_generator_type(generator, context._equivalence),
                    self._expression._address_space._address_space
                ), self._expression._initial_value.create_generator_value(self._expression._type, generator, context)
            )
        else:
            generator.declare_variable(
                self._expression._variable_name,
                generator.make_address_space(
                    self._expression._type.create_generator_type(generator, context._equivalence),
                    self._expression._address_space._address_space
                )
            )


class IrVariable(IrExpression):
    def __init__(self, name, type, initital_value, address_space, metadata):
        # type: (str, IrType, Optional[IrExpression], int, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrExpression.__init__(self, metadata)
        self._variable_name = 's_%s' % (name[1:].replace('.', '_'))
        self._type = type
        self._initial_value = initital_value
        self._address_space = IrAddressSpace(address_space)

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> IrPosition
        self._type.resolve(module)
        if self._initial_value is not None:
            self._initial_value.resolve(module, position)
        return IrExpression.resolve(self, module, position)

    def get_type(self, suggested_type):
        # type: (IrType) -> IrType
        return IrTypePtr(self._type, self._address_space._address_space)

    def create_generator_value(self, type, generator, code_context):
        # type: (IrType, IrccGenerator, IrCodeGenContext) -> IrccExpression
        return generator.make_expression_address(generator.make_expression_variable_reference(self._variable_name))


if TYPE_CHECKING:
    from typing import List, Optional, Tuple
    from .ir_type import IrType
    from .ir_module import IrModule
    from .ir_reference import IrReference
    from .ir_metadata import IrMetadataLink
    from ..ir_codegen import IrccGenerator, IrccExpression
    from ..ir_position import IrPosition
