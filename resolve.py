from math import modf
from constants import *
from tree import *

def find_father_or_none(tree: Tree) -> Tree:
    if (tree.father):
        return tree.father
    return tree

def add(tree: Tree) -> Tree:
    child1, child2 = tree.childs[:2]
    if child1.nature != TYPE_NUMBER:
        return child1
    if child2.nature != TYPE_NUMBER:
        return child2
    replace_tree(tree, Tree(str(float(child1.value) + float(child2.value)), TYPE_NUMBER))
    return find_father_or_none(tree)

def substract(tree: Tree) -> Tree:
    child1, child2 = tree.childs[:2]
    if child1.nature != TYPE_NUMBER:
        return child1
    if child2.nature != TYPE_NUMBER:
        return child2
    replace_tree(tree, Tree(str(float(child1.value) - float(child2.value)), TYPE_NUMBER))
    return find_father_or_none(tree)

def multiply(tree: Tree) -> Tree:
    child1, child2 = tree.childs[:2]
    if child1.nature != TYPE_NUMBER:
        return child1
    if child2.nature != TYPE_NUMBER:
        return child2
    replace_tree(tree, Tree(str(float(child1.value) * float(child2.value)), TYPE_NUMBER))
    return find_father_or_none(tree)

def divide(tree: Tree) -> Tree:
    child1, child2 = tree.childs[:2]
    if child1.nature != TYPE_NUMBER:
        return child1
    if child2.nature != TYPE_NUMBER:
        return child2
    replace_tree(tree, Tree(str(float(child1.value) / float(child2.value)), TYPE_NUMBER))
    return find_father_or_none(tree)


def power(tree: Tree) -> Tree:
    child1, child2 = tree.childs[:2]
    if child1.nature != TYPE_NUMBER:
        return child1
    if child2.nature != TYPE_NUMBER:
        return child2
    replace_tree(tree, Tree(str(float(child1.value) ** float(child2.value)), TYPE_NUMBER))
    return find_father_or_none(tree)

def round_nbr_tree(tree: Tree) -> None:
    if tree.nature == TYPE_NUMBER and not (cut_value := modf(float(tree.value)))[0]:
        tree.value = int(cut_value[1])
    for child in tree.childs:
        round_nbr_tree(child)

def resolve(tree: Tree) -> Tree:
    while True:
        if not tree.father and not tree.childs:
            break
        elif tree.nature == TYPE_OPERATOR:
            if tree.value == '+':
                tree = add(tree)
            elif tree.value == '-':
                tree = substract(tree)
            elif tree.value == '*':
                tree = multiply(tree)
            elif tree.value == '/':
                tree = divide(tree)
            elif tree.value == '**':
                tree = power(tree)
        print('\nRoot :')
        print_tree(find_root(tree))
        print('\n')
    
    round_nbr_tree(tree)
    return tree