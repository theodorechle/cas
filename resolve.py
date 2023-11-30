from constants import *
from tree import *

def add(tree: Tree) -> Tree:
    child1, child2 = tree.childs[:2]
    if child1.nature != TYPE_NUMBER:
        return child1
    if child2.nature != TYPE_NUMBER:
        return child2
    replace_tree(tree, Tree(str(float(child1.value) + float(child2.value)), TYPE_NUMBER))
    return tree

def substract(tree: Tree) -> Tree:
    child1, child2 = tree.childs[:2]
    if child1.nature != TYPE_NUMBER:
        return child1
    if child2.nature != TYPE_NUMBER:
        return child2
    replace_tree(tree, Tree(str(float(child1.value) - float(child2.value)), TYPE_NUMBER))
    return tree

def multiply(tree: Tree) -> Tree:
    child1, child2 = tree.childs[:2]
    if child1.nature != TYPE_NUMBER:
        return child1
    if child2.nature != TYPE_NUMBER:
        return child2
    replace_tree(tree, Tree(str(float(child1.value) * float(child2.value)), TYPE_NUMBER))
    return tree

def divide(tree: Tree) -> Tree:
    child1, child2 = tree.childs[:2]
    if child1.nature != TYPE_NUMBER:
        return child1
    if child2.nature != TYPE_NUMBER:
        return child2
    replace_tree(tree, Tree(str(float(child1.value) / float(child2.value)), TYPE_NUMBER))
    return tree


def power(tree: Tree) -> Tree:
    child1, child2 = tree.childs[:2]
    if child1.nature != TYPE_NUMBER:
        return child1
    if child2.nature != TYPE_NUMBER:
        return child2
    replace_tree(tree, Tree(str(float(child1.value) ** float(child2.value)), TYPE_NUMBER))
    return tree

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
    return tree