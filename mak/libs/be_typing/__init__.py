MYPY = False
if MYPY:
    from typing import *
else:

    class _Index(object):
        def __getitem__(self, key):
            return None

        def __call__(self, *args, **kw_args):
            pass

    TYPE_CHECKING = False
    Tuple = _Index()
    List = _Index()
    TypeVar = _Index()

    def cast(a, b):
        return b
