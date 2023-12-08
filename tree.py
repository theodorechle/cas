from constants import *

class Tree:
    def __init__(self, value: str='', nature: int=0, father: 'Tree'|None=None, child_index: int=0) -> None:
        self.value: str = value
        self.nature: int = nature
        self.father: Tree|None = father
        self.child_index: int = child_index
        self.childs: list[Tree] = []

    def append_tree_child(self, child: 'Tree') -> 'Tree':
        child.father = self
        child.child_index = len(self.childs)
        self.childs.append(child)
        return child
    
    def append_child(self, value: str='', nature: int=0) -> 'Tree':
        return self.append_tree_child(Tree(value, nature, self, len(self.childs)))


def print_tree(tree: Tree, level: int=0):
    print(f'{"    "*level}{tree.value} ({TYPES[tree.nature]})')
    for child in tree.childs:
        print_tree(child, level + 1)

def replace_tree(tree1: Tree, tree2: Tree) -> Tree:
    tree1.value = tree2.value
    tree1.nature = tree2.nature
    tree1.childs = tree2.childs
    for child in tree1.childs:
        child.father = tree1
    return tree1

def find_root(tree: Tree) -> Tree:
    if not tree.father:
        return tree
    return find_root(tree.father)

def tree_str(tree: Tree) -> Tree:
    if tree.nature == TYPE_OPERATOR:
        s = f'({tree_str(tree.childs[0])}{tree.value}{tree_str(tree.childs[1])})'
    elif tree.nature == TYPE_FUNCTION:
        s = f'{tree.value}('
        if tree.childs:
            s += tree_str(tree.childs[0])
            for child in tree.childs[1:]:
                s += f',{tree_str(child)}'
        s += ')'
    else:
        s = tree.value
    return s

def tree_length(tree: Tree) -> int:
    nb = 0
    for child in tree.childs:
        if (n := tree_length(child)) > nb:
            nb = n
    return nb + 1