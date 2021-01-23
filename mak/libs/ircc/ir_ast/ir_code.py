from be_typing import TYPE_CHECKING, cast
from .ir_type import IrAddressSpaceInference, IrTypeVoid
from .ir_expr import IrExpression, IrExpressionDeclaration
from abc import abstractmethod


class IrCodeGenContext:
    def __init__(self, equivalence, code):
        # type: (Dict[int, int], IrCodeBlock) -> None
        self._equivalence = equivalence
        self._code = code
        self._current_segment = code._segments[0]
        self._loops = []   # type: List[Tuple[IrCodeSegment, IrLoopInfo]]

    def push_loop(self, owner, loop):
        # type: (IrCodeSegment, IrLoopInfo) -> None
        assert loop not in self._loops
        self._loops.append((owner, loop))

    def pop_loop(self, owner, loop):
        # type: (IrCodeSegment, IrLoopInfo) -> None
        assert self._loops[-1][1] == loop
        self._loops.pop(-1)


class IrDominatorNode:
    def __init__(self, segment):
        # type: (IrCodeSegment) -> None
        self._segment = segment
        self._parent = None            # type: Optional[IrDominatorNode]
        self._dominators = set([self]) # type: Set[IrDominatorNode]
        self._children = []            # type: List[IrDominatorNode]
        self._exits = []               # type: List[IrCodeSegment]


class IrLoopInfo:
    def __init__(self, loop_header):
        # type: (IrCodeSegment) -> None
        self._header = loop_header
        self._body = []    # type: List[IrCodeSegment]
        self._exit = None  # type: Optional[IrCodeSegment]

    def add_loopback(self, loopback):
        # type: (IrCodeSegment) -> None
        self._body.append(loopback)
        exits = set(self._header._postdominator_node._dominators)
        for segment in self._body:
            exits.intersection_update(segment._postdominator_node._dominators)
        self._exit = None
        for e in exits:
            if e._segment == self._header:
                continue
            if e._segment in self._body:
                continue
            if self._exit is None:
                self._exit = e._segment
            elif self._exit._postdominator_node in e._dominators:
                self._exit = e._segment


class IrInstruction(IrExpression):
    def __init__(self, opcode, result, metadata):
        # type: (str, Optional[IrReference], List[Tuple[IrMetadataLink, IrMetadataLink]]) -> None
        IrExpression.__init__(self, metadata)
        self._opcode = opcode
        self._result = result
        self._result_name = None if result is None else 'var_%s' % result[1:].replace('.', '_')
        self._segment = None   # type: Optional[IrCodeSegment]
        self._phi_nodes = []   # type: List[IrInstruction]

    def collect_phi_exprs(self):
        # type: () -> List[Tuple[IrExpression, str]]
        return []

    def get_type(self, suggested_type):
        # type: (IrType) -> IrType
        return IrTypeVoid()

    def terminal(self):
        # type: () -> bool
        return False

    def labels(self):
        # type: () -> List[str]
        return []

    def declare(self, scope):
        # type: (IrScope) -> None
        if self._result:
            scope.declare(self._result, IrExpressionDeclaration(self))

    def resolve(self, module, position):
        # type: (IrModule, IrPosition) -> IrPosition
        return IrExpression.resolve(self, module, position)

    def create_instance(self, equivalence):
        # type: (Dict[int, int]) -> None
        pass

    def register_stack_data(self, generator, context):
        # type: (IrccGenerator, IrCodeGenContext) -> None
        if not self.is_inline():
            assert self._result_name is not None
            generator.declare_variable(
                self._result_name,
                self.get_type(IrTypeVoid()).create_generator_type(generator, context._equivalence)
            )

    def is_inline(self):
        # type: () -> bool
        if self._result_name is not None:
            return len(self._usage) == 1
        else:
            return True

    def create_generator_value(self, type, generator, code_context):
        # type: (IrType, IrccGenerator, IrCodeGenContext) -> IrccValue
        if self.is_inline():
            return self._create_generator_value(type, generator, code_context)
        else:
            assert self._result_name is not None
            return generator.make_value_reference(self._result_name)

    @abstractmethod
    def _create_generator_value(self, type, generator, code_context):
        # type: (IrType, IrccGenerator, IrCodeGenContext) -> IrccValue
        raise NotImplementedError

    @abstractmethod
    def generate(self, generator, context, next_segment):
        # type: (IrccGenerator, IrCodeGenContext, Optional[IrCodeSegment]) -> Optional[IrccValue]
        raise NotImplementedError


class IrAssignInstruction(IrInstruction):
    def __init__(self, result, type, expr):
        # type: (IrReference, IrType, IrExpression) -> None
        IrInstruction.__init__(self, 'phi_assign', result, [])
        self._type = type
        self._expression = expr

    def get_type(self, suggested_type):
        # type: (IrType) -> IrType
        return self._type

    def generate(self, generator, context, next_segment):
        # type: (IrccGenerator, IrCodeGenContext, Optional[IrCodeSegment]) -> Optional[IrccValue]
        assert self._result is not None
        generator.instuction_assign(
            self, self._result, self._expression.create_generator_value(self._type, generator, context)
        )
        return None

    def _create_generator_value(self, type, generator, code_context):
        # type: (IrType, IrccGenerator, IrCodeGenContext) -> IrccValue
        raise NotImplementedError


class IrCodeSegment:
    def __init__(self, label, instructions):
        # type: (str, List[IrInstruction]) -> None
        self._label = label
        self._instructions = instructions
        self._successors = []      # type: List[IrCodeSegment]
        self._predecessors = []    # type: List[IrCodeSegment]
        self._dominator_node = IrDominatorNode(self)
        self._postdominator_node = IrDominatorNode(self)
        self._loop_info = None     # type: Optional[IrLoopInfo]
        for i in self._instructions:
            i._segment = self

    def resolve(self, module, position, equivalence, return_type, return_position):
        # type: (IrModule, IrPosition, IrAddressSpaceInference, IrType, IrPosition) -> IrPosition
        for i in self._instructions:
            position = i.resolve(module, position)
        for i in self._instructions:
            i.resolve_type(equivalence, return_type, return_position)
        return position

    def _create_instance(self, equivalence):
        # type: (Dict[int, int]) -> None
        for i in self._instructions:
            i.create_instance(equivalence)

    def declare_stack_data(self, generator, context):
        # type: (IrccGenerator, IrCodeGenContext) -> None
        for instruction in self._instructions:
            instruction.register_stack_data(generator, context)

    def _write_instructions(self, generator, context, next_segment):
        # type: (IrccGenerator, IrCodeGenContext, Optional[IrCodeSegment]) -> None
        for instruction in self._instructions:
            instruction.generate(generator, context, next_segment)

    def visit(self, generator, context, next_segment):
        # type: (IrccGenerator, IrCodeGenContext, Optional[IrCodeSegment]) -> None
        if context._loops:
            if self == context._loops[-1][1]._header:
                if next_segment != context._loops[-1][1]._exit:
                    generator.instruction_continue(self._label)
                    return
                else:
                    # fallthrough to the loop start
                    return
            elif self == context._loops[-1][1]._exit:
                generator.instruction_break(self._label)
                return
        if self == next_segment:
            # fallthrough
            return
        current_segment = self
        while True:
            if current_segment._loop_info:
                generator.begin_loop(current_segment._label)
                context.push_loop(self, current_segment._loop_info)

            if current_segment._postdominator_node._parent is None:
                next_segment_children = next_segment
            else:
                next_segment_children = current_segment._postdominator_node._parent._segment
                if context._loops:
                    if next_segment_children == context._loops[-1][1]._header:
                        next_segment_children = next_segment

            context._current_segment = current_segment
            current_segment._write_instructions(generator, context, next_segment_children)

            if context._loops:
                if next_segment_children == context._loops[-1][1]._exit and self == context._loops[-1][0]:
                    context.pop_loop(self, context._loops[-1][1])
                    generator.end_loop()

            if next_segment_children is None:
                break
            current_segment = next_segment_children
            if current_segment == next_segment:
                break


class IrCodeBlock:
    def __init__(self, instructions):
        # type: (List[IrInstruction]) -> None
        self._segments = []    # type: List[IrCodeSegment]
        self._equivalence = IrAddressSpaceInference()
        label = 'start'
        stream = []            # type: List[IrInstruction]
        for instruction in instructions:
            if instruction._opcode == 'label':
                assert len(stream) == 0
                assert isinstance(instruction, ir_instructions.IrInstLabel)
                label = instruction._label
                stream.append(instruction)
            else:
                stream.append(instruction)
                if instruction.terminal():
                    self._segments.append(IrCodeSegment(label, stream))
                    stream = []
                    label = '!!!'

        for s in self._segments:
            terminal = s._instructions[-1]
            successors = terminal.labels()
            s._successors = [self._find_segment(n) for n in successors]
            for n in s._successors:
                if s not in n._predecessors:
                    n._predecessors.append(s)
        self._dominator_tree = self._build_dominator_tree()
        self._build_postdominator_tree()
        self._build_loop_info()

    def _find_segment(self, name):
        # type: (str) -> IrCodeSegment
        for s in self._segments:
            if s._label == name:
                return s
        else:
            raise NotImplementedError

    def _build_dominator_tree(self):
        # type: () -> IrDominatorNode
        all_nodes = set([s._dominator_node for s in self._segments])
        for s in self._segments[1:]:
            s._dominator_node._dominators = all_nodes
        changed = True
        while changed:
            changed = False
            for s in self._segments[1:]:
                dominator_nodes = set([]) # type: Set[IrDominatorNode]
                if s._predecessors:
                    dominator_nodes.update(s._predecessors[0]._dominator_node._dominators)
                    for pred in s._predecessors[1:]:
                        dominator_nodes.intersection_update(pred._dominator_node._dominators)
                dominator_nodes.add(s._dominator_node)
                if s._dominator_node._dominators != dominator_nodes:
                    changed = True
                    s._dominator_node._dominators = dominator_nodes
        for s in self._segments[1:]:
            parent = None                 # type: Optional[IrDominatorNode]
            for dom_node in s._dominator_node._dominators:
                if dom_node == s._dominator_node:
                    continue
                elif parent is None:
                    parent = dom_node
                elif dom_node in parent._dominators:
                    assert parent not in dom_node._dominators
                else:
                    assert parent in dom_node._dominators
                    parent = dom_node
            s._dominator_node._parent = parent
            if parent is not None:
                parent._children.append(s._dominator_node)

        def _build_exit_nodes(node):
            # type: (IrDominatorNode) -> None
            for successor in node._segment._successors:
                if node not in successor._dominator_node._dominators:
                    node._exits.append(successor)
            for child in node._children:
                _build_exit_nodes(child)
                for exit_segment in child._exits:
                    if node not in exit_segment._dominator_node._dominators:
                        if exit_segment not in node._exits:
                            node._exits.append(exit_segment)

        _build_exit_nodes(self._segments[0]._dominator_node)
        return self._segments[0]._dominator_node

    def _build_postdominator_tree(self):
        # type: () -> None
        all_nodes = set([s._postdominator_node for s in self._segments])
        for s in self._segments:
            if len(s._successors) > 0:
                s._postdominator_node._dominators = all_nodes
        changed = True
        while changed:
            changed = False
            for s in self._segments:
                dominator_nodes = set([]) # type: Set[IrDominatorNode]
                if s._successors:
                    dominator_nodes.update(s._successors[0]._postdominator_node._dominators)
                    for succ in s._successors[1:]:
                        dominator_nodes.intersection_update(succ._postdominator_node._dominators)
                dominator_nodes.add(s._postdominator_node)
                if s._postdominator_node._dominators != dominator_nodes:
                    changed = True
                    s._postdominator_node._dominators = dominator_nodes
        for s in self._segments:
            parent = None                 # type: Optional[IrDominatorNode]
            for dom_node in s._postdominator_node._dominators:
                if dom_node == s._postdominator_node:
                    continue
                elif parent is None:
                    parent = dom_node
                elif dom_node in parent._dominators:
                    assert parent not in dom_node._dominators
                else:
                    assert parent in dom_node._dominators
                    parent = dom_node
            s._postdominator_node._parent = parent
            if parent is not None:
                parent._children.append(s._postdominator_node)

    def _build_loop_info(self):
        # type: () -> None
        for s in self._segments:
            for successor in s._successors:
                if successor._dominator_node in s._dominator_node._dominators:
                    if successor._loop_info is None:
                        successor._loop_info = IrLoopInfo(successor)
                    successor._loop_info.add_loopback(s)
                    predecessors = [s]
                    while predecessors:
                        pred = predecessors.pop(0)
                        if pred == successor:
                            continue
                        if pred in successor._loop_info._body:
                            continue
                        successor._loop_info._body.append(pred)
                        predecessors += pred._predecessors

        def _print_node(node, indent, node_indent, child_indent):
            # type: (IrDominatorNode, str, str, str) -> None
            if node._segment._postdominator_node._parent is not None:
                print(
                    '%s%s%s\t\t\t[%s][%s]' % (
                        indent, node_indent, node._segment._label, ','.join([s._label for s in node._exits]),
                        node._segment._postdominator_node._parent._segment._label
                    )
                )
            else:
                print(
                    '%s%s%s\t\t\t[%s][*]' %
                    (indent, node_indent, node._segment._label, ','.join([s._label for s in node._exits]))
                )
            if node._children:
                for n in node._children[:-1]:
                    _print_node(n, indent + child_indent, ' +- ', ' |  ')
                _print_node(node._children[-1], indent + child_indent, ' `- ', '    ')

        #_print_node(self._segments[0]._dominator_node, '', '', '')

    def resolve(self, module, position, return_type, return_position):
        # type: (IrModule, IrPosition, IrType, IrPosition) -> None
        """
            Resolving a method also resolves generic address spaces.
            The resolution step will bucket together types that are constrained into the same generic address space.
            It expects the argument types to seed the buckets.
            The kernel parameters can't use the generic address space; when a kernel method is resolved, it will
            allow to infer the correct address space of all types. Any type whose generic address space can't be
            deduced at compile time will trigger a compile error.
        """
        for s in self._segments:
            position = s.resolve(module, position, self._equivalence, return_type, return_position)
        self._resolve_phi_nodes()

    def _resolve_phi_nodes(self):
        # type: () -> None
        for s in self._segments:
            for inst in s._instructions:
                phi_exprs = inst.collect_phi_exprs()
                if len(phi_exprs) == 0:
                    break
                assert inst._result is not None
                for expr, label in phi_exprs:
                    segment = self._find_segment(label[1:])
                    assert segment in s._predecessors
                    segment._instructions.insert(
                        -1, IrAssignInstruction(inst._result, inst.get_type(IrTypeVoid()), expr)
                    )

    def _create_instance(self, equivalence):
        # type: (Dict[int, int]) -> None
        for s in self._segments:
            s._create_instance(equivalence)

    def visit(self, generator, equivalence):
        # type: (IrccGenerator, Dict[int, int]) -> None
        context = IrCodeGenContext(equivalence, self)
        for s in self._segments:
            s.declare_stack_data(generator, context)
        self._segments[0].visit(generator, context, None)


from . import instructions as ir_instructions
if TYPE_CHECKING:
    from typing import Dict, List, Optional, Set, Tuple, Union
    from .ir_module import IrModule
    from .ir_metadata import IrMetadataLink
    from .ir_type import IrType
    from .ir_value import IrValue
    from .ir_expr import IrExpression
    from .ir_reference import IrReference
    from .ir_method import IrMethodParameter
    from .ir_scope import IrScope
    from ..ir_codegen import IrccGenerator, IrccValue
    from ..ir_position import IrPosition