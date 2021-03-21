from .cxx_lexer_98 import CxxLexer98
from .keyword import keywords_03


class CxxLexer03(CxxLexer98):
    keywords = CxxLexer98.keywords + keywords_03
    tokens = CxxLexer98.tokens + keywords_03