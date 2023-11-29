from typing import Type

OPERATORS = {
    '+': 1,
    '-': 1,
    '*': 2,
    '/': 2,
    '**': 3,
}

class Tree:
    def __init__(self, value: str='', nature: str='', father: Type['Tree']|None=None, child_index=0) -> None:
        self.value = value
        self.nature = nature
        self.father = father
        self.child_index = child_index
        self.childs = []

    def append_tree_child(self, child: Type['Tree']) -> Type['Tree']:
        self.childs.append(child)
        return child
    
    def append_child(self, value: str='', nature: str='') -> Type['Tree']:
        return self.append_tree_child(Tree(value, nature, self, len(self.childs)))


def print_tree(tree: Tree, level: int=0):
    print(f'{"    "*level}{tree.value} ({tree.nature})')
    for child in tree.childs:
        print_tree(child, level + 1)

def is_nature(nature: str, check_nature: str):
    return not nature or nature == check_nature

def parser(expr: str) -> list[Tree]:
    expr_list: list[Tree] = []
    value: str = ''
    nature: str = ''
    length: int = len(expr)
    index_: int = 0

    while index_ < length:
        new_tree: bool = True
        char = expr[index_]
        if char.isspace():
            if not value:
                new_tree = False
        elif char.isalpha():
            if is_nature(nature, 'var'):
                if not value and expr_list and expr_list[-1].nature == 'int':
                    value = '*'
                    nature = 'ope'
                    index_ -= 1
                else:
                    value += char
                    nature = 'var'
                    new_tree = False
            else:
                index_ -= 1
        elif char.isdigit():
            if is_nature(nature, 'int'):
                value += char
                nature = 'int'
                new_tree = False
            elif nature == 'var':
                value += char
                nature = 'var'
                new_tree = False
        elif char in OPERATORS:
            if is_nature(nature, 'opr'):
                if char == '*' and expr_list and expr_list[-1].value == '*':
                    expr_list[-1].value = '**'
                    new_tree = False
                else:
                    value = char
                    nature = 'opr'
            else:
                index_ -= 1
        if new_tree:
            expr_list.append(Tree(value, nature))
            value = ''
            nature = ''
        index_ += 1
    expr_list.append(Tree(value, nature))
    return expr_list

def replace_tree(tree1: Tree, tree2: Tree) -> Tree:
    tree1.value = tree2.value
    tree1.nature = tree2.nature
    tree1.childs = tree2.childs
    return tree1

def find_root(tree: Tree) -> Tree:
    if (not tree.father):
        return tree
    return find_root(tree.father)

def to_tree(expr_list: list[Tree]) -> Tree:
    tree = Tree()
    for t in expr_list:
        if t.nature == 'int':
            replace_tree(tree, t)
            tree = find_root(tree)
        elif t.nature == 'opr':
            if not (is_opr:=tree.value in OPERATORS) or OPERATORS[t.value] <= OPERATORS[tree.value]:
                replace_tree(t.append_child(), tree)
                tree = t.append_child()
            elif is_opr:
                child: Tree = tree.childs[-1]
                while child.nature == 'opr' and OPERATORS[t.value] > OPERATORS[child.value]:
                        child = child.childs[-1]
                replace_tree(t.append_child(), child)
                tree = replace_tree(child.father.childs[-1], t).append_child()

    return find_root(tree)

parsed = parser('3-5*3+7/3+5')
# for value in parsed:
#     print_tree(value)
tree = to_tree(parsed)
print_tree(tree)