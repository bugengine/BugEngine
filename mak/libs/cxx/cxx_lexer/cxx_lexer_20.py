from .cxx_lexer_17 import CxxLexer17
from .keyword import keywords_20


class CxxLexer20(CxxLexer17):
    keywords = CxxLexer17.keywords + keywords_20
    tokens = CxxLexer17.tokens + keywords_20