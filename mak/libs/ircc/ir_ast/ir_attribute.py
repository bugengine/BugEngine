from .ir_declaration import IrDeclaration
from be_typing import TYPE_CHECKING


class IrAttribute:
    def __init__(self, attribute, parameters=[]):
        # type: (str, List[IrValue]) -> None
        self._attribute = attribute
        self._parameters = parameters

    def resolve(self, module):
        # type: (IrModule) -> List[IrAttribute]
        return [self]

    def __str__(self):
        # type: () -> str
        return self._attribute + (('(%s)'%', '.join(str(value) for value in self._parameters)) if self._parameters else '')


class IrAttributeGroup:
    def resolve(self, module):
        # type: (IrModule) -> IrAttributeGroup
        return self


class IrAttributeGroupObject(IrAttributeGroup):
    def __init__(self, attribute_list):
        # type: (List[IrAttribute]) -> None
        self._attributes = attribute_list

    def resolve(self, module):
        # type: (IrModule) -> IrAttributeGroup
        self._attributes = [a for attribute in self._attributes for a in attribute.resolve(module)]
        return self
    
    def __str__(self):
        # type: () -> str
        return '; '.join(str(attr) for attr in self._attributes)


class IrAttributeGroupDeclaration(IrDeclaration):
    def __init__(self, attribute_group):
        # type: (IrAttributeGroup) -> None
        self._attribute_group = attribute_group

    def resolve(self, module):
        # type: (IrModule) -> IrDeclaration
        self._attribute_group = self._attribute_group.resolve(module)
        return self

    def write_declaration(self, declared_name):
        # type: (IrReference) -> None
        #print('attributes - ', declared_name, self._attribute_group)
        pass


class IrAttributeGroupLink(IrAttributeGroup):
    def __init__(self, reference):
        # type: (IrReference) -> None
        self._reference = reference

    def resolve(self, module):
        # type: (IrModule) -> IrAttributeGroup
        return module.get(self._reference, IrAttributeGroupDeclaration)._attribute_group


if TYPE_CHECKING:
    from typing import List, Optional
    from .ir_module import IrModule
    from .ir_reference import IrReference
    from .ir_value import IrValue