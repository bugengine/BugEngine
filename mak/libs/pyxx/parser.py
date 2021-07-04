from .c import CParser, C89Parser, C99Parser, C11Parser, C17Parser
from .cxx import CxxParser, Cxx98Parser, Cxx03Parser, Cxx11Parser, Cxx14Parser, Cxx17Parser, Cxx20Parser, Cxx23Parser
from .objc import ObjC89Parser, ObjC99Parser, ObjC11Parser, ObjC17Parser
from .objc import ObjCxx98Parser, ObjCxx03Parser, ObjCxx11Parser, ObjCxx14Parser, ObjCxx17Parser, ObjCxx20Parser, ObjCxx23Parser

_parsers = {
    'c': {
        'c89': C89Parser,
        'c99': C99Parser,
        'c11': C11Parser,
        'c17': C17Parser,
    },
    'c++':
        {
            'c++98': Cxx98Parser,
            'c++03': Cxx03Parser,
            'c++11': Cxx11Parser,
            'c++14': Cxx14Parser,
            'c++17': Cxx17Parser,
            'c++20': Cxx20Parser,
            'c++23': Cxx23Parser,
        },
    'objc': {
        'c89': ObjC89Parser,
        'c99': ObjC99Parser,
        'c11': ObjC11Parser,
        'c17': ObjC17Parser,
    },
    'objc++':
        {
            'c++98': ObjCxx98Parser,
            'c++03': ObjCxx03Parser,
            'c++11': ObjCxx11Parser,
            'c++14': ObjCxx14Parser,
            'c++17': ObjCxx17Parser,
            'c++20': ObjCxx20Parser,
            'c++23': ObjCxx23Parser,
        },
}                                      # type: Dict[str, Dict[str, Union[Type[CParser], Type[CxxParser]]]]


def parser(lang, extension, std):
    # type: (str, str, str) -> Union[Type[CParser], Type[CxxParser]]
    if lang == 'auto':
        if extension == '.c':
            lang = 'c'
        elif extension in ['.cc', '.cpp', '.cxx', '.C', '.CC']:
            lang = 'c++'
        elif extension == 'm':
            lang = 'objc'
        elif extension in ('mm', '.h', '.hh'):
            lang = 'objc++'
        else:
            lang = 'c'
    return _parsers[lang][std]


from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from typing import Dict, Type, Union
    import glrp
