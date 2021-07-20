from .lr0itemset import LR0ItemSet
from .lr0dominancenode import LR0DominanceNode
from .lr0path import LR0Path
from collections import OrderedDict
from be_typing import TYPE_CHECKING
import sys


def _log(title, conflict_paths, out, name_map):
    # type: (Text, List[LR0Path], Logger, List[str]) -> None
    seen = set([])
    if conflict_paths:
        count = len(set(conflict_paths))
        out.info(u'   %s', title)
        out.info(u'   \u256d\u2574')
        for path in conflict_paths:
            if path in seen:
                continue
            count -= 1
            seen.add(path)
            strings = path.expand_left().to_string(name_map)[0]
            for s in strings:
                out.info(u'   \u2502 %s', s)
            if count == 0:
                out.info(u'   \u2570\u2574')
            else:
                out.info(u'   \u251c\u2574')


def _log_counterexamples(conflict_list, out, name_map):
    # type: (List[Tuple[LR0DominanceNode, Text, Optional[int]]], Logger, List[str]) -> List[LR0Item]
    conflict_paths = [(message, []) for _, message, _ in conflict_list] # type: List[Tuple[Text, List[LR0Path]]]
    result = []                                                         # type: List[LR0Item]

    queue = []     # type: List[List[Tuple[LR0Path, Optional[int], Set[Tuple[LR0DominanceNode, Optional[int]]], int]]]

    queue.append([(LR0Path(node), lookahead, set(), index) for index, (node, _, lookahead) in enumerate(conflict_list)])
    while queue:
        path_list = queue.pop(0)
        #assert path_1._node.item_set == path_2._node.item_set
        path_len = 0
        found_lookaheads = True
        for path, lookahead, _, _ in path_list:
            path_len = max(path_len, path._node._item._index)
            found_lookaheads &= lookahead is None

        if path_len == 0:
            if found_lookaheads:
                for path, _, _, index in path_list:
                    conflict_paths[index][1].append(path)
            else:
                states = OrderedDict(
                )                      # type: Dict[LR0ItemSet, List[Tuple[LR0Path, Optional[int], Set[Tuple[LR0DominanceNode, Optional[int]]], int]]]

                for path, lookahead, seen, index in path_list:
                    if lookahead is not None:
                        for path, la in path._node.backtrack_up(path, None, lookahead, seen):
                            try:
                                states[path._node._item_set].append((path, la, seen, index))
                            except KeyError:
                                states[path._node._item_set] = [(path, la, seen, index)]
                for state, plist in states.items():
                    for path, lookahead, seen, index in path_list:
                        if lookahead is None:
                            if path._node._item_set == state:
                                plist.append((path, lookahead, seen, index))
                            else:
                                for path, la in path._node.backtrack_up(path, state, lookahead, seen):
                                    #assert path1._node.item_set == path2._node.item_set
                                    plist.append((path, la, seen, index))
                for _, plist in states.items():
                    queue.append(plist)
        else:
            states = OrderedDict()
            for path, lookahead, seen, index in path_list:
                if path._node._item._index > 0:
                    for path, la in path._node.backtrack_up(path, None, lookahead, seen):
                        try:
                            states[path._node._item_set].append((path, la, seen, index))
                        except KeyError:
                            states[path._node._item_set] = [(path, la, seen, index)]
            for state, plist in states.items():
                for path, lookahead, seen, index in path_list:
                    if path._node._item._index == 0:
                        for path, la in path._node.backtrack_up(path, state, lookahead, seen):
                            #assert path1._node.item_set == path2._node.item_set
                            plist.append((path, la, seen, index))
            for _, plist in states.items():
                queue.append(plist)

    for message, conflicts in conflict_paths:
        _log(message, conflicts, out, name_map)
        out.info('')
    return result


def create_parser_table(productions, start_id, name_map, terminal_count, sm_log, conflict_log, error_log, dot_file):
    # type: (Dict[int, Grammar.Production], int, List[str], int, Logger, Logger, Logger, Logger) -> None
    cidhash = {}       # type: Dict[int, int]
    goto_cache = {}    # type: Dict[Tuple[int, int], Optional[LR0ItemSet]]
    goto_cache_2 = {}  # type: Dict[int, Any]

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

        gs = []    # type: List[Tuple[LR0Item, Optional[LR0DominanceNode], int]]
        for item in item_set:
            next = item._next
            if next and next._before == lookahead:
                gs.append((next, item_set[item], lookahead))

        gs = sorted(gs, key=lambda x: id(x[0]))
        for item, _, _ in gs:
            s1 = s.get(id(item))
            if not s1:
                s1 = {}
                s[id(item)] = s1
            s = s1

        result = s.get(0, None)
        if result is None:
            if gs:
                result = LR0ItemSet(gs)
                s[0] = result
            else:
                s[0] = None
        else:
            result.add_core(gs)

        goto_cache[(item_set_id, lookahead)] = result
        return result

    def create_item_sets():
        # type: () -> List[LR0ItemSet]
        assert len(productions[start_id]) == 1
        states = [LR0ItemSet([(productions[start_id][0]._item, None, 2)])]
        cidhash[id(states[0])] = 0

        # Loop over the items in C and each grammar symbols
        i = 0
        while i < len(states):
            state = states[i]
            i += 1

            # Collect all of the symbols that could possibly be in the goto(I,X) sets
            asyms = set([])
            for item in state:
                asyms.update(item._symbols)

            for x in sorted(asyms):
                g = goto(state, x)
                if not g or id(g) in cidhash:
                    continue
                cidhash[id(g)] = len(states)
                states.append(g)

        return states

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
                if item._index < item.len:
                    a = item.rule.production[item._index]
                    if a < terminal_count:
                        if a not in terms:
                            terms.append(a)

            # This extra bit is to handle the start state
            if state == start_id and N == productions[start_id][0].production[0]:
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
                if item._index < item.len:
                    a = item.rule.production[item._index]
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
                    if p.rule._prod_symbol != N:
                        continue

                    # Okay, we have a name match.  We now follow the production all the way
                    # through the state machine until we get the . on the right hand side

                    lr_index = p._index
                    j = state
                    while lr_index < p.len:
                        t = p.rule.production[lr_index]
                        lr_index = lr_index + 1

                        # Check to see if this symbol and state are a non-terminal transition
                        if (j, t) in dtrans:
                            # Yes.  Okay, there is some chance that this is an includes relation
                            # the only way to know for certain is whether the rest of the
                            # production derives empty

                            li = lr_index
                            while li < p.len:
                                if p.rule.production[li] < terminal_count:
                                    break # No forget it
                                if p.rule.production[li] not in nullable:
                                    break
                                li = li + 1
                            else:
                                          # Appears to be a relation between (j,t) and (state,N)
                                includes.append((j, t))

                        g = goto(states[j], t) # Go to next set
                        j = cidhash[id(g)]     # Go to next state

                    # When we get here, j is the final state, now we have to locate the production
                    for r in states[j]:
                        if r.rule._prod_symbol != p.rule._prod_symbol:
                            continue
                        if r.len != p.len:
                            continue
                        # i = 0
                        # This look is comparing a production ". A B C" with "A B C ."
                        # while i < r._index:
                        #    if r._rule[i] != p._rule[i + 1]:
                        #        break
                        #    i = i + 1
                        #else:
                        #    lookb.append((j, r))
                        if p._index == 0 and r.rule.production[:r._index] == p.rule.production[:r._index]:
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
        for prod_symbol, prod in productions.items():
            if prod._empty:
                nullable.add(prod_symbol)

        # Find all non-terminal transitions
        trans = []
        for stateno, state in enumerate(states):
            for item in state:
                if item._index < item.len:
                    t = (stateno, item.rule.production[item._index])
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
    states = create_item_sets()
    add_lalr_lookahead(states)

    st = 0

    num_missing_annotations = 0
    invalid_precedence_count = 0
    num_rr = 0
    num_sr = 0

    dot_file.info('digraph Grammar {')
    for item_group in states:
        dot_file.info("  subgraph cluster_%d {", st)
        dot_file.info('    label="State %d";', st)
        dot_file.info('    style=filled;')
        dot_file.info('    color=lightgray;')
        dot_file.info('    node[style=filled;color=white];')
        state_id = id(item_group)
        for item in item_group:
            dot_file.info('    %d[label="%s"];', id(item_group[item]), item.to_string(name_map))
        for item in item_group:
            dnode = item_group[item]
            for child in dnode._direct_children:
                dot_file.info('    %d -> %d;', id(child), id(dnode))
        dot_file.info("  }")
        st += 1

    for st, item_group in enumerate(states):
        for item in item_group:
            node = item_group[item]
            for predecessor in node._predecessors:
                assert node._predecessor_lookahead is not None
                dot_file.info(
                    '  %d -> %d[label="%s"];', id(predecessor), id(node), name_map[node._predecessor_lookahead]
                )

        # Loop over each production
        st_action = {}     # type: Dict[int, List[Tuple[int, LR0Item]]]
        st_goto = {}
        sm_log.info('state %d:', st)
        sm_log.info('')
        for item in item_group:
            sm_log.info('    (%d) %s', item.rule._prod_symbol, item.to_string(name_map))
        sm_log.info('')

        for item in item_group:
            if item.len == item._index:
                if item.rule._prod_symbol == start_id:
                    # Start symbol. Accept!
                    st_action[0] = st_action.get(0, []) + [(0, item)]
                    item.rule._reduced += 1
                else:
                    # We are at the end of a production.  Reduce!
                    for a in item._lookaheads[st]:
                        st_action[a] = st_action.get(a, []) + [(-item.rule._id, item)]
                        item.rule._reduced += 1
            else:
                i = item._index
                a = item.rule.production[i] # Get symbol right after the "."
                if a < terminal_count:
                    g = goto(item_group, a)
                    j = cidhash[id(g)]
                    if j >= 0:
                        st_action[a] = st_action.get(a, []) + [(j, item)]

        for a in sorted(st_action):
            actions = st_action[a]
            action_dest = {}   # type: Dict[int, List[LR0Item]]
            for i, item in actions:
                try:
                    action_dest[i].append(item)
                except KeyError:
                    action_dest[i] = [item]

            accepted_actions = {}  # type: Dict[int, List[LR0Item]]

            if len(action_dest) > 1:
                # looks like a potential conflict, look at precedence
                conflict_log.info('State %d:', st)
                conflict_log.info('  disambiguation for lookahead %s', name_map[a])

                precedence, associativity = (-1, 'nonassoc')
                shift_actions = False
                reduce_actions = False
                assoc_error = False
                precedence_set = False
                split = False
                for j, items in action_dest.items():
                    for item in items:
                        if item._precedence is not None:
                            precedence_set = True
                            if item._precedence[1] > precedence:
                                precedence = item._precedence[1]
                                associativity = item._precedence[0]
                                assoc_error = False
                                shift_actions = j >= 0
                                reduce_actions = j < 0
                                split = item._split
                            elif item._precedence[1] == precedence:
                                if item._precedence[0] != associativity:
                                    assoc_error = True
                                shift_actions |= j >= 0
                                reduce_actions |= j < 0
                                split |= item._split
                        elif precedence == -1:
                            shift_actions |= j >= 0
                            reduce_actions |= j < 0
                            split |= item._split

                for j, items in action_dest.items():
                    for item in items:
                        if item._precedence is None and precedence_set:
                            if j >= 0:
                                conflict_log.info('  ** %s has no precedence annotation', item.to_string(name_map))
                                num_missing_annotations += 1
                            conflict_log.info('  [discarded] %s', item.to_string(name_map))
                            continue
                        elif item._precedence is not None:
                            if item._precedence[1] < precedence:
                                conflict_log.info('  [discarded] %s', item.to_string(name_map))
                                continue
                            if assoc_error:
                                conflict_log.info('  ** %s has no precedence annotation', item.to_string(name_map))
                                num_missing_annotations += 1
                            if split and not item._split:
                                conflict_log.info('  ** %s has no split annotation', item.to_string(name_map))
                                num_missing_annotations += 1
                            if j < 0 and shift_actions and associativity == 'left':
                                conflict_log.info('  [discarded] %s', item.to_string(name_map))
                                continue
                            if j >= 0 and reduce_actions and associativity == 'right':
                                conflict_log.info('  [discarded] %s', item.to_string(name_map))
                                continue
                        try:
                            accepted_actions[j].append(item)
                        except KeyError:
                            accepted_actions[j] = [item]
                        conflict_log.info('  [accepted]  %s', item.to_string(name_map))

            if len(accepted_actions) > 1 and not split:
                # handle conflicts
                conflicts = []     # type: List[Tuple[LR0DominanceNode, Text, Optional[int]]]
                num_rr += 1
                for j in sorted(accepted_actions):
                    items = accepted_actions[j]
                    for item in items:
                        node = item_group[item]
                        if j > 0:
                            conflicts.append((node, 'Shift using rule %s' % item.to_string(name_map), None))
                            num_rr -= 1
                            num_sr += 1
                        else:
                            conflicts.append((node, 'Reduce using rule %s' % item.to_string(name_map), a))

                _log_counterexamples(conflicts, conflict_log, name_map)
                conflict_log.info('')
            elif len(accepted_actions) > 1:
                splits = []
                for j in sorted(accepted_actions):
                    items = accepted_actions[j]
                    for item in items:
                        splits.append(item_group[item])
                #_find_splits()
                #conflict_log.info('')

        nkeys = set([])
        for item in item_group:
            for s in item._symbols:
                if s > terminal_count:
                    g = goto(item_group, s)
                    j = cidhash.get(id(g), -1)
                    if j >= 0:
                        if s not in nkeys:
                            nkeys.add(s)
                            sm_log.info('    %-30s shift and go to state %d', name_map[s], j)
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

    # Report errors
    if num_missing_annotations == 1:
        error_log.warning('1 missing precedence annotation')
    elif num_missing_annotations > 1:
        error_log.warning('%d missing precedence annotations', num_missing_annotations)

    if invalid_precedence_count == 1:
        error_log.warning('1 invalid precedence annotation')
    elif invalid_precedence_count > 1:
        error_log.warning('%d invalid precedence annotations', num_missing_annotations)

    if num_sr == 1:
        error_log.warning('1 shift/reduce conflict')
    elif num_sr > 1:
        error_log.warning('%d shift/reduce conflicts', num_sr)

    if num_rr == 1:
        error_log.warning('1 reduce/reduce conflict')
    elif num_rr > 1:
        error_log.warning('%d reduce/reduce conflicts', num_rr)

    for _, production in productions.items():
        for rule in production:
            if rule._reduced == 0:
                error_log.warning('Rule (%s) is never reduced', rule.to_string(name_map))


if TYPE_CHECKING:
    from be_typing import Any, Callable, Dict, List, Optional, Set, Text, Tuple
    from .grammar import Grammar
    from .lr0item import LR0Item
    from ..log import Logger
