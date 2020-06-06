from .ir_object import IrObject
from .ir_declaration import IrDeclaration
from be_typing import TYPE_CHECKING


class IrMethodParameter(IrObject):
    def __init__(self, type, name, attributes):
        # type: (IrType, IrReference, List[str]) -> None
        IrObject.__init__(self)
        self._type = type
        self._id = name

    def resolve(self, module):
        # type: (IrModule) -> IrMethodParameter
        self._type = self._type.resolve(module)
        return self


class IrMethodMetadataParameter(IrMethodParameter):
    pass


class IrMethod(IrObject):
    def __init__(self):
        IrObject.__init__(self)

    def resolve(self, module):
        # type: (IrModule) -> IrMethod
        return self


class IrMethodDeclaration(IrDeclaration):
    METHOD_INDEX = 0
    def __init__(self, method):
        # type: (IrMethodObject) -> None
        self._method = method
        self._method._name = 'method_%d' % IrMethodDeclaration.METHOD_INDEX
        IrMethodDeclaration.METHOD_INDEX += 1

    def resolve(self, module):
        # type: (IrModule) -> IrDeclaration
        self._method = self._method.resolve(module)
        return self

    def collect(self, ir_name):
        # type: (str) -> List[Tuple[str, IrDeclaration]]
        assert self._method is not None
        return [(ir_name+'_%d'%index, IrMethodDeclaration(instance)) for index, instance in enumerate(self._method._instances)]

    def visit(self, generator, name):
        # type: (IrccGenerator, str) -> None
        if self._method._return_type is not None:
            return_type = self._method._return_type.create_generator_type(generator)
        else:
            return_type = generator.type_void()
        parameters = [(p._type.create_generator_type(generator), p._name) for p in self._method._parameters]
        generator.begin_method(self._method._name, return_type, parameters)
        generator.end_method()

class IrMethodLink(IrMethod):
    def __init__(self, reference):
        # type: (IrReference) -> None
        IrMethod.__init__(self)
        self._reference = reference

    def resolve(self, module):
        # type: (IrModule) -> IrMethod
        result = module.get(self._reference, IrMethodDeclaration)
        assert isinstance(result, IrMethod)
        result.resolve(module)
        return result


class IrMethodObject(IrMethod):
    def __init__(self, return_type, parameters, calling_convention):
        # type: (Optional[IrType], List[IrMethodParameter], str) -> None
        IrMethod.__init__(self)
        self._return_type = return_type
        self._parameters = parameters
        for i, p in enumerate(self._parameters):
            p._name = 'param_%d'%i
        self._calling_convention = calling_convention
        self._definition = None  # type: Optional[IrMethodBody]
        self._instances = []  # type: List[IrMethodObject]

    def define(self, instruction_list):
        # type: (List[IrInstruction]) -> None
        self._definition = IrMethodBody(instruction_list)
        if self._calling_convention == 'spir_kernel':
            self._instances.append(self)
            self._definition._instantiate()

    def resolve(self, module):
        # type: (IrModule) -> IrMethodObject
        if self._return_type:
            self._return_type = self._return_type.resolve(module)
        self._parameters = [p.resolve(module) for p in self._parameters]
        return self


class IrMethodBody:
    def __init__(self, instruction_list):
        # type: (List[IrInstruction]) -> None
        self._instruction_list = instruction_list

    def _instantiate(self):
        pass


if TYPE_CHECKING:
    from typing import List, Optional, Tuple
    from .ir_module import IrModule
    from .ir_reference import IrReference
    from .ir_instruction import IrInstruction
    from .ir_type import IrType
    from ..ir_codegen import IrccGenerator