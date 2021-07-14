from be_typing import TYPE_CHECKING


class LR0Path(object):
    def __init__(self, node, use_marker=True):
        # type: (LR0DominanceNode, bool) -> None
        self._node = node
        self._use_marker = use_marker
        self._hash_cache = (self._node._item, ) # type: Optional[Tuple[LR0Item,...]]

    def _hash(self):
        # type: () -> Tuple[LR0Item,...]
        return (self._node._item, )

    def __hash__(self):
        # type: () -> int
        if self._hash_cache is None:
            self._hash_cache = self._hash()
        return hash(self._hash_cache)

    def __eq__(self, other):
        # type: (Any) -> bool
        return isinstance(other, LR0Path) and self._hash() == other._hash()

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
        return self._to_string([], name_map, True, True)

    def _to_string(self, sequence, name_map, add_derivation, complete_right):
        # type: (List[int], List[str], bool, bool) -> Tuple[List[Text], int]
        expanded_symbol = name_map[self._node._item._symbol]
        if self._use_marker:
            sequence.append(1)
        if complete_right:
            sequence += self._node._item.rule.production[self._node._item._index:]
        sequence_str = u' '.join(name_map[i] if i != 1 else '\u2666' for i in sequence)
        if add_derivation:
            extra_padding = u'\u2500' * (len(sequence_str) - 2 - len(expanded_symbol))
            derivation_str = u'\u2570%s%s\u256f' % (expanded_symbol, extra_padding)
            return [sequence_str, derivation_str], max(len(sequence_str), len(derivation_str))
        else:
            return [sequence_str], len(sequence_str)


class _LR0BaseConstruction(LR0Path):
    def __init__(self, node, follow):
        # type: (LR0DominanceNode, LR0Path) -> None
        self._node = node
        self._follow = follow
        self._hash_cache = None

    def _hash(self):
        # type: () -> Tuple[LR0Item,...]
        if self._hash_cache is None:
            self._hash_cache = (self._node._item, ) + self._follow._hash()
        return self._hash_cache


class _LR0Extension(_LR0BaseConstruction):
    def _to_string(self, sequence, name_map, add_derivation, complete_right):
        # type: (List[int], List[str], bool, bool) -> Tuple[List[Text], int]
        sequence.append(self._node._item.rule.production[self._node._item._index])
        return self._follow._to_string(sequence, name_map, add_derivation, complete_right)


class _LR0Derivation(_LR0BaseConstruction):
    def _to_string(self, sequence, name_map, add_derivation, complete_right):
        # type: (List[int], List[str], bool, bool) -> Tuple[List[Text], int]
        expanded_symbol = name_map[self._node._item._symbol]
        sequence_str = u' '.join(name_map[i] for i in sequence)
        if complete_right:
            post_sequence_str = u' '.join(
                name_map[i] for i in self._node._item.rule.production[self._node._item._index + 1:]
            )
        else:
            post_sequence_str = u''
        result, length = self._follow.to_string(name_map)
        if sequence_str:
            padding = ' ' * (len(sequence_str) + 1)
            result = [u'%s %s' % (sequence_str, result[0])] + [u'%s%s' % (padding, s) for s in result[1:]]
            length += len(sequence_str) + 1
        if post_sequence_str:
            result[0] += ' ' * (length - len(result[0]) + 1) + post_sequence_str
            length += len(post_sequence_str) + 1
        if add_derivation:
            extra_padding = u'\u2500' * (length - 2 - len(expanded_symbol))
            derivation_str = u'\u2570%s%s\u256f' % (expanded_symbol, extra_padding)
            return result + [derivation_str], max(length, len(derivation_str))
        else:
            return result, length


class _LR0LeftExpansion(_LR0BaseConstruction):
    def _to_string(self, sequence, name_map, add_derivation, complete_right):
        # type: (List[int], List[str], bool, bool) -> Tuple[List[Text], int]
        sequence += self._node._item.rule.production[:self._node._item._index]
        return self._follow._to_string(sequence, name_map, add_derivation, complete_right)


class _LR0Expansion(_LR0BaseConstruction):
    def __init__(self, node, follow, expanded_path):
        # type: (LR0DominanceNode, LR0Path, LR0Path) -> None
        self._node = node
        self._follow = follow
        self._next = expanded_path
        self._hash_cache = None

    def _to_string(self, sequence, name_map, add_derivation, complete_right):
        # type: (List[int], List[str], bool, bool) -> Tuple[List[Text], int]
        expanded_symbol = name_map[self._node._item._symbol]
        sequence_str = u' '.join(name_map[i] for i in sequence)
        result, length = self._follow._to_string([], name_map, False, False)
        next, next_len = self._next._to_string([], name_map, False, complete_right)

        if sequence_str:
            padding = ' ' * (len(sequence_str) + 1)
            result = [u'%s %s' % (sequence_str, result[0])] + [u'%s%s' % (padding, s) for s in result[1:]]
            length += len(sequence_str) + 1
        max_length = length

        for i, (n1, n2) in enumerate(zip(result, next)):
            result[i] = '%s%s%s' % (n1, ' ' * (length - len(n1) + 1), n2)
            max_length = max(max_length, len(result[i]))
        for final in next[i + 1:]:
            result.append(' ' * (length + 1) + final)

        if add_derivation:
            extra_padding = u'\u2500' * (max_length - 2 - len(expanded_symbol))
            derivation_str = u'\u2570%s%s\u256f' % (expanded_symbol, extra_padding)
            return result + [derivation_str], max(max_length, len(derivation_str))
        else:
            return result, max_length


if TYPE_CHECKING:
    from be_typing import Any, List, Optional, Text, Tuple, Union
    from .lr0dominancenode import LR0DominanceNode
    from .lr0item import LR0Item
