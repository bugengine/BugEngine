from typing import cast, TYPE_CHECKING


class ArgumentList:
    def __init__(self, arguments):
        # type: (Dict[int,CppObject]) -> None
        args = [None] * len(arguments) # type: List[Optional[CppObject]]

        for index, arg in arguments.items():
            args[index] = arg
        self._arguments = cast('List[CppObject]', args)
        self._signature_cache = {'': ';'.join(a.signature() for a in self._arguments)}

    def __len__(self):
        # type: () -> int
        return len(self._arguments)

    def __bool__(self):
        # type: () -> bool
        return len(self._arguments) > 0

    def __getitem__(self, index):
        # type: (int) -> CppObject
        return self._arguments[index]

    def signature(self, template_bindings=None):
        # type: (Dict[BaseTemplateParameter, Tuple[int, Template]]) -> str
        if not template_bindings:
            return self._signature_cache['']
        return ';'.join(a.signature(template_bindings) for a in self._arguments)
        # TODO: cache

    def __str__(self):
        # type: () -> str
        return ', '.join(str(a) for a in self._arguments)


if TYPE_CHECKING:
    from typing import Dict, List, Optional, Tuple
    from .cppobject import CppObject
    from .ast_templates import Template, BaseTemplateParameter
