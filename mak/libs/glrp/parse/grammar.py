from ..log import Logger
from be_typing import TYPE_CHECKING, overload
from .lr0item import LR0Item
from . import lalr
import io
import os
import sys


class Grammar(object):
    class Rule:
        def __init__(self, id, prod_symbol, prod_name, production, action, annnotation_list, filename, lineno):
            # type: (int, int, str, Tuple[int,...], Parser.Action, List[Tuple[str, List[str], int]], str, int) -> None
            self._id = id
            self._prod_symbol = prod_symbol
            self._prod_name = prod_name
            self._production = production
            self._action = action
            self._filename = filename
            self._lineno = lineno
            predecessor = self._production[-1] if len(self._production) else None
            self._annotations = {}     # type: Dict[int, Dict[str, List[str]]]
            for annotation, values, index in annnotation_list:
                if index == len(production):
                    index = -1
                try:
                    self._annotations[index][annotation] = values
                except KeyError:
                    self._annotations[index] = {annotation: values}
            self._item = LR0Item(self, len(production), None, predecessor, [], set([-1]))
            self._reduced = 0

        def to_string(self, name_map):
            # type: (List[str]) -> str
            return '%s -> %s' % (name_map[self._prod_symbol], ' '.join([name_map[p] for p in self._production]))

        def __iter__(self):
            # type: () -> Iterator[int]
            return iter(self._production)

        @overload
        def __getitem__(self, index):
            # type: (int) -> int
            return self._production[index]

        @overload
        def __getitem__(self, index):
            # type: (slice) -> Sequence[int]
            return self._production[index]

        def __getitem__(self, index):
            # type: (Union[int, slice]) -> Union[int, Sequence[int]]
            return self._production[index]

        def __len__(self):
            # type: () -> int
            return len(self._production)

    class Production:
        def __init__(self, prod_symbol, rule_list):
            # type: (int, List[Grammar.Rule]) -> None
            self._rule_list = rule_list
            self._prod_symbol = prod_symbol
            self._first = set([])  # type: Set[int]
            self._first_list = []  # type: List[int]
            self._empty = False
            self._nonterminal_count = 0
            for rule in rule_list:
                if len(rule._production) == 0:
                    self._empty = True

        def add_rule(self, rule):
            # type: (Grammar.Rule) -> None
            self._rule_list.append(rule)
            if len(rule._production) == 0:
                self._empty = True

        def __iter__(self):
            # type: () -> Iterator[Grammar.Rule]
            return iter(self._rule_list)

        def __getitem__(self, index):
            # type: (int) -> Grammar.Rule
            return self._rule_list[index]

        def __len__(self):
            # type: () -> int
            return len(self._rule_list)

    def __init__(self, name, terminals, rules, start_symbol, parser, output_dir):
        # type: (str, Dict[str, int], List[Tuple[str, Parser.Action, List[str], List[Tuple[str, List[str], int]], str, int]], str, Parser, str) -> None
        debug_filename = os.path.join(output_dir, name + '.txt')
        index = dict(terminals)
        name_map = [''] * (len(terminals))
        log = Logger(io.open(debug_filename, 'w', encoding='utf-8'))
        stderr = Logger(io.open(sys.stderr.fileno(), 'w', encoding='utf-8', closefd=False))
        dot_file = Logger(io.open(os.path.splitext(debug_filename)[0] + '.dot', 'w', encoding='utf-8'))
        for name, i in terminals.items():
            name_map[i] = name

        rules = rules + [
            ("%s'" % start_symbol, parser.__class__.AcceptAction(), [start_symbol, name_map[0]], [], '', 1)
        ]
        for nonterminal, _, _, _, _, _ in rules:
            if nonterminal not in index:
                i = len(index)
                index[nonterminal] = i
                name_map.append(nonterminal)
                assert name_map[i] == nonterminal

        name_map.append('<epsilon>')

        start_id = len(index) - 1
        productions = _create_productions(rules, index, log)
        for prod_symbol, prod in productions.items():
            log.info(
                '%d %s {%s}', prod_symbol, name_map[prod_symbol], ', '.join([name_map[t] for t in prod._first_list])
            )
            for rule in prod:
                log.info('  %s', rule.to_string(name_map))

        _create_lr0_items(productions)
        lalr.create_parser_table(productions, start_id, name_map, len(terminals), log, stderr, dot_file)

        #with open('cxx.y', 'w') as yaccfile:
        #    for index, terminal in enumerate(name_map[2:1 + len(terminals)]):
        #        yaccfile.write('%%token _%d "%s"\n' % (index - 1, terminal))
        #    yaccfile.write('\n%%start %s\n\n%%%%\n' % start_symbol)
        #    for prod_symbol, prod in productions.items():
        #        print(name_map[prod_symbol])
        #        if prod_symbol == start_id:
        #            continue
        #        for rule in prod:
        #            if len(rule) == 0:
        #                yaccfile.write('\n%s: ;\n' % (name_map[prod_symbol]))
        #            else:
        #                yaccfile.write(
        #                    '\n%s:\n        %s\n    ;\n' % (
        #                        name_map[prod_symbol],
        #                        ' '.join(name_map[i] if i > len(terminals) else '"%s"' % name_map[i] for i in rule)
        #                    )
        #                )
        #    yaccfile.write('\n%%%%\n')

        #with open('cxx.py', 'w') as slyfile:
        #    slyfile.write('import sly\nclass CxxLexer(sly.Lexer):\n    tokens = []\n    literals = {\n')
        #    for index, terminal in enumerate(name_map[2:1 + len(terminals)]):
        #        slyfile.write('        "%s",\n' % (terminal))
        #    slyfile.write('    }\n')
        #    slyfile.write(
        #        '\nclass CxxParser(sly.Parser):\n    tokens = CxxLexer.tokens\n    debugfile = "cxxparser.out"\n'
        #    )

        #    for prod_symbol, prod in productions.items():
        #        print(name_map[prod_symbol])
        #        if prod_symbol == start_id:
        #            continue
        #        for rule in prod:
        #            if len(rule) == 0:
        #                slyfile.write(
        #                    '\n    @_(\'\')\n    def %s(self, p):\n        pass\n' %
        #                    (name_map[prod_symbol].replace('-', '_'))
        #                )
        #            else:
        #                slyfile.write(
        #                    '\n    @_(\'%s\')\n    def %s(self, p):\n        pass\n' % (
        #                        ' '.join(
        #                            name_map[i].replace('-', '_') if i > len(terminals) else
        #                            (name_map[i] if len(name_map[i]) > 1 else '"%s"' % name_map[i]) for i in rule
        #                        ), name_map[prod_symbol].replace('-', '_')
        #                    )
        #                )


def _create_productions(rules, index, log):
    # type: (List[Tuple[str, Parser.Action, List[str], List[Tuple[str, List[str], int]], str, int]], Dict[str, int], Logger) -> Dict[int, Grammar.Production]
    rule_index = 0
    productions = {}   # type: Dict[int, Grammar.Production]

    for nonterminal, action, production, attribute_list, filename, lineno in rules:
        prod_symbol = index[nonterminal]
        try:
            rule = Grammar.Rule(
                rule_index, prod_symbol, nonterminal, tuple(index[s] for s in production), action, attribute_list,
                filename, lineno
            )
        except KeyError as error:  # unknown rule or terminal
            log.error('unknown object used in rule: %s', str(error))
        else:
            rule_index += 1
            try:
                productions[prod_symbol].add_rule(rule)
            except KeyError:
                productions[prod_symbol] = Grammar.Production(prod_symbol, [rule])

    queue = list(productions.keys())
    while queue:
        prod_symbol = queue.pop(0)
        prod = productions[prod_symbol]
        len_set_before = len(prod._first)
        if prod._empty:
            prod._first.add(-1)
        for rule in prod:
            i = 0
            found_epsilon = True
            while found_epsilon:
                found_epsilon = False
                try:
                    t = rule[i]
                except IndexError:
                    break
                else:
                    i += 1
                    try:
                        p = productions[t]
                    except KeyError:
                        prod._first.add(t)
                    else:
                        for t in p._first:
                            if t != -1:
                                prod._first.add(t)
                            else:
                                found_epsilon = True
        len_set_after = len(prod._first)
        if len_set_before != len_set_after or len_set_after == 0:
            prod._first_list = sorted(prod._first)
            queue.append(prod_symbol)

    return productions


def _create_lr0_items(productions):
    # type: (Dict[int, Grammar.Production]) -> None
    for _, prods in productions.items():
        for rule in prods:
            follow = set([-1])
            i = len(rule) - 1
            while i >= 0:
                try:
                    production = productions[rule._production[i]]
                except KeyError:
                    # terminal
                    next = []
                    follow_buffer = set([rule._production[i]])
                else:
                    next = list(production)
                    follow_buffer = production._first
                    if -1 in follow_buffer:
                        follow_buffer = set(follow_buffer)
                        follow_buffer.remove(-1)
                        follow_buffer = follow_buffer.union(follow)
                predecessor = rule._production[i - 1] if i > 0 else None
                rule._item = LR0Item(rule, i, rule._item, predecessor, next, follow)
                follow = follow_buffer
                i -= 1


if TYPE_CHECKING:
    from typing import Dict, Iterator, List, Sequence, Set, Tuple, Union
    from .parser import Parser