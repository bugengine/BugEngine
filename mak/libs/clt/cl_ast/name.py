from .cppobject import CppObject
from .ast_templates import Template


class Name:
    def __init__(
        self,
        lexer,
        name,
        position,
        target=None,
        template=None,
        arguments=None,
        template_bindings=None,
        parent=None,
        data=None,
        shadow=False,
        errors=[]
    ):
        # type: (ClLexer, str, Position, Optional[CppObject], Optional[Template], Optional[List[Union[Value, TypeRef]]], Optional[ClLexer.TemplateBind], Optional[Name], Optional[CppObject], bool, List[str]) -> None
        self.lexer = lexer
        self.name = name
        self.position = position
        self.target = target
        self.template = template
        self.arguments = arguments
        self.template_bindings = template_bindings or (
            (parent is not None) and parent.template_bindings or None
        )                                                             # type: Optional[ClLexer.TemplateBind]
        self.parent = parent
        self.dependent = parent and parent.dependent or False         # type: bool
        self.errors = errors[:]
        self.shadow = shadow
        if shadow:
            assert not parent
        self.data = data

    def __str__(self):
        # type: () -> str
        if self.arguments:
            name = '%s<%s>' % (self.name, (', '.join(str(a) for a in self.arguments)))
        else:
            name = self.name
        if self.parent:
            return str(self.parent) + '::' + name
        else:
            return name

    def equals(self, other, template_bindings):
        # type: (Optional[Name], Dict[BaseTemplateParameter, Tuple[int, Template]]) -> bool
        if not other:
            return False
        if self.dependent:
            if self.parent is not None and other.parent is not None:
                if not self.parent.equals(other.parent, template_bindings):
                    return False
            if self.name != other.name:
                return False
            if self.template != other.template:
                return False
            if isinstance(self.template, Template):
                assert self.arguments is not None
                if other.arguments is not None:
                    s1 = ';'.join(
                        a.signature(template_bindings) for a in self.template.expand_template_arg_list(self.arguments)
                    )
                    s2 = ';'.join(
                        a.signature(template_bindings) for a in self.template.expand_template_arg_list(other.arguments)
                    )
                    if s1 != s2:
                        return False
                else:
                    return False
            else:
                s1 = ';'.join(a.signature(template_bindings) for a in (self.arguments or []))
                s2 = ';'.join(a.signature(template_bindings) for a in (other.arguments or []))
                if s1 != s2:
                    return False
            return True
        else:
            return self.target == other.target
        return False

    def get_type(self):
        # type: () -> str
        if self.target:
            return self.target.get_token_type()
        else:
            return 'ID'

    def is_dependent(self):
        # type: () -> bool
        return self.dependent

    def is_shadow(self):
        # type: () -> bool
        return self.shadow

    def is_qualified(self):
        # type: () -> bool
        return self.parent != None

    def bind(self):
        # type: () -> Name
        if self.template:
            if not self.template_bindings:
                #assert self.shadow, self
                pass
            else:
                self.template_bindings.template.bind(self.template)
        if self.parent:
            self.parent.bind()
        return self

    def show_errors(self):
        # type: () -> None
        if self.parent:
            self.parent.show_errors()
        for error in self.errors:
            self.lexer.error(error, self.position)


from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from typing import Dict, List, Optional, Tuple, Union
    from ..cl_lexer import ClLexer
    from .position import Position
    from .ast_templates import BaseTemplateParameter
    from .argument_list import ArgumentList
    from .typeref import TypeRef
    from .value import Value
