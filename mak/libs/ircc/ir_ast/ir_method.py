from .ir_object import IrObject
from .ir_expr import IrExpression, IrExpressionDeclaration
from .ir_declaration import IrDeclaration
from .ir_type import IrTypePtr
from .ir_attribute import IrAttributeGroup, IrAttributeGroupObject
from .ir_code import IrCodeBlock
from .ir_scope import IrScope
from be_typing import TYPE_CHECKING


class IrMethodParameter(IrExpression):
    def __init__(self, type, name, attributes):
        # type: (IrType, IrReference, IrAttributeGroup) -> None
        IrObject.__init__(self)
        self._type = type
        self._id = name or '_'
        self._attributes = attributes

    def resolve(self, module):
        # type: (IrModule) -> IrMethodParameter
        self._type = self._type.resolve(module).uniquify()
        self._attributes = self._attributes.resolve(module)
        return self

    def signature(self):
        # type: () -> str
        #return '[%s]%s' % (self._attributes.signature(), self._type.signature())
        return self._type.signature()

    def flatten(self):
        # type: () -> List[IrMethodParameter]
        assert isinstance(self._attributes, IrAttributeGroupObject)
        for attr in self._attributes._attributes:
            if attr._attribute == 'byval':
                assert isinstance(self._type, IrTypePtr)
                t = self._type._pointee
                break
        else:
            t = self._type
        result = []
        types = t.flatten()
        if len(types) > 1:
            for i, t in enumerate(types):
                result.append(IrMethodParameter(t, '%s_%d' % (self._id, i), IrAttributeGroupObject([])))
        else:
            result.append(IrMethodParameter(types[0], self._id, IrAttributeGroupObject([])))
        return result

    def get_type(self):
        # type: () -> IrType
        return self._type


class IrMethodMetadataParameter(IrMethodParameter):
    pass


class IrMethod(IrObject):
    def __init__(self):
        # type: () -> None
        IrObject.__init__(self)

    def resolve(self, module):
        # type: (IrModule) -> IrMethodObject
        raise NotImplementedError

    def find_instance(self, arguments):
        # type: (List[IrType]) -> IrMethodObject
        raise NotImplementedError


class IrMethodDeclaration(IrDeclaration):
    METHOD_INDEX = 0

    def __init__(self, method, signature=None):
        # type: (IrMethodObject, Optional[str]) -> None
        IrDeclaration.__init__(self)
        self._method = method
        self._method._name = 'method_%d' % IrMethodDeclaration.METHOD_INDEX
        self._method_resolution = None # type: Optional[IrMethodObject]
        self._signature = signature
        IrMethodDeclaration.METHOD_INDEX += 1

    def resolve(self, module):
        # type: (IrModule) -> IrDeclaration
        if self._method_resolution is None:
            self._method_resolution = self._method.resolve(module)
            self._method_resolution.resolve_definition(module)
        return self

    def instantiate(self):
        # type: () -> None
        assert self._method_resolution is not None
        self._method_resolution.instantiate()

    def collect(self, ir_name):
        # type: (str) -> List[Tuple[str, IrDeclaration]]
        self._method.on_collect()
        result = []                                                                            # type: List[Tuple[str, IrDeclaration]]
        result += [(ir_name + '_spir', IrMethodDeclaration(instance)) for instance in self._method._kernel_instances]
        if len(self._method._instances) > 1:
            result += [
                (ir_name + '_%d' % index, IrMethodDeclaration(instance, signature))
                for index, (signature, instance) in enumerate(self._method._instances.items())
            ]
        else:
            result += [
                (ir_name, IrMethodDeclaration(instance, signature))
                for signature, instance in self._method._instances.items()
            ]
        return result

    def visit(self, generator, name):
        # type: (IrccGenerator, str) -> None
        if self._method._return_type is not None:
            return_type = self._method._return_type.create_generator_type(generator)
        else:
            return_type = generator.type_void()
        parameters = [(p._type.create_generator_type(generator), p._id[1:]) for p in self._method._parameters]
        if generator.begin_method(name[1:], return_type, parameters, self._method._calling_convention):
            self._method.visit(generator, self._signature)
            generator.end_method()


class IrMethodLink(IrMethod):
    def __init__(self, reference):
        # type: (IrReference) -> None
        IrMethod.__init__(self)
        self._reference = reference

    def resolve(self, module):
        # type: (IrModule) -> IrMethodObject
        result = module.get(self._reference, IrMethodDeclaration)
        result.resolve(module)
        return result._method


class IrMethodObject(IrMethod):
    def __init__(self, return_type, parameters, calling_convention):
        # type: (IrType, List[IrMethodParameter], str) -> None
        IrMethod.__init__(self)
        self._return_type = return_type
        self._parameters = parameters
        for i, p in enumerate(self._parameters):
            p._name = 'param_%d' % i
        self._calling_convention = calling_convention
        self._definition = None        # type: Optional[IrMethodBody]
        self._instances = {}           # type: Dict[str, IrMethodObject]
        self._kernel_instances = []    # type: List[IrMethodObject]
        self._declarations = []
        self._scope = IrScope()
        for p in self._parameters:
            pdecl = IrExpressionDeclaration(p)
            self._scope.declare(p._id, pdecl)
            self._declarations.append(pdecl)

    def define(self, instruction_list):
        # type: (List[IrInstruction]) -> None
        self._definition = IrMethodBody(instruction_list)

    def instantiate(self):
        # type: () -> None
        if self._calling_convention == 'spir_kernel':
            assert self._definition is not None
            self.find_instance([p._type for p in self._parameters])
            # create an intermediate kernel entry point
            self._kernel_instances.append(self._create_kernel_wrapper())

    def on_collect(self):
        # type: () -> None
        pass

    def _create_kernel_wrapper(self):
        # type: () -> IrMethodObject
        parameters = []    # type: List[IrMethodParameter]
        for p in self._parameters:
            parameters += p.flatten()
        result = IrMethodObject(self._return_type, parameters, 'spir_kernel_flat')
        return result

    def resolve(self, module):
        # type: (IrModule) -> IrMethodObject
        module.push_scope(self._scope)
        try:
            self._parameters = [p.resolve(module) for p in self._parameters]
        finally:
            module.pop_scope()
        return self

    def resolve_definition(self, module):
        # type: (IrModule) -> None
        module.push_scope(self._scope)
        try:
            self._return_type = self._return_type.resolve(module)
            if self._definition is not None:
                self._definition = self._definition.resolve(module, self._parameters, self._return_type)
        finally:
            module.pop_scope()

    def find_instance(self, arguments):
        # type: (List[IrType]) -> IrMethodObject
        signature = ','.join(a.signature() for a in arguments)
        try:
            return self._instances[signature]
        except KeyError:
            result = self._create_instance(arguments, signature)
            self._instances[signature] = result
            return result

    def _create_instance(self, arguments, signature):
        # type: (List[IrType], str) -> IrMethodObject
        if self._definition:
            assert len(self._declarations) == len(arguments)
            return self
        else:
            # TODO
            return self

    def visit(self, generator, signature):
        # type: (IrccGenerator, Optional[str]) -> None
        pass


class IrMethodBody:
    def __init__(self, instruction_list):
        # type: (List[IrInstruction]) -> None
        self._scope = IrScope()
        for instruction in instruction_list:
            instruction.declare(self._scope)
        self._code = IrCodeBlock(instruction_list)

    def resolve(self, module, parameters, return_type):
        # type: (IrModule, List[IrMethodParameter], IrType) -> IrMethodBody
        module.push_scope(self._scope)
        try:
            self._code = self._code.resolve(module, return_type)
        finally:
            module.pop_scope()
        return self


if TYPE_CHECKING:
    from typing import Dict, List, Optional, Tuple
    from .ir_module import IrModule
    from .ir_reference import IrReference
    from .ir_code import IrInstruction
    from .ir_type import IrType
    from ..ir_codegen import IrccGenerator