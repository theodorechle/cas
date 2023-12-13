from typing import Type
from constants import *

class Tree:
    def __init__(self, value: str='', nature: int=0, parent: Type['Tree']|None=None, child_index: int=0) -> None:
        """
        value : The tree value
        nature : The nature (type) of the tree, like TYPE_OPERATOR, TYPE_INT, ...
        parent : The parent of the tree, who itself will reference the tree in his childs
        child_index : The index where the tree is in the childs' list of his father
        """
        self.value: str = value
        self.nature: int = nature
        self.parent: Tree|None = parent
        self.child_index: int = child_index
        self.childs: list[Tree] = []

    def append_tree_child(self, child: Type['Tree']) -> Type['Tree']:
        """
        Add the child at the end of the tree childs' list
        Doesn't change child
        """
        new_child = replace_tree(Tree(), child)
        new_child.parent = self
        new_child.child_index = len(self.childs)
        self.childs.append(new_child)
        return new_child
    
    def append_child(self, value: str='', nature: int=0) -> Type['Tree']:
        return self.append_tree_child(Tree(value, nature, self, len(self.childs)))


def print_tree(tree: Tree, level: int=0):
    """
    Display the value and the nature of the tree (with the TYPES dict)
    Add a tabulation for each level in the tree
    Example :
            *
          /   \\
        +       3
      /   \\
    5       7
    
    is represented like this :
    
    \* (opt)

        + (opt)

            5 (nbr)

            7 (nbr)

        3 (nbr)
    """
    print(f'{"    "*level}{tree.value} ({TYPES[tree.nature]})')
    for child in tree.childs:
        print_tree(child, level + 1)

def replace_tree(tree1: Tree, tree2: Tree) -> Tree:
    """
    Replace the value, the nature and the childs of tree1 with these of tree2
    Replace the parent of each child with tree1
    Return tree1
    """
    tree1.value = tree2.value
    tree1.nature = tree2.nature
    tree1.childs = tree2.childs
    for child in tree1.childs:
        child.parent = tree1
    return tree1

def find_root(tree: Tree) -> Tree:
    if not tree.parent:
        return tree
    return find_root(tree.parent)

def tree_str(tree: Tree) -> str:
    """
    Return a string with a representation of the tree given in parameter
    Example :
            *
          /   \\
        +       3
      /   \\
    5       7

    is returned like this :

    "(5+7)*3"
    """
    if tree.nature == TYPE_OPERATOR:
        # detect +- to replace with -
        operator_redundancy = (OPERATORS[tree.value] == 1 and (tree.childs[1].nature == TYPE_NUMBER and str(tree.childs[1].value)[0] == "-"))
        s = f'{tree_str(tree.childs[0])}{tree.value if not operator_redundancy else ""}{tree_str(tree.childs[1])}'
        parenthesis = tree.parent and (OPERATORS[tree.parent.value] > OPERATORS[tree.value] or tree.parent.value == EXPONANT) or tree.value in NO_TOGGLE
        if parenthesis:
            s = f'({s})'
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