class IrccValue:
    def __init__(self, value, precedence):
        # type: (str, int) -> None
        self._value = value
        self._precedence = precedence

    def wrap(self, precedence):
        # type: (int) -> str
        if precedence >= self._precedence:
            return self._value
        else:
            return '(%s)' % self._value
