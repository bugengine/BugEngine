from .cxx_lexer_03 import CxxLexer03
from .keyword import keywords_11
from be_typing import TYPE_CHECKING


class CxxLexer11(CxxLexer03):
    keywords = CxxLexer03.keywords + keywords_11
    tokens = CxxLexer03.tokens + ('[[', ) + keywords_11

    def token(self):
        # type: () -> Generator[glrp.Token, None, None]
        for token in CxxLexer03.token(self):
            if token._name == '[':
                try:
                    if self.look_ahead(0)._name == '[':
                        self.set_token_type(token, '[[')
                except StopIteration:
                    pass
            yield token


if TYPE_CHECKING:
    from typing import Generator
    import glrp
