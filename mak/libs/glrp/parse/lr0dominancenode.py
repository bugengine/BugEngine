from be_typing import TYPE_CHECKING
import functools
from .lr0path import LR0Path


class LR0DominanceNode(object):
    def __init__(self, item_set, item, predecessor=None, parent=None):
        # type: (LR0ItemSet, LR0Item, Optional[Tuple[int, LR0DominanceNode]], Optional[LR0DominanceNode]) -> None
        self._item_set = item_set
        self._item = item
        self._parents = set()                            # type: Set[LR0DominanceNode]
        if predecessor is not None:
            self._predecessor_lookahead = predecessor[0] # type: Optional[int]
            self._predecessors = [predecessor[1]]
        else:
            self._predecessor_lookahead = None
            self._predecessors = []
        self._successor = None                           # type: Optional[LR0DominanceNode]

        self._direct_parents = []
        self._direct_children = []     # type: List[LR0DominanceNode]
        if parent is not None:
            self._direct_parents.append(parent)
            parent._direct_children.append(self)

    def expand_empty(self, first_set):
        # type: (Dict[int, Set[int]]) -> Optional[LR0Path]
        # expand the first item of the path to build empty productions
        if self._item._index == self._item.len:
            return LR0Path(self, use_marker=False)
        result = None  # type: Optional[LR0Path]
        for child in self._direct_children:
            try:
                following_symbol = child._item.rule.production[0]
            except IndexError:
                result = LR0Path(child, use_marker=False)
                result = result.derive_from(self)
                return result
            else:
                assert child._successor is not None
                if -1 in first_set.get(following_symbol, set([following_symbol])):
                    p = child._successor.expand_empty(first_set)
                    if p is not None:
                        result = child.expand_empty(first_set)
                        assert result is not None
                        result = result.expand_next(p)
                        result = result.derive_from(self)
                        return result
        return None

    def expand_lookahead(self, lookahead, first_set):
        # type: (int, Dict[int, Set[int]]) -> Optional[LR0Path]
        # expand the first item of the path until it starts with the lookahead
        queue = [(self, [[]])]     # type: List[Tuple[LR0DominanceNode, List[List[LR0Path]]]]
        seen = set()

        while queue:
            node, paths = queue.pop(0)
            if node in seen:
                continue
            seen.add(node)

            try:
                following_symbol = node._item.rule.production[node._item._index]
            except IndexError:
                continue

            if following_symbol == lookahead:
                result = None
                paths[-1].append(LR0Path(node, use_marker=False))
                while paths:
                    child_paths = paths.pop(-1)
                    if result is not None:
                        child_paths[-1] = child_paths[-1].expand_next(result)
                    merge_children = lambda x, y: x.derive_from(y._node)
                    result = functools.reduce(merge_children, child_paths[::-1])
                return result
            elif lookahead in first_set.get(following_symbol, []):
                for child in sorted(node._direct_children, key=lambda n: n._item.len):
                    queue.append((child, paths[:-1] + [paths[-1] + [LR0Path(node, use_marker=False)]]))
            elif -1 in first_set.get(following_symbol, []) and node._successor is not None:
                empty_path = node.expand_empty(first_set)
                if empty_path is not None:
                    queue.append((node._successor, paths[:-1] + [paths[-1] + [empty_path]] + [[]]))

        return None

    def filter_node_by_lookahead(self, path, lookahead, first_set):
        # type: (LR0Path, Optional[int], Dict[int, Set[int]]) -> List[Tuple[LR0Path, Optional[int]]]
        result = []    # type: List[Tuple[LR0Path, Optional[int]]]
        if lookahead is not None:
            try:
                following_symbol = self._item.rule.production[self._item._index + 1]
            except IndexError:
                result.append((path, lookahead))
            else:
                assert self._successor is not None
                if lookahead == following_symbol:
                    result.append((path, None))
                elif lookahead in first_set.get(following_symbol, []):
                    successor_path = self._successor.expand_lookahead(lookahead, first_set)
                    assert successor_path is not None
                    result.append((path.expand_next(successor_path), None))
                elif -1 in first_set.get(following_symbol, []):
                    successor_path = self._successor.expand_empty(first_set)
                    assert successor_path is not None
                    for p, la in self._successor.filter_node_by_lookahead(successor_path, lookahead, first_set):
                        result.append((path.expand_next(p), la))
        else:
            result.append((path, lookahead))
        return result

    def backtrack_up(self, path, state, lookahead, first_set, seen):
        # type: (LR0Path, Optional[LR0ItemSet], Optional[int], Dict[int, Set[int]], Set[Tuple[LR0DominanceNode, Optional[int]]]) -> List[Tuple[LR0Path, Optional[int]]]
        queue = [(path, lookahead)]
        result = []    # type: List[Tuple[LR0Path, Optional[int]]]
                       #if (self, lookahead) in seen:
                       #    return result
        seen.add((self, lookahead))
        shortest_path_seen = set()
        while queue:
            path, lookahead = queue.pop(0)
            node = path._node

            for parent in node._direct_parents:
                if (lookahead, node._item.rule.production[:node._item._index + 1]) in shortest_path_seen:
                    continue
                if (parent, lookahead) in seen:
                    continue
                seen.add((parent, lookahead))
                for p, la in parent.filter_node_by_lookahead(path.derive_from(parent), lookahead, first_set):
                    if la is None:
                        shortest_path_seen.add((lookahead, node._item.rule.production[:node._item._index + 1]))
                    if la is None and state is None:
                        result.append((p, la))
                    else:
                        queue.append((p, la))
            for predecessor in node._predecessors:
                if (predecessor, lookahead) in seen:
                    continue
                seen.add((predecessor, lookahead))
                if state is None or predecessor._item_set == state:
                    assert node._predecessor_lookahead is not None
                    result.append((path.extend(predecessor, node._predecessor_lookahead), lookahead))
        return result


if TYPE_CHECKING:
    from be_typing import Dict, List, Optional, Set, Optional, Tuple
    from .lr0item import LR0Item
    from .lr0itemset import LR0ItemSet