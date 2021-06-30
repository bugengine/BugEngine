from be_typing import TYPE_CHECKING


class LR0Path(object):
    class PathItem(object):
        def __init__(self, lookahead):
            # type: (int) -> None
            self._hash = (lookahead, ) # type: Tuple[Union[int, LR0Item], ...]
            self._lookahead = lookahead

        def to_string(self, name_map):
            # type: (List[str]) -> Tuple[List[str], int]
            la = name_map[self._lookahead]
            return [la], len(la)

    def __init__(self, node, sequence, use_marker=True):
        # type: (LR0DominanceNode, List[Union[LR0Path, LR0Path.PathItem]], bool) -> None
        self._node = node
        if sequence:
            self._sequence = sequence
        else:
            if use_marker:
                self._sequence = list(
                    [LR0Path.PathItem(1)] + [LR0Path.PathItem(i) for i in self._node._item[self._node._item._index:]]
                )
            else:
                self._sequence = list([LR0Path.PathItem(i) for i in self._node._item[self._node._item._index:]])
        self._hash = (node._item, )    # type: Tuple[Union[int, LR0Item], ...]
        for s in self._sequence:
            self._hash += s._hash

    def __hash__(self):
        # type: () -> int
        return hash(self._hash)

    def __eq__(self, other):
        # type: (Any) -> bool
        return isinstance(other, LR0Path) and self._hash == other._hash

    def derive_from(self, node, lookahead):
        # type: (LR0DominanceNode, Optional[int]) -> LR0Path
        if lookahead is None:
            sequence = [self]  # type: (List[Union[LR0Path, LR0Path.PathItem]])
            result = LR0Path(node, sequence + [LR0Path.PathItem(i) for i in node._item[node._item._index + 1:]])
        else:
            sequence = [LR0Path.PathItem(lookahead)]
            result = LR0Path(node, sequence + self._sequence)
        return result

    def expand_left(self):
        # type: () -> LR0Path
        sequence = [
            LR0Path.PathItem(i) for i in self._node._item[:self._node._item._index]
        ]                                                                           # type: (List[Union[LR0Path, LR0Path.PathItem]])

        return LR0Path(self._node, sequence + self._sequence)

    def expand(self, index, path):
        # type: (int, LR0Path) -> LR0Path
        return LR0Path(self._node, self._sequence[:index] + path._sequence)

    def to_string(self, name_map):
        # type: (List[str]) -> Tuple[List[str], int]
        expanded_symbol = name_map[self._node._item._symbol]
        if len(self._sequence) == 0:
            return ['', '\u2570%s\u256f' % expanded_symbol], len(expanded_symbol) + 2
        buffer, length = self._sequence[0].to_string(name_map)
        for item in self._sequence[1:]:
            temp = buffer
            extension, ext_length = item.to_string(name_map)
            buffer = ['%s %s' % (i.ljust(length), j) for i, j in zip(temp, extension)]
            buffer += temp[len(extension):]
            buffer += [(1 + length) * ' ' + j for j in extension[len(temp):]]
            length += 1 + ext_length

        extra_padding = '\u2500' * (length - 2 - len(expanded_symbol))
        buffer.append('\u2570%s%s\u256f' % (expanded_symbol, extra_padding))
        return buffer, max(length, len(buffer[-1]))


if TYPE_CHECKING:
    from be_typing import Any, List, Optional, Tuple, Union
    from .lr0dominancenode import LR0DominanceNode
    from .lr0item import LR0Item
