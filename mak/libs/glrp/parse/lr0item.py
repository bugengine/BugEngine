from be_typing import TYPE_CHECKING, overload


class LR0Item:
    def __init__(self, rule, index, next, predecessor, successors, follow):
        # type: (Grammar.Rule, int, Optional[LR0Item], Optional[int], List[Grammar.Rule], Set[int]) -> None
        self._rule = rule
        self._symbol = rule._prod_symbol
        self._index = index
        self._next = next
        self._before = predecessor
        self._after = successors
        self._symbols = set(rule._production)
        self._follow = follow
        self._lookaheads = {}      # type: Dict[int, List[int]]
        self._precedence = None    # type: Optional[Tuple[str, int]]
        self._split = False

        if index == len(rule):
            index = -1
        annotations = rule._annotations.get(index, {})
        for annotation, values in annotations.items():
            if annotation == "prec":
                if len(values) == 1:
                    try:
                        precedence = int(values[0])
                    except ValueError:
                        raise SyntaxError(
                            'incorrect precedence: value should be an integer or a pair (string,integer), got %s' %
                            values[0], (rule._filename, rule._lineno, 0, '')
                        )
                    else:
                        self._precedence = ('nonassoc', precedence)
                elif len(values) == 2:
                    if values[0] not in ('left', 'right', 'nonassoc'):
                        raise SyntaxError(
                            'incorrect precedence: value should be an integer or a pair (string,integer), got %s' %
                            ', '.join(values), (rule._filename, rule._lineno, 0, '')
                        )
                    try:
                        precedence = int(values[1])
                    except ValueError:
                        raise SyntaxError(
                            'incorrect precedence: value should be an integer or a pair (string,integer), got %s' %
                            ', '.join(values), (rule._filename, rule._lineno, 0, '')
                        )
                    else:
                        self._precedence = (values[0], precedence)
                else:
                    raise SyntaxError(
                        'incorrect precedence: value should be an integer or a pair (string,integer), got %s' %
                        ', '.join(values), (rule._filename, rule._lineno, 0, '')
                    )
            elif annotation == "split":
                if len(values) != 0:
                    raise SyntaxError(
                        'incorrect annotation: split does not take any argument, got %s' % ','.join(values),
                        (rule._filename, rule._lineno, 0, '')
                    )
                self._split = True
            else:
                raise SyntaxError('unknown annotation %s' % annotation, (rule._filename, rule._lineno, 0, ''))

    def _annotations(self):
        # type: () -> str
        result = ''
        if self._precedence is not None:
            result += '[prec:%s,%d]' % self._precedence
        return result

    def to_string(self, name_map):
        # type: (List[str]) -> str
        return '%s -> %s' % (
            name_map[self._rule._prod_symbol], ' '.join(
                [name_map[p]
                 for p in self._rule._production[:self._index]] + ['%s%s' % (name_map[1], self._annotations())] +
                [name_map[p] for p in self._rule._production[self._index:]]
            )
        )

    def is_reduction_item(self):
        # type: () -> bool
        return self._index == len(self._rule)

    @overload
    def __getitem__(self, index):
        # type: (int) -> int
        return self._rule[index]

    @overload
    def __getitem__(self, index):
        # type: (slice) -> Sequence[int]
        return self._rule[index]

    def __getitem__(self, index):
        # type: (Union[int, slice]) -> Union[int, Sequence[int]]
        return self._rule[index]

    def __len__(self):
        # type: () -> int
        return len(self._rule)


if TYPE_CHECKING:
    from be_typing import Dict, List, Optional, Sequence, Set, Tuple, Union
    from .grammar import Grammar