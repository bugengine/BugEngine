from be_typing import TYPE_CHECKING
from .lr0path import LR0Path


class LR0DominanceNode(object):
    def __init__(self, item_set, item, predecessor=None, parent=None):
        # type: (LR0ItemSet, LR0Item, Optional[Tuple[int, LR0DominanceNode]], Optional[LR0DominanceNode]) -> None
        self._item_set = item_set
        self._item = item
        if predecessor is not None:
            self._predecessor_lookahead = predecessor[0] # type: Optional[int]
            self._predecessors = [predecessor[1]]
        else:
            self._predecessor_lookahead = None
            self._predecessors = []
        self._successor = None                           # type: Optional[LR0DominanceNode]

        self._direct_parents = []
        self._parents = set([])
        self._direct_children = []     # type: List[LR0DominanceNode]
        self._children = set([])       # type: Set[LR0DominanceNode]
        if parent is not None:
            self._direct_parents.append(parent)
            self._parents.add(parent)
            parent._direct_children.append(self)
            parent._children.add(self)

    def expand_empty(self, first_set):
        # type: (Dict[int, List[int]]) -> Optional[LR0Path]
        # expand the first item of the path to build empty productions
        if self._item._index == len(self._item):
            return LR0Path(self, [], use_marker=False)
        result = None  # type: Optional[LR0Path]
        for child in self._direct_children:
            try:
                following_symbol = child._item[0]
            except IndexError:
                result = LR0Path(child, [], use_marker=False)
                result = result.derive_from(self, None)
                return result
            else:
                assert child._successor is not None
                if -1 in first_set[following_symbol]:
                    p = child._successor.expand_empty(first_set)
                    if p is not None:
                        result = child.expand_empty(first_set)
                        assert result is not None
                        result = result.expand(1, p)
                        result = result.derive_from(self, None)
                        return result
        return None

    def expand_lookahead(self, lookahead, first_set):
        # type: (int, Dict[int, List[int]]) -> Optional[LR0Path]
        # expand the first item of the path until it starts with the lookahead
        if self._item[self._item._index] == lookahead:
            return LR0Path(self, [], use_marker=False)
        result = None  # type: Optional[LR0Path]
        for child in self._direct_children:
            try:
                following_symbol = child._item[0]
            except IndexError:
                pass
            else:
                if lookahead == following_symbol:
                    result = LR0Path(child, [], use_marker=False)
                    result = result.derive_from(self, None)
                    return result
                elif lookahead in first_set[following_symbol]:
                    result = child.expand_lookahead(lookahead, first_set)
                    assert result is not None
                    result = result.derive_from(self, None)
                    return result
                elif '<empty>' in first_set[following_symbol]:
                    assert child._successor is not None
                    p = child._successor.expand_lookahead(lookahead, first_set)
                    if p:
                        result = child.expand_empty(first_set)
                        assert result is not None
                        result = result.expand(1, p)
                        result = result.derive_from(self, None)
                        return result
        return None

    def filter_node_by_lookahead(self, path, lookahead, first_set):
        # type: (LR0Path, Optional[int], Dict[int, List[int]]) -> List[Tuple[LR0Path, Optional[int]]]
        result = []    # type: List[Tuple[LR0Path, Optional[int]]]
        if lookahead is not None:
            try:
                following_symbol = self._item[self._item._index + 1]
            except IndexError:
                result.append((path, lookahead))
            else:
                assert self._successor is not None
                if -1 in first_set[following_symbol]:
                    successor_path = self._successor.expand_empty(first_set)
                    assert successor_path is not None
                    for p, la in self._successor.filter_node_by_lookahead(successor_path, lookahead, first_set):
                        result.append((path.expand(1, p), la))
                if lookahead in first_set[following_symbol]:
                    successor_path = self._successor.expand_lookahead(lookahead, first_set)
                    assert successor_path is not None
                    result.append((path.expand(1, successor_path), None))
        else:
            result.append((path, lookahead))
        return result

    def backtrack_up(self, path, state, lookahead, first_set, seen):
        # type: (LR0Path, LR0ItemSet, Optional[int], Dict[int, List[int]], Set[Tuple[LR0DominanceNode, Optional[int]]]) -> List[Tuple[LR0Path, Optional[int]]]
        queue = [(path, lookahead)]
        result = []
        shortest_path_seen = set()
        while queue:
            path, lookahead = queue.pop(0)
            node = path._node
            for parent in sorted(node._direct_parents, key=lambda n: len(n._item) - n._item._index):
                if (parent, lookahead) in seen:
                    continue
                seen.add((parent, lookahead))
                if parent._item._index > 0:
                    if (lookahead, parent._item_set, parent._item[:parent._item._index]) in shortest_path_seen:
                        continue
                for p, la in parent.filter_node_by_lookahead(path.derive_from(parent, None), lookahead, first_set):
                    if parent._item._index > 0 and la is None:
                        shortest_path_seen.add((lookahead, parent._item_set, parent._item[:parent._item._index]))
                    if la is None and state is None:
                        result.append((p, la))
                    else:
                        queue.append((p, la))
            for predecessor in node._predecessors:
                if (predecessor, lookahead) in seen:
                    continue
                seen.add((predecessor, lookahead))
                if state is None or predecessor._item_set == state:
                    if predecessor._item._index > 0:
                        if (
                            lookahead, predecessor._item_set, predecessor._item[:predecessor._item._index]
                        ) in shortest_path_seen:
                            continue
                        shortest_path_seen.add(
                            (lookahead, predecessor._item_set, predecessor._item[:predecessor._item._index])
                        )
                    result.append((path.derive_from(predecessor, node._predecessor_lookahead), lookahead))
        return result


if TYPE_CHECKING:
    from be_typing import Dict, List, Optional, Set, Optional, Tuple
    from .lr0item import LR0Item
    from .lr0itemset import LR0ItemSet