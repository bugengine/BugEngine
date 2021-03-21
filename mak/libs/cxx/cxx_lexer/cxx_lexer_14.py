from .cxx_lexer_11 import CxxLexer11
from .keyword import keywords_14


class CxxLexer14(CxxLexer11):
    keywords = CxxLexer11.keywords + keywords_14
    tokens = CxxLexer11.tokens + keywords_14