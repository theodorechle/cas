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

def replace_tree(tree1: Tree, tree2: Tree) -> None:
    tree1.value = tree2.value
    tree1.nature = tree2.nature
    tree1.childs = tree2.childs

def to_tree(expr_list: list[Tree]) -> Tree:
    tree = Tree()
    for t in expr_list:
        if t.nature == 'int':
            replace_tree(tree, t)
            t = tree
        elif t.nature == 'opr':
            replace_tree(t.append_child(), tree)
            t = t.append_child()

        tree = t
    while tree.father != None:
        tree = tree.father
    return tree

parsed = parser('5*3+7+3')
# for value in parsed:
#     print_tree(value)
tree = to_tree(parsed)
print_tree(tree)