from .ir_object import IrObject
from .ir_expr import IrExpression, IrExpressionDeclaration
from .ir_declaration import IrDeclaration
from .ir_type import IrTypePtr, IrAddressSpaceInference
from .ir_attribute import IrAttributeGroup, IrAttributeGroupObject
from .ir_code import IrCodeBlock
from .ir_scope import IrScope
from ..ir_position import IrPosition
from ..ir_messages import IrAddressSpaceResolutionError
from be_typing import TYPE_CHECKING


class IrMethodParameter(IrExpression):
    def __init__(self, type, name, attributes):
        # type: (IrType, IrReference, IrAttributeGroup) -> None
        IrExpression.__init__(self)
        self._type = type
        self._id = name or '_'
        self._attributes = attributes
        self._position = IrPosition('', 0, 0, 0, '')

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> IrPosition
        self._position = IrExpression.resolve(self, module, position)
        self._type.resolve(module)
        self._attributes = self._attributes.resolve(module)
        return self._position

    def signature(self, resolved_addressspace):
        # type: (Dict[int, int]) -> str
        #return '[%s]%s' % (self._attributes.signature(), self._type.signature())
        return self._type.signature(resolved_addressspace)

    def flatten(self, equivalence):
        # type: (IrAddressSpaceInference) -> List[IrMethodParameter]
        assert isinstance(self._attributes, IrAttributeGroupObject)
        for attr in self._attributes._attributes:
            if attr._attribute == 'byval':
                assert isinstance(self._type, IrTypePtr)
                t = self._type._pointee
                break
        else:
            t = self._type
        result = []
        types = t.flatten(equivalence, self._position)
        if len(types) > 1:
            for i, t in enumerate(types):
                result.append(IrMethodParameter(t, '%s_%d' % (self._id, i), IrAttributeGroupObject([])))
        else:
            result.append(IrMethodParameter(types[0], self._id, IrAttributeGroupObject([])))
        return result

    def get_type(self, suggested_type):
        # type: (IrType) -> IrType
        return self._type


class IrMethodMetadataParameter(IrMethodParameter):
    pass


class IrMethod(IrObject):
    def __init__(self):
        # type: () -> None
        IrObject.__init__(self)

    def resolve(self, module):
        # type: (IrModule) -> None
        raise NotImplementedError

    def find_instance(self, arguments, resolved_addressspace):
        # type: (List[IrType], Dict[int, int]) -> IrMethodObject
        raise NotImplementedError

    def equivalence(self):
        # type: () -> IrAddressSpaceInference
        raise NotImplementedError

    def return_type(self):
        # type: () -> IrType
        raise NotImplementedError

    def parameters(self):
        # type: () -> List[IrMethodParameter]
        raise NotImplementedError


class IrMethodDeclaration(IrDeclaration):
    METHOD_INDEX = 0

    def __init__(self, method, signature=None, equivalence={}):
        # type: (IrMethodObject, Optional[str], Dict[int, int]) -> None
        IrDeclaration.__init__(self)
        self._method = method
        self._method._name = 'method_%d' % IrMethodDeclaration.METHOD_INDEX
        self._signature = signature
        self._equivalence = equivalence
        IrMethodDeclaration.METHOD_INDEX += 1

    def resolve(self, module):
        # type: (IrModule) -> None
        self._method.resolve(module)

    def resolve_content(self, module):
        # type: (IrModule) -> None
        self._method.resolve_definition(module)

    def instantiate(self, module):
        # type: (IrModule) -> None
        self._method.instantiate(module)

    def collect(self, ir_name):
        # type: (str) -> List[Tuple[str, IrDeclaration]]
        self._method.on_collect()
        result = []                                                                                           # type: List[Tuple[str, IrDeclaration]]
        result += [(ir_name + '_spir', IrMethodDeclaration(instance)) for instance in self._method._kernel_instances]
        if len(self._method._instances) > 1:
            result += [
                (ir_name + '_%d' % index, IrMethodDeclaration(instance, signature, equivalence))
                for index, (signature, (instance, equivalence)) in enumerate(self._method._instances.items())
            ]
        else:
            result += [
                (ir_name, IrMethodDeclaration(instance, signature, equivalence))
                for signature, (instance, equivalence) in self._method._instances.items()
            ]
        return result

    def visit(self, generator, name):
        # type: (IrccGenerator, str) -> None
        return_type = self._method._return_type.create_generator_type(generator, self._equivalence)
        parameters = [
            (p._type.create_generator_type(generator, self._equivalence), '_%s' % p._id[1:].replace('.', '_'))
            for p in self._method._parameters
        ]
        if generator.begin_method(
            name[1:], return_type, parameters, self._method._calling_convention, self._method._definition is not None
        ):
            self._method.visit(generator, self._equivalence)
            generator.end_method()


class IrMethodLink(IrMethod):
    def __init__(self, reference):
        # type: (IrReference) -> None
        IrMethod.__init__(self)
        self._reference = reference
        self._method = None    # type: Optional[IrMethod]

    def resolve(self, module):
        # type: (IrModule) -> None
        result = module.get(self._reference, IrMethodDeclaration)
        self._method = result._method

    def find_instance(self, arguments, resolved_addressspace):
        # type: (List[IrType], Dict[int, int]) -> IrMethodObject
        assert self._method is not None
        return self._method.find_instance(arguments, resolved_addressspace)

    def equivalence(self):
        # type: () -> IrAddressSpaceInference
        assert self._method is not None
        return self._method.equivalence()

    def return_type(self):
        # type: () -> IrType
        assert self._method is not None
        return self._method.return_type()

    def parameters(self):
        # type: () -> List[IrMethodParameter]
        assert self._method is not None
        return self._method.parameters()


class IrMethodObject(IrMethod):
    def __init__(self, return_type, parameters, calling_convention, metadata):
        # type: (IrType, List[IrMethodParameter], str, List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrMethod.__init__(self)
        self._return_type = return_type
        self._parameters = parameters
        for i, p in enumerate(self._parameters):
            p._name = 'param_%d' % i
        self._calling_convention = calling_convention
        self._definition = None        # type: Optional[IrMethodBody]
        self._instances = {}           # type: Dict[str, Tuple[IrMethodObject, Dict[int, int]]]
        self._kernel_instances = []    # type: List[IrMethodObject]
        self._declarations = []
        self._scope = IrScope()
        self._metadata = metadata
        self._position = IrPosition('', 0, 0, 0, '')
        for p in self._parameters:
            pdecl = IrExpressionDeclaration(p)
            self._scope.declare(p._id, pdecl)
            self._declarations.append(pdecl)

    def equivalence(self):
        # type: () -> IrAddressSpaceInference
        if self._definition:
            return self._definition._code._equivalence
        else:
            return IrAddressSpaceInference()

    def return_type(self):
        # type: () -> IrType
        return self._return_type

    def parameters(self):
        # type: () -> List[IrMethodParameter]
        return self._parameters

    def define(self, instruction_list):
        # type: (List[IrInstruction]) -> None
        self._definition = IrMethodBody(instruction_list)

    def instantiate(self, module):
        # type: (IrModule) -> None
        if self._calling_convention == 'spir_kernel':
            assert self._definition is not None
            # create an intermediate kernel entry point
            method = self._create_kernel_wrapper()
            try:
                resolved_params = self._definition._code._equivalence.create_direct_map()
            except IrAddressSpaceResolutionError as e:
                module.logger().C0100(e.position, e.type, str(e))
                while e.parent is not None:
                    e = e.parent
                    module.logger().C0101(e.position, str(e))
            else:
                self.find_instance([p._type for p in self._parameters], resolved_params)
                self._kernel_instances.append(method)

    def on_collect(self):
        # type: () -> None
        pass

    def _create_kernel_wrapper(self):
        # type: () -> IrMethodObject
        assert self._definition is not None
        parameters = []    # type: List[IrMethodParameter]
        for p in self._parameters:
            parameters += p.flatten(self._definition._code._equivalence)
        result = IrMethodObject(self._return_type, parameters, 'spir_kernel_flat', self._metadata)
        return result

    def resolve(self, module):
        # type: (IrModule) -> None
        for name, value in self._metadata:
            value.resolve(module, self._position)
            if name._reference == '!dbg':
                self._position = value.get_position()
        module.push_scope(self._scope)
        try:
            self._return_type.resolve(module)
            for p in self._parameters:
                p.resolve(module, self._position)
        finally:
            module.pop_scope()

    def resolve_definition(self, module):
        # type: (IrModule) -> None
        module.push_scope(self._scope)
        try:
            if self._definition is not None:
                self._definition = self._definition.resolve(
                    module, self._position, self._parameters, self._return_type, self._position
                )
        finally:
            module.pop_scope()

    def find_instance(self, arguments, resolved_addressspace):
        # type: (List[IrType], Dict[int, int]) -> IrMethodObject
        signature = ','.join(a.signature(resolved_addressspace) for a in arguments)
        try:
            return self._instances[signature][0]
        except KeyError:
            result = self._create_instance(arguments, signature, resolved_addressspace)
            self._instances[signature] = (result, resolved_addressspace)
            return result

    def _create_instance(self, arguments, signature, resolved_addressspace):
        # type: (List[IrType], str, Dict[int, int]) -> IrMethodObject
        for parameter in self._parameters:
            parameter._type.create_instance(resolved_addressspace)
        if self._definition is not None:
            assert len(self._declarations) == len(arguments)
            self._definition._create_instance(resolved_addressspace)
            return self
        else:
            # TODO
            return self

    def visit(self, generator, equivalence):
        # type: (IrccGenerator, Dict[int, int]) -> None
        if self._definition is not None:
            self._definition.visit(generator, equivalence)

    def get_position(self):
        # type: () -> IrPosition
        return self._position


class IrMethodBody:
    def __init__(self, instruction_list):
        # type: (List[IrInstruction]) -> None
        self._scope = IrScope()
        for instruction in instruction_list:
            instruction.declare(self._scope)
        self._code = IrCodeBlock(instruction_list)

    def resolve(self, module, position, parameters, return_type, return_position):
        # type: (IrModule, IrPosition, List[IrMethodParameter], IrType, IrPosition) -> IrMethodBody
        module.push_scope(self._scope)
        try:
            self._code.resolve(module, position, return_type, return_position)
        finally:
            module.pop_scope()
        return self

    def visit(self, generator, equivalence):
        # type: (IrccGenerator, Dict[int, int]) -> None
        self._code.visit(generator, equivalence)

    def _create_instance(self, equivalence):
        # type: (Dict[int, int]) -> None
        self._code._create_instance(equivalence)


if TYPE_CHECKING:
    from typing import Dict, List, Optional, Tuple
    from .ir_module import IrModule
    from .ir_reference import IrReference
    from .ir_code import IrInstruction
    from .ir_type import IrType
    from .ir_metadata import IrMetadataLink
    from ..ir_codegen import IrccGenerator
