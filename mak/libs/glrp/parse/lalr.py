from .lr0itemset import LR0ItemSet
from .lr0dominancenode import LR0DominanceNode
from be_typing import TYPE_CHECKING
import sys


def create_parser_table(productions, start_id, name_map, terminal_count, log, error_log, dot_file):
    # type: (Dict[int, Grammar.Production], int, List[str], int, Logger, Logger, Logger) -> None
    cidhash = {}
    goto_cache = {}        # type: Dict[Tuple[int, int], Optional[LR0ItemSet]]
    goto_cache_2 = {}      # type: Dict[int, Any]
    dominator_nodes = []   # type: List[LR0DominanceNode]

    def goto(item_set, lookahead):
        # type: (LR0ItemSet, int) -> Optional[LR0ItemSet]
        # First we look for a previously cached entry
        item_set_id = id(item_set)
        result = goto_cache.get((item_set_id, lookahead), None)
        if result is not None:
            return result

        s = goto_cache_2.get(lookahead)
        if not s:
            s = {}
            goto_cache_2[lookahead] = s

        gs = []
        for item in item_set:
            next = item._next
            if next and next._before == lookahead:
                s1 = s.get(id(next))
                if not s1:
                    s1 = {}
                    s[id(next)] = s1
                gs.append((next, item_set._items[item], lookahead))
                s = s1

        result = s.get(0, None)
        if result is None:
            if gs:
                result = LR0ItemSet(gs, dominator_nodes)
                s[0] = result
            else:
                s[0] = None
        else:
            result.add_caller(gs, dominator_nodes)

        goto_cache[(item_set_id, lookahead)] = result
        return result

    def create_item_sets():
        # type: () -> Tuple[List[LR0DominanceNode], List[LR0ItemSet]]
        assert len(productions[start_id]) == 1
        assert len(dominator_nodes) == 0
        root_node = LR0DominanceNode(None, productions[start_id][0]._item, {}, dominator_nodes)
        states = [LR0ItemSet([(productions[start_id][0]._item, root_node, 1)], dominator_nodes)]
        cidhash[id(states[0])] = 0

        # Loop over the items in C and each grammar symbols
        i = 0
        while i < len(states):
            state = states[i]
            i += 1

            # Collect all of the symbols that could possibly be in the goto(I,X) sets
            asyms = set([])
            for item in state:
                for s in item._symbols:
                    asyms.add(s)

            for x in asyms:
                g = goto(state, x)
                if not g or id(g) in cidhash:
                    continue
                cidhash[id(g)] = len(states)
                states.append(g)

        return dominator_nodes, states

    def add_lalr_lookahead(states):
        # type: (List[LR0ItemSet]) -> None

        def traverse(x, N, stack, F, X, R, FP):
            # type: (Tuple[int, int], Dict[Tuple[int, int], int], List[Tuple[int, int]], Dict[Tuple[int, int], List[int]], List[Tuple[int, int]], Callable[[Tuple[int, int]], List[Tuple[int, int]]], Callable[[Tuple[int, int]], List[int]]) -> None
            stack.append(x)
            d = len(stack)
            N[x] = d
            F[x] = FP(x)   # F(X) <- F'(x)

            rel = R(x)     # Get y's related to x
            for y in rel:
                if N[y] == 0:
                    traverse(y, N, stack, F, X, R, FP)
                N[x] = min(N[x], N[y])
                for a in F.get(y, []):
                    if a not in F[x]:
                        F[x].append(a)
            if N[x] == d:
                N[stack[-1]] = sys.maxsize
                F[stack[-1]] = F[x]
                element = stack.pop()
                while element != x:
                    N[stack[-1]] = sys.maxsize
                    F[stack[-1]] = F[x]
                    element = stack.pop()

        def digraph(X, R, FP):
            # type: (List[Tuple[int, int]], Callable[[Tuple[int, int]], List[Tuple[int, int]]], Callable[[Tuple[int, int]], List[int]]) -> Dict[Tuple[int, int], List[int]]
            N = {}
            for x in X:
                N[x] = 0
            stack = []     # type: List[Tuple[int, int]]
            F = {}         # type: Dict[Tuple[int, int], List[int]]
            for x in X:
                if N[x] == 0:
                    traverse(x, N, stack, F, X, R, FP)
            return F

        def dr_relation(trans):
            # type: (Tuple[int, int]) -> List[int]
            state, N = trans
            terms = []

            item_set = goto(states[state], N)
            assert item_set is not None
            for item in item_set:
                if item._index < len(item._rule):
                    a = item._rule._production[item._index]
                    if a < terminal_count:
                        if a not in terms:
                            terms.append(a)

            # This extra bit is to handle the start state
            if state == start_id and N == productions[start_id][0][0]:
                terms.append(0)

            return terms

        def reads_relation(trans, empty):
            # type: (Tuple[int, int], Set[int]) -> List[Tuple[int, int]]
            # Look for empty transitions
            rel = []
            state, N = trans

            item_set = goto(states[state], N)
            assert item_set is not None
            j = cidhash[id(item_set)]
            for item in item_set:
                if item._index < len(item._rule):
                    a = item._rule[item._index]
                    if a in empty:
                        rel.append((j, a))

            return rel

        def compute_read_sets(trans, nullable):
            # type: (List[Tuple[int, int]], Set[int]) -> Dict[Tuple[int, int], List[int]]
            FP = lambda x: dr_relation(x)
            R = lambda x: reads_relation(x, nullable)
            F = digraph(trans, R, FP)
            return F

        def compute_lookback_includes(trans, nullable):
            # type: (List[Tuple[int, int]], Set[int]) -> Tuple[Dict[Tuple[int, int], List[Tuple[int, LR0Item]]], Dict[Tuple[int, int], List[Tuple[int, int]]]]
            lookdict = {}      # Dictionary of lookback relations
            includedict = {}   # type: Dict[Tuple[int, int], List[Tuple[int, int]]]

            # Make a dictionary of non-terminal transitions
            dtrans = {}
            for t1 in trans:
                dtrans[t1] = 1

            # Loop over all transitions and compute lookbacks and includes
            for state, N in trans:
                lookb = []
                includes = []
                for p in states[state]:
                    if p._rule._prod_index != N:
                        continue

                    # Okay, we have a name match.  We now follow the production all the way
                    # through the state machine until we get the . on the right hand side

                    lr_index = p._index
                    j = state
                    while lr_index < len(p._rule):
                        t = p._rule[lr_index]
                        lr_index = lr_index + 1

                        # Check to see if this symbol and state are a non-terminal transition
                        if (j, t) in dtrans:
                            # Yes.  Okay, there is some chance that this is an includes relation
                            # the only way to know for certain is whether the rest of the
                            # production derives empty

                            li = lr_index
                            while li < len(p._rule):
                                if p._rule[li] < terminal_count:
                                    break # No forget it
                                if p._rule[li] not in nullable:
                                    break
                                li = li + 1
                            else:
                                          # Appears to be a relation between (j,t) and (state,N)
                                includes.append((j, t))

                        g = goto(states[j], t) # Go to next set
                        j = cidhash[id(g)]     # Go to next state

                    # When we get here, j is the final state, now we have to locate the production
                    for r in states[j]:
                        if r._rule._prod_index != p._rule._prod_index:
                            continue
                        if len(r._rule) != len(p._rule):
                            continue
                        # i = 0
                        # This look is comparing a production ". A B C" with "A B C ."
                        # while i < r._index:
                        #    if r._rule[i] != p._rule[i + 1]:
                        #        break
                        #    i = i + 1
                        #else:
                        #    lookb.append((j, r))
                        if p._index == 0 and r._rule._production[:r._index] == p._rule._production[:r._index]:
                            lookb.append((j, r))
                for ii in includes:
                    if ii not in includedict:
                        includedict[ii] = []
                    includedict[ii].append((state, N))
                lookdict[(state, N)] = lookb

            return lookdict, includedict

        def compute_follow_sets(ntrans, readsets, inclsets):
            # type: (List[Tuple[int, int]], Dict[Tuple[int, int], List[int]], Dict[Tuple[int, int], List[Tuple[int, int]]]) -> Dict[Tuple[int, int], List[int]]
            FP = lambda x: readsets[x]
            R = lambda x: inclsets.get(x, [])
            F = digraph(ntrans, R, FP)
            return F

        def add_lookaheads(lookbacks, followset):
            # type: (Dict[Tuple[int, int], List[Tuple[int, LR0Item]]], Dict[Tuple[int, int], List[int]]) -> None
            for trans, lb in lookbacks.items():
                # Loop over productions in lookback
                for state, p in lb:
                    if state not in p._lookaheads:
                        p._lookaheads[state] = []
                    l = p._lookaheads[state]
                    f = followset.get(trans, [])
                    for a in f:
                        if a not in l:
                            l.append(a)

        # Determine all of the nullable nonterminals
        nullable = set([])
        for prod_index, prod in productions.items():
            if prod._empty:
                nullable.add(prod_index)

        # Find all non-terminal transitions
        trans = []
        for stateno, state in enumerate(states):
            for item in state:
                if item._index < len(item._rule):
                    t = (stateno, item._rule[item._index])
                    if t[1] >= terminal_count:
                        if t not in trans:
                            trans.append(t)

        # Compute read sets
        readsets = compute_read_sets(trans, nullable)

        # Compute lookback/includes relations
        lookd, included = compute_lookback_includes(trans, nullable)

        # Compute LALR FOLLOW sets
        followsets = compute_follow_sets(trans, readsets, included)

        # Add all of the lookaheads
        add_lookaheads(lookd, followsets)

    goto_table = {}    # Goto array
    action = {}        # Action array
    actionp = {}       # Action production array (temporary)

    # Build the parser table, state by state
    dominator_nodes, states = create_item_sets()
    add_lalr_lookahead(states)

    # build dominance tree for items
    #all_nodes = set(dominator_nodes)
    #for node in dominator_nodes[1:]:
    #    node._dominators = all_nodes
    #changed = True
    #while changed:
    #    changed = False
    #    for node in dominator_nodes[1:]:
    #        dominators = set([])   # type: Set[LR0DominanceNode]
    #        dominators.update(node._predecessors[0]._dominators)
    #        for pred in node._predecessors[1:]:
    #            dominators.intersection_update(pred._dominators)
    #        dominators.add(node)
    #        if node._dominators != dominators:
    #            changed = True
    #            node._dominators = dominators

    #for node in dominator_nodes[1:]:
    #    parent = None  # type: Optional[LR0DominanceNode]
    #    for dom_node in node._dominators:
    #        if dom_node == node:
    #            continue
    #        elif parent is None:
    #            parent = dom_node
    #        elif dom_node in parent._dominators:
    #            assert parent not in dom_node._dominators
    #        else:
    #            assert parent in dom_node._dominators
    #            parent = dom_node
    #    node._parent = parent
    #    if parent is not None:
    #        parent._children.append(node)

    st = 0

    num_missing_annotations = 0
    num_rr = 0

    dot_file.info('digraph Grammar {')
    for item_group in states:
        dot_file.info("  subgraph cluster_%d {", st)
        dot_file.info('    label="State %d";', st)
        dot_file.info('    style=filled;')
        dot_file.info('    color=lightgray;')
        dot_file.info('    node[style=filled;color=white];')
        state_id = id(item_group)
        for item in item_group:
            dot_file.info('    %d[label="%s"];', id(item_group._items[item]), item.to_string(name_map))
        for item in item_group:
            dnode = item_group._items[item]
            for predecessor in dnode._predecessors:
                if predecessor._item_set == state_id:
                    dot_file.info('    %d -> %d;', id(predecessor), id(dnode))
        dot_file.info("  }")
        st += 1
    dot_file.info('  %d[label="start"];', id(dominator_nodes[0]))

    st = 0
    for item_group in states:
        # Loop over each production
        st_action = {}     # type: Dict[int, List[Tuple[int, LR0Item]]]
        st_goto = {}
        log.info('state %d:', st)
        log.info('')
        for item in item_group:
            log.info('    (%d) %s', item._rule._prod_index, item.to_string(name_map))
        log.info('')

        for item in item_group:
            if len(item._rule) == item._index:
                if item._rule._prod_index == start_id:
                    # Start symbol. Accept!
                    st_action[0] = st_action.get(0, []) + [(0, item)]
                    item._rule._reduced += 1
                else:
                    # We are at the end of a production.  Reduce!
                    for a in item._lookaheads[st]:
                        st_action[a] = st_action.get(a, []) + [(-item._rule._id, item)]
                        item._rule._reduced += 1
            else:
                i = item._index
                a = item._rule[i]  # Get symbol right after the "."
                if a < terminal_count:
                    g = goto(item_group, a)
                    j = cidhash[id(g)]
                    if j >= 0:
                        st_action[a] = st_action.get(a, []) + [(j, item)]

        for a, actions in st_action.items():
            action_dest = {}   # type: Dict[int, List[LR0Item]]
            for i, item in actions:
                try:
                    action_dest[i].append(item)
                except KeyError:
                    action_dest[i] = [item]

            accepted_actions = {}  # type: Dict[int, List[LR0Item]]

            if len(action_dest) > 1:
                # looks like a potential conflict, look at precedence
                actions = sorted(actions, key=lambda x: x[1]._precedence[1] if x[1]._precedence is not None else -1)
                precedence = actions[-1][1]._precedence[1] if actions[-1][1]._precedence is not None else -1
                associativity = actions[-1][1]._precedence[0] if actions[-1][1]._precedence is not None else 'left'
                for _, item in actions:
                    if item._precedence is None:
                        log.warning('  ** %s [%s] needs a precedence annotation', item.to_string(name_map), name_map[a])
                        error_log.warning(
                            '  ** %s [%s] needs a precedence annotation', item.to_string(name_map), name_map[a]
                        )
                        num_missing_annotations += 1
                assoc_conflict = False
                for j, item in actions:
                    if item._precedence is not None:
                        if item._precedence[1] < precedence:
                            log.info('  -- %s is not used', item.to_string(name_map))
                        elif item._precedence[0] != associativity:
                            log.error('  *** associativity conflicts!')
                            log.error('  *** %s', item.to_string(name_map))
                            error_log.error('  *** associativity conflicts!')
                            error_log.error('  *** %s', item.to_string(name_map))
                            assoc_conflict = True
                        elif associativity == 'left' and j >= 0:
                            try:
                                accepted_actions[j].append(item)
                            except KeyError:
                                accepted_actions[j] = [item]
                        elif associativity == 'right' and j < 0:
                            try:
                                accepted_actions[j].append(item)
                            except KeyError:
                                accepted_actions[j] = [item]
                        elif associativity == 'nonassoc':
                            try:
                                accepted_actions[j].append(item)
                            except KeyError:
                                accepted_actions[j] = [item]
                        else:
                            log.info('  -- %s is not used', item.to_string(name_map))
                    else:
                        try:
                            accepted_actions[j].append(item)
                        except KeyError:
                            accepted_actions[j] = [item]

                if assoc_conflict:
                    log.error('  *** %s', actions[-1][1].to_string(name_map))
                    log.error('  *** using %s', actions[-1][1].to_string(name_map))
                    error_log.error('  *** %s', actions[-1][1].to_string(name_map))
                    error_log.error('  *** using %s', actions[-1][1].to_string(name_map))
            else:
                accepted_actions = action_dest

            for target_group, item_list in accepted_actions.items():
                if target_group >= 0:
                    for item1 in item_list:
                        node1 = item_group._items[item1]
                        for item2 in states[target_group]:
                            node2 = states[target_group]._items[item2]
                            if node1 in node2._predecessors:
                                dot_file.info('  %d -> %d[label="%s"];', id(node1), id(node2), name_map[a])
            if len(accepted_actions) > 1:
                # get the longest reduce action
                dom_nodes = {}     # type: Dict[Optional[LR0ItemSet], Dict[LR0DominanceNode, LR0Path]]
                longest_reduction = 0
                conflict_paths = {}
                for _, item_list in accepted_actions.items():
                    for item in item_list:
                        longest_reduction = max(longest_reduction, item._index)
                for _, item_list in accepted_actions.items():
                    for item in item_list:
                        for path in item_group._items[item].find_core(longest_reduction, set([])):
                            conflict_paths[path._node] = path
                    for node, path in conflict_paths.items():
                        try:
                            dom_nodes[node._item_set][path._node] = path
                        except:
                            dom_nodes[node._item_set] = {path._node: path}

                for _, item_list in accepted_actions.items():
                    for item in item_list:
                        if not item._split:
                            log.warning('  ** %s [%s] needs a split annotation', item.to_string(name_map), name_map[a])
                            error_log.warning(
                                '  ** %s [%s] needs a split annotation', item.to_string(name_map), name_map[a]
                            )
                            num_missing_annotations += 1

                # reduce until...
                reduced = True
                while reduced:
                    new_dom_nodes = {} # type: Dict[Optional[LR0ItemSet], Dict[LR0DominanceNode, LR0Path]]
                    reduced = False

                    #error_log.warning('  ** state %d', st)
                    #for _, nodes in dom_nodes.items():
                    #    for node in nodes:
                    #        log.warning('  ** %s needs a merge annotation', node._item.to_string(name_map))
                    #        error_log.warning('  ** %s needs a merge annotation', node._item.to_string(name_map))
                    #        num_missing_annotations += 1

                    for item_group_id, nodes in dom_nodes.items():
                        conflict_paths = {}
                        n1, p1 = nodes.popitem()
                        longest_reduction = n1._item._index
                        origin = set(n1._direct_predecessors)
                        for node in nodes:
                            longest_reduction = max(longest_reduction, node._item._index)
                            origin.intersection_update(node._direct_predecessors)
                        nodes[n1] = p1
                        if longest_reduction == 0 and len(origin) == 0:
                            for node in nodes:
                                for pred in node._direct_predecessors:
                                    if pred._item._index != 0:
                                        origin.add(pred)
                            longest_reduction = 0
                            for node in origin:
                                longest_reduction = max(longest_reduction, node._item._index)
                        if longest_reduction != 0:
                            reduced = True
                            for node, path in nodes.items():
                                for core_path in node.find_core(longest_reduction, set([])):
                                    conflict_paths[core_path._node] = core_path.extend(path)
                            for node, path in conflict_paths.items():
                                try:
                                    new_dom_nodes[node._item_set][path._node] = path
                                except:
                                    new_dom_nodes[node._item_set] = {path._node: path}
                        else:
                            new_dom_nodes[item_group_id] = nodes
                    dom_nodes = new_dom_nodes

                # find the deepest nodes of each state that reach only one of the conflicts
                #error_log.warning('  ** state %d', st)
                #for _, nodes in dom_nodes.items():
                #    for node in nodes:
                #        log.warning('  ** %s needs a merge annotation', node._item.to_string(name_map))
                #        error_log.warning('  ** %s needs a merge annotation', node._item.to_string(name_map))
                #        num_missing_annotations += 1

                for item_group_id, nodes in dom_nodes.items():
                    leaves = dict(nodes)
                    conflict_nodes = {}
                    for node, path in nodes.items():
                        conflict_nodes[node] = [path]
                    node, path = nodes.popitem()
                    origin = set(node._direct_predecessors)
                    while nodes:
                        node, path = nodes.popitem()
                        origin.intersection_update(node._direct_predecessors)
                    for o in origin:
                        for successor in o._direct_successors:
                            inter = set(leaves).intersection(successor._direct_successors)
                            if len(inter) > 0 and len(inter) < len(leaves):
                                conflict_nodes[successor] = [leaves[x] for x in inter]
                    for n1, paths in conflict_nodes.items():
                        for n2, _ in conflict_nodes.items():
                            if n2 in n1._direct_predecessors:
                                break
                        else:
                            log.warning('  ** %s needs a merge annotation', n1._item.to_string(name_map))
                            error_log.warning('  ** %s needs a merge annotation', n1._item.to_string(name_map))
                            for p in paths:
                                p.log(log, name_map)
                                p.log(error_log, name_map)
                            num_missing_annotations += 1

                # find the deepest nodes of each state that reach only one of the conflicts
                #error_log.warning('  ** state %d', st)
                #for _, nodes in dom_nodes.items():
                #    for node in nodes:
                #        log.warning('  ** %s needs a merge annotation', node._item.to_string(name_map))
                #        error_log.warning('  ** %s needs a merge annotation', node._item.to_string(name_map))
                #        num_missing_annotations += 1

        nkeys = set([])
        for item in item_group:
            node1 = item_group._items[item]
            for s in item._symbols:
                if s > terminal_count:
                    g = goto(item_group, s)
                    j = cidhash.get(id(g), -1)
                    if j >= 0:
                        for item2 in states[j]:
                            dnode = states[j]._items[item2]
                            if node1 in dnode._predecessors:
                                dot_file.info('  %d->%d[label="%s"];', id(node1), id(dnode), name_map[s])
                        if s not in nkeys:
                            nkeys.add(s)
                            #st_goto[n] = j
                            log.info('    %-30s shift and go to state %d', name_map[s], j)

        st += 1
        """
        # Print the actions associated with each terminal
        _actprint = {}
        for a, p, m in actlist:
            if a in st_action:
                if p is st_actionp[a]:
                    log.info('    %-15s %s', name_map[a], m)
                    _actprint[(a, m)] = 1
        log.info('')
        # Print the actions that were not used. (debugging)
        not_used = 0
        for a, p, m in actlist:
            if a in st_action:
                if p is not st_actionp[a]:
                    if not (a, m) in _actprint:
                        log.note('  ! %-15s [ %s ]', name_map[a], m)
                        not_used = 1
                        _actprint[(a, m)] = 1
        if not_used:
            log.note('')

        # Construct the goto table for this state

        nkeys = set([])    # type: Set[int]
        for item in item_group:
            for s in item._symbols:
                if s > terminal_count:
                    nkeys.add(s)
        for n in nkeys:
            g = goto(item_group, n)
            j = cidhash.get(id(g), -1)
            if j >= 0:
                st_goto[n] = j
                log.info('    %-30s shift and go to state %d', name_map[n], j)

        action[st] = st_action
        goto_table[st] = st_goto
        st += 1

        for state, tok, rule, resolution in sr_conflicts:
            log.warning('shift/reduce conflict for %s in state %d resolved as %s', name_map[tok], state, resolution)
            log.warning('  reduce rule %s', rule.to_string(name_map))
            error_log.warning(
                'shift/reduce conflict for %s in state %d resolved as %s', name_map[tok], state, resolution
            )
            error_log.warning('  reduce rule %s', rule.to_string(name_map))

        already_reported = set()
        for state, rule, rejected in rr_conflicts:
            if (state, id(rule), id(rejected)) in already_reported:
                continue
            log.warning('reduce/reduce conflict in state %d resolved using rule (%s)', state, rule.to_string(name_map))
            log.warning('rejected rule (%s) in state %d', rejected.to_string(name_map), state)
            error_log.warning(
                'reduce/reduce conflict in state %d resolved using rule (%s)', state, rule.to_string(name_map)
            )
            error_log.warning('rejected rule (%s) in state %d', rejected.to_string(name_map), state)
            already_reported.add((state, id(rule), id(rejected)))
        """
    dot_file.info('}')

    # Report shift/reduce and reduce/reduce conflicts
    if num_missing_annotations == 1:
        log.warning('1 missing annotation')
        error_log.warning('1 missing annotation')
    elif num_missing_annotations > 1:
        log.warning('%d missing annotations', num_missing_annotations)
        error_log.warning('%d missing annotations', num_missing_annotations)

    if num_rr == 1:
        log.warning('1 reduce/reduce conflict')
    elif num_rr > 1:
        log.warning('%d reduce/reduce conflicts', num_rr)
        error_log.warning('%d reduce/reduce conflicts', num_rr)

    for _, production in productions.items():
        for rule in production:
            if rule._reduced == 0:
                log.warning('Rule (%s) is never reduced', rule.to_string(name_map))


if TYPE_CHECKING:
    from be_typing import Any, Callable, Dict, List, Optional, Set, Tuple
    from .grammar import Grammar
    from .lr0item import LR0Item
    from .lr0path import LR0Path
    from ..log import Logger
