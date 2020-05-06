from be_typing import TYPE_CHECKING


class IrHeader:
    def __init__(self, name, value):
        # type: (str, str) -> None
        self._name = name
        self._value = value
