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

    def expand_empty(self):
        # type: () -> LR0Path
        # expand the first item of the path to build empty productions
        if self._item._index == self._item.len:
            return LR0Path(self, use_marker=False)
        for child in self._direct_children:
            if -1 not in child._item._first:
                continue
            if child._successor is None:
                result = LR0Path(child, use_marker=False)
                result = result.derive_from(self)
                return result
            else:
                if -1 in child._item._follow:
                    p = child._successor.expand_empty()
                    result = child.expand_empty()
                    result = result.expand_next(p)
                    result = result.derive_from(self)
                    return result
        raise ValueError()

    def expand_lookahead(self, lookahead):
        # type: (int) -> LR0Path
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
                previous = None
                paths[-1].append(LR0Path(node, use_marker=False))
                while paths:
                    child_paths = paths.pop(-1)
                    if previous is not None:
                        child_paths[-1] = child_paths[-1].expand_next(previous)
                    merge_children = lambda x, y: x.derive_from(y._node)
                    result = functools.reduce(merge_children, child_paths[::-1])
                    previous = result
                return result
            elif lookahead in node._item._first:
                for child in node._direct_children:
                    queue.append((child, paths[:-1] + [paths[-1] + [LR0Path(node, use_marker=False)]]))
            elif -1 in node._item._first and node._successor is not None:
                empty_path = node.expand_empty()
                queue.append((node._successor, paths[:-1] + [paths[-1] + [empty_path]] + [[]]))
        raise ValueError()

    def filter_node_by_lookahead(self, path, lookahead):
        # type: (LR0Path, Optional[int]) -> LR0Path
        following_symbol = self._item.rule.production[self._item._index + 1]
        assert self._successor is not None
        if lookahead == following_symbol:
            return path
        elif lookahead in self._successor._item._first:
            successor_path = self._successor.expand_lookahead(lookahead)
            return path.expand_next(successor_path)
        elif -1 in self._successor._item._first:
            successor_path = self._successor.expand_empty()
            p = self._successor.filter_node_by_lookahead(successor_path, lookahead)
            return path.expand_next(p)
        else:
            raise ValueError()

    def backtrack_up(self, path, state, lookahead, seen):
        # type: (LR0Path, Optional[LR0ItemSet], Optional[int], Set[Tuple[LR0DominanceNode, Optional[int]]]) -> List[Tuple[LR0Path, Optional[int]]]
        queue = [(path, lookahead)]
        result = []                    # type: List[Tuple[LR0Path, Optional[int]]]
                                       #if (self, lookahead) in seen:
                                       #    return result
        seen.add((self, lookahead))
        shortest_path_seen = set()     # type: Set[Tuple[Optional[int], int, Tuple[int, ...]]]
        state_path_seen = set()
        local_seen = set()

        while queue:
            path, lookahead = queue.pop(0)
            node = path._node

            for parent in node._direct_parents:
                if (parent, lookahead) in local_seen:
                    continue
                local_seen.add((parent, lookahead))
                item = parent._item
                if lookahead is not None:
                    try:
                        offset = item._follow[lookahead]
                    except KeyError:
                        pass
                    else:
                        if (
                            lookahead, parent._item._symbol, parent._item.rule.production[:parent._item.len - offset]
                        ) in shortest_path_seen:
                            continue
                        shortest_path_seen.add(
                            (lookahead, parent._item._symbol, parent._item.rule.production[:parent._item.len - offset])
                        )
                        p = parent.filter_node_by_lookahead(path.derive_from(parent), lookahead)
                        if state is None:
                            result.append((p, None))
                        else:
                            queue.append((p, None))
                    try:
                        offset = item._follow[-1]
                    except KeyError:
                        pass
                    else:
                        if (
                            None, parent._item._symbol, parent._item.rule.production[:parent._item.len - offset]
                        ) in shortest_path_seen:
                            continue
                        shortest_path_seen.add(
                            (None, parent._item._symbol, parent._item.rule.production[:parent._item.len - offset])
                        )
                        queue.append((path.derive_from(parent), lookahead))
                else:
                    if (
                        None, parent._item._symbol, parent._item.rule.production[:parent._item._index + 1]
                    ) in shortest_path_seen:
                        continue
                    shortest_path_seen.add(
                        (None, parent._item._symbol, parent._item.rule.production[:parent._item._index + 1])
                    )
                    queue.append((path.derive_from(parent), lookahead))
            for predecessor in node._predecessors:
                if (predecessor, lookahead) in seen:
                    continue
                if state is None or predecessor._item_set == state:
                    seen.add((predecessor, lookahead))
                    if lookahead is None:
                        if predecessor._item_set in state_path_seen:
                            continue
                        state_path_seen.add(predecessor._item_set)
                    assert node._predecessor_lookahead is not None
                    result.append((path.extend(predecessor, node._predecessor_lookahead), lookahead))
        return result


if TYPE_CHECKING:
    from be_typing import List, Optional, Set, Optional, Tuple
    from .lr0item import LR0Item
    from .lr0itemset import LR0ItemSet