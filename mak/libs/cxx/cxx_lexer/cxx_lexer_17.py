from .cxx_lexer_14 import CxxLexer14
from .keyword import keywords_17


class CxxLexer17(CxxLexer14):
    keywords = CxxLexer14.keywords + keywords_17
    tokens = CxxLexer14.tokens + keywords_17