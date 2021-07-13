from be_typing import TYPE_CHECKING


class LR0Path(object):
    def __init__(self, node, use_marker=True):
        # type: (LR0DominanceNode, bool) -> None
        self._node = node
        self._use_marker = use_marker
        self._hash = (node._item, )    # type: Tuple[LR0Item,...]

    def __hash__(self):
        # type: () -> int
        return hash(self._hash)

    def __eq__(self, other):
        # type: (Any) -> bool
        return isinstance(other, LR0Path) and self._hash == other._hash

    def extend(self, node, lookahead):
        # type: (LR0DominanceNode, int) -> LR0Path
        return _LR0Extension(node, self)

    def derive_from(self, node):
        # type: (LR0DominanceNode) -> LR0Path
        return _LR0Derivation(node, self)

    def expand_left(self):
        # type: () -> LR0Path
        return _LR0LeftExpansion(self._node, self)

    def expand_next(self, path):
        # type: (LR0Path) -> LR0Path
        return _LR0Expansion(self._node, self, path)

    def to_string(self, name_map):
        # type: (List[str]) -> Tuple[List[Text], int]
        return ([''], 0)

    def _to_string(self, sequence, name_map):
        # type: (List[int], List[str]) -> Tuple[List[Text], int]
        return ([''], 0)
        expanded_symbol = name_map[self._node._item._symbol]
        if len(self._sequence) == 0:
            return [u'', u'\u2570%s\u256f' % expanded_symbol], len(expanded_symbol) + 2
        buffer, length = self._sequence[0].to_string(name_map)
        for item in self._sequence[1:]:
            temp = buffer
            extension, ext_length = item.to_string(name_map)
            buffer = [u'%s %s' % (i.ljust(length), j) for i, j in zip(temp, extension)]
            buffer += temp[len(extension):]
            buffer += [(1 + length) * u' ' + j for j in extension[len(temp):]]
            length += 1 + ext_length

        extra_padding = u'\u2500' * (length - 2 - len(expanded_symbol))
        buffer.append(u'\u2570%s%s\u256f' % (expanded_symbol, extra_padding))
        return buffer, max(length, len(buffer[-1]))


class _LR0BaseConstruction(LR0Path):
    def __init__(self, node, follow):
        # type: (LR0DominanceNode, LR0Path) -> None
        self._node = node
        self._follow = follow
        self._hash = (node._item, ) + follow._hash # type: Tuple[LR0Item,...]


class _LR0Extension(_LR0BaseConstruction):
    def to_string(self, name_map):
        # type: (List[str]) -> Tuple[List[Text], int]
        return [''], 0


class _LR0Derivation(_LR0BaseConstruction):
    def to_string(self, name_map):
        # type: (List[str]) -> Tuple[List[Text], int]
        return [''], 0


class _LR0LeftExpansion(_LR0BaseConstruction):
    def to_string(self, name_map):
        # type: (List[str]) -> Tuple[List[Text], int]
        return [''], 0


class _LR0Expansion(_LR0BaseConstruction):
    def __init__(self, node, follow, expanded_path):
        # type: (LR0DominanceNode, LR0Path, LR0Path) -> None
        self._node = node
        self._follow = follow
        self._hash = (node._item, ) + follow._hash # type: Tuple[LR0Item,...]
        self._next = expanded_path

    def to_string(self, name_map):
        # type: (List[str]) -> Tuple[List[Text], int]
        return [''], 0


if TYPE_CHECKING:
    from be_typing import Any, List, Optional, Text, Tuple, Union
    from .lr0dominancenode import LR0DominanceNode
    from .lr0item import LR0Item
