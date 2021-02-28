from .lexer17 import CppLexer17
from .keyword import keywords_20


class CppLexer20(CppLexer17):
    keywords = CppLexer17.keywords + keywords_20
    tokens = CppLexer17.tokens + tuple('KW_' + kw.upper() for kw in keywords_20)