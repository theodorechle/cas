from typing import Type
from constants import TYPES

class Tree:
    def __init__(self, value: str='', nature: int=0, father: Type['Tree']|None=None, child_index: int=0) -> None:
        self.value: str = value
        self.nature: int = nature
        self.father: Tree|None = father
        self.child_index: int = child_index
        self.childs: list[Tree] = []

    def append_tree_child(self, child: Type['Tree']) -> Type['Tree']:
        child.father = self
        child.child_index = len(self.childs)
        self.childs.append(child)
        return child
    
    def append_child(self, value: str='', nature: int=0) -> Type['Tree']:
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