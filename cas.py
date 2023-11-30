from typing import Type

TYPE_VARIABLE = 1
TYPE_NUMBER = 2
TYPE_OPERATOR = 3
OPENING_PARENTHESIS = 4
CLOSING_PARENTHESIS = 5
TYPE_FUNCTION = 6

TYPES = {
    0: 'NUL',
    TYPE_VARIABLE: 'var',
    TYPE_NUMBER: 'nbr',
    TYPE_OPERATOR: 'opt',
    OPENING_PARENTHESIS: 'opa',
    CLOSING_PARENTHESIS: 'cpa',
    TYPE_FUNCTION: 'func'
}

OPERATORS = {
    '+': 1,
    '-': 1,
    '*': 2,
    '/': 2,
    '**': 3,
}

class Tree:
    def __init__(self, value: str='', nature: int=0, father: Type['Tree']|None=None, child_index=0) -> None:
        self.value = value
        self.nature = nature
        self.father = father
        self.child_index = child_index
        self.childs = []

    def append_tree_child(self, child: Type['Tree']) -> Type['Tree']:
        self.childs.append(child)
        return child
    
    def append_child(self, value: str='', nature: int=0) -> Type['Tree']:
        return self.append_tree_child(Tree(value, nature, self, len(self.childs)))


def print_tree(tree: Tree, level: int=0):
    print(f'{"    "*level}{tree.value} ({TYPES[tree.nature]})')
    for child in tree.childs:
        print_tree(child, level + 1)

def is_nature(nature: int, check_nature: int):
    return not nature or nature == check_nature

def parser(expr: str) -> list[Tree]:
    expr_list: list[Tree] = []
    value: str = ''
    nature: int = 0
    length: int = len(expr)
    index_: int = 0

    while index_ < length:
        new_tree: bool = True
        char = expr[index_]
        if char.isspace():
            if not value:
                new_tree = False
        elif char.isalpha():
            if is_nature(nature, TYPE_VARIABLE):
                if not value and expr_list and expr_list[-1].nature == TYPE_NUMBER:
                    value = '*'
                    nature = TYPE_VARIABLE
                    index_ -= 1
                else:
                    value += char
                    nature = TYPE_VARIABLE
                    new_tree = False
            else:
                index_ -= 1
        elif char == '(':
            if not nature:
                if expr_list and expr_list[-1].nature in (TYPE_NUMBER, TYPE_VARIABLE, TYPE_FUNCTION):
                    value = '*'
                    nature = TYPE_OPERATOR
                    index_ -= 1
                else:
                    nature = OPENING_PARENTHESIS
            else:
                index_ -= 1
        elif char == ')':
            if not nature:
                nature = CLOSING_PARENTHESIS
            else:
                index_ -= 1
        elif char.isdigit():
            if is_nature(nature, TYPE_NUMBER):
                value += char
                nature = TYPE_NUMBER
                new_tree = False
            elif nature == TYPE_VARIABLE:
                value += char
                nature = TYPE_VARIABLE
                new_tree = False
        elif char in OPERATORS:
            if is_nature(nature, TYPE_OPERATOR):
                if char == '*' and expr_list and expr_list[-1].value == '*':
                    expr_list[-1].value = '**'
                    new_tree = False
                else:
                    value = char
                    nature = TYPE_OPERATOR
            else:
                index_ -= 1
        if new_tree:
            expr_list.append(Tree(value, nature))
            value = ''
            nature = 0
        index_ += 1
    expr_list.append(Tree(value, nature))
    return expr_list

def replace_tree(tree1: Tree, tree2: Tree) -> Tree:
    tree1.value = tree2.value
    tree1.nature = tree2.nature
    tree1.childs = tree2.childs
    return tree1

def find_root(tree: Tree) -> Tree:
    if not tree.father:
        return tree
    return find_root(tree.father)

def find_root_or_parenthesis(tree: Tree) -> Tree:
    if not tree.father or tree.father.nature == OPENING_PARENTHESIS:
        return tree
    return find_root_or_parenthesis(tree.father)

def remove_parenthesis(tree: Tree) -> Tree:
    if tree.nature == CLOSING_PARENTHESIS:
        replace_tree(tree, tree.childs[0])
    for child in tree.childs:
        remove_parenthesis(child)

def to_tree(expr_list: list[Tree]) -> Tree:
    tree = Tree()
    for t in expr_list:
        if t.nature == TYPE_NUMBER:
            replace_tree(tree, t)
            tree = find_root_or_parenthesis(tree)
        elif t.nature == TYPE_OPERATOR:
            if not (is_opr:=tree.nature == TYPE_OPERATOR) or OPERATORS[t.value] <= OPERATORS[tree.value]:
                print('tree.value : ', tree.value)
                replace_tree(t.append_child(), tree)
                replace_tree(tree, t)
                tree = tree.append_child()
            elif is_opr:
                child: Tree = tree.childs[-1]
                while child.nature == TYPE_OPERATOR and OPERATORS[t.value] > OPERATORS[child.value]:
                        child = child.childs[-1]
                replace_tree(t.append_child(), child)
                tree = replace_tree(child.father.childs[-1], t).append_child()
        elif t.nature == OPENING_PARENTHESIS:
            replace_tree(tree, t)
            tree = tree.append_child()
        elif t.nature == CLOSING_PARENTHESIS:
            tree.father.nature = CLOSING_PARENTHESIS
            tree = find_root_or_parenthesis(tree)
        print("tree :")
        print_tree(find_root(tree))
        print("\n")
    
    remove_parenthesis(find_root(tree))
    return find_root(tree)

parsed = parser('5*((10-7)+3)*7*5')
# for value in parsed:
#     print_tree(value)
tree = to_tree(parsed)
print_tree(tree)