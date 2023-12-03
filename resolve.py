from math import modf
from constants import *
from tree import *

def find_last_father(tree: Tree) -> Tree:
    if (tree.father):
        return tree.father
    return tree

def is_litteral_calculation(tree: Tree) -> Tree:
    if tree.nature == TYPE_VARIABLE:
        return True
    elif tree.nature == TYPE_OPERATOR:
        childs_litteral = []
        for child in tree.childs:
            childs_litteral.append(is_litteral_calculation(child))
            if childs_litteral[-1]:
                if child.nature == TYPE_OPERATOR and OPERATORS[child.value] <= OPERATORS[tree.value]:
                    childs_litteral[-1] = False
        if childs_litteral.count(True) < len(childs_litteral) -1:
                return False
    elif tree.nature == TYPE_NUMBER:
        return False
    return True

def check_number_in(tree: Tree) -> Tree|None:
    for child in tree.childs:
        if child.nature == TYPE_NUMBER:
            return child
        elif child.nature == TYPE_OPERATOR:
            t = check_number_in(child)
            if t != None:
                return t

def operate(tree: Tree, child: Tree, value: Tree) -> None:
    t = Tree()
    t.value = child.value
    t.nature = child.nature
    child.value = tree.value
    child.nature = tree.nature
    child.append_tree_child(t)
    child.append_tree_child(value)

def recursive_operation(tree: Tree, value: Tree, operator: str) -> None:
    for child in tree.childs:
        if child.nature == TYPE_OPERATOR:
            if OPERATORS[operator] == OPERATORS[child.value]:
                to_operate: Tree|None = check_number_in(child)
                if to_operate == None:
                    to_operate = child.childs[0]
                operate(tree, to_operate, value)
                return
            elif OPERATORS[operator] > OPERATORS[child.value]:
                recursive_operation(child, value)


def operate_on_left_litteral(tree: Tree) -> None:
    child1, child2 = tree.childs
    if child1.nature != TYPE_NUMBER:
        if child2.nature == TYPE_NUMBER:
            if not is_litteral_calculation(child1):
                return child1
            if OPERATORS[child1.value] <= OPERATORS[tree.value]:
                recursive_operation(tree, child2, tree.value)
                replace_tree(tree, child1)
                return tree
        return child1

def operate_on_right_litteral(tree: Tree) -> None:
    child1, child2 = tree.childs
    if child2.nature != TYPE_NUMBER:
        if child1.nature == TYPE_NUMBER:
            if not is_litteral_calculation(child2):
                return child2
            if OPERATORS[child2.value] <= OPERATORS[tree.value]:
                recursive_operation(tree, child1, tree.value)
                replace_tree(tree, child2)
                return tree
        return child2

def add(tree: Tree) -> Tree:
    child1, child2 = tree.childs
    t = operate_on_left_litteral(tree)
    if t != None: return t
    t = operate_on_right_litteral(tree)
    if t != None: return t
    replace_tree(tree, Tree(str(float(child1.value) + float(child2.value)), TYPE_NUMBER))
    return find_last_father(tree)

def substract(tree: Tree) -> Tree:
    child1, child2 = tree.childs
    t = operate_on_left_litteral(tree)
    if t != None: return t
    t = operate_on_right_litteral(tree)
    if t != None: return t
    replace_tree(tree, Tree(str(float(child1.value) - float(child2.value)), TYPE_NUMBER))
    return find_last_father(tree)

def multiply(tree: Tree) -> Tree:
    child1, child2 = tree.childs
    t = operate_on_left_litteral(tree)
    if t != None: return t
    t = operate_on_right_litteral(tree)
    if t != None: return t
    replace_tree(tree, Tree(str(float(child1.value) * float(child2.value)), TYPE_NUMBER))
    return find_last_father(tree)

def divide(tree: Tree) -> Tree:
    child1, child2 = tree.childs
    t = operate_on_left_litteral(tree)
    if t != None: return t
    t = operate_on_right_litteral(tree)
    if t != None: return t
    replace_tree(tree, Tree(str(float(child1.value) / float(child2.value)), TYPE_NUMBER))
    return find_last_father(tree)


def power(tree: Tree) -> Tree:
    child1, child2 = tree.childs
    t = operate_on_left_litteral(tree)
    if t != None: return t
    t = operate_on_right_litteral(tree)
    if t != None: return t
    replace_tree(tree, Tree(str(float(child1.value) ** float(child2.value)), TYPE_NUMBER))
    return find_last_father(tree)

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