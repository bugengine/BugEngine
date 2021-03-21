from be_typing import TYPE_CHECKING


class Grammar(object):
    def __init__(self, terminals, rules):
        # type: (Dict[str, int], List[Tuple[str, Parser.Action, List[str]]]) -> None
        self._index = dict(terminals)
        for nonterminal, action, production in rules:
            if nonterminal not in self._index:
                self._index[nonterminal] = 1 + len(self._index)
        for nonterminal, action, production in rules:
            try:
                prod = [self._index[s] for s in production]
            except KeyError as error:
                print(error)

            pass


if TYPE_CHECKING:
    from typing import Any, Dict, List, Tuple
    from .parser import Parser