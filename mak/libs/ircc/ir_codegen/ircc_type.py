from be_typing import TYPE_CHECKING
try:
    from itertools import izip_longest as zip_longest
except ImportError:
    from itertools import zip_longest  # type: ignore


class IrccType:
    def __init__(self, declaration, base=None, sep=' '):
        # type: (List[str], Optional[IrccType], str) -> None
        if base is None:
            self._declaration = declaration
        else:
            self._declaration = [sep.join(x).strip() for x in zip(base._declaration, declaration)]

    def format(self, names=[]):
        # type: (List[str]) -> str
        decl = [(a + ' ' + b).strip() for a, b in zip_longest(self._declaration, names, fillvalue='')]
        return ' '.join(d for d in decl if d)


if TYPE_CHECKING:
    from typing import List, Optional