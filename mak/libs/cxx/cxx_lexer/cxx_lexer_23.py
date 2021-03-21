from .cxx_lexer_20 import CxxLexer20
from .keyword import keywords_23


class CxxLexer23(CxxLexer20):
    keywords = CxxLexer20.keywords + keywords_23
    tokens = CxxLexer20.tokens + keywords_23