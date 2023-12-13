from math import modf
from constants import *
from tree import *

def find_last_parent(tree: Tree) -> Tree:
    if (tree.parent):
        return tree.parent
    return tree

def is_litteral_calculation(tree: Tree) -> Tree:
    if tree.nature == TYPE_VARIABLE:
        return True
    elif tree.nature == TYPE_OPERATOR:
        childs_litteral = []
        for child in tree.childs:
            childs_litteral.append(is_litteral_calculation(child))
            if child.nature == TYPE_OPERATOR and OPERATORS[child.value] <= OPERATORS[tree.value] and child.value != "/":
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

def operate(child: Tree, value: Tree, operator: str) -> None:
    t = replace_tree(Tree(), child)
    child.value = operator
    child.nature = TYPE_OPERATOR
    child.childs = []
    child.append_tree_child(value)
    child.append_tree_child(t)

def recursive_operation(tree: Tree, value: Tree, operator: str, first_recursion: bool = False) -> None|int:
    for child in tree.childs:
        if operator == '-':
            if child.child_index == 0:
                if first_recursion:
                    recursive_operation(tree.childs[not value.child_index], value, operator)
                    return 1
                operate(child, value, operator)
                continue
            operate(child, Tree('0', TYPE_NUMBER), operator)
            continue
        if child.nature == TYPE_OPERATOR:
            if child.value == "/":
                return -1
            if OPERATORS[operator] == OPERATORS[child.value]:
                if child.value == "-" or (to_operate := check_number_in(child)) == None:
                    to_operate = child.childs[0]
                operate(to_operate, value, operator)
                return
            elif OPERATORS[operator] > OPERATORS[child.value]:
                return recursive_operation(child, value, operator)
        elif not first_recursion:
            operate(child, value, operator)

def return_next_tree(tree: Tree) -> Tree:
    child2 = tree.childs[1]
    if child2.nature != TYPE_OPERATOR or is_litteral_calculation(child2):
        if tree.parent:
            if tree.child_index == 0:
                return tree.parent.childs[1]
            else:
                return tree.parent
        return tree
    return child2

def operate_on_left_litteral(tree: Tree) -> None:
    child1, child2 = tree.childs
    if child1.nature == TYPE_OPERATOR:
        if child2.nature != TYPE_NUMBER and not is_litteral_calculation(child1):
            return child1
        if OPERATORS[child1.value] > OPERATORS[tree.value]:
            return return_next_tree(tree)
        if recursive_operation(tree, child2, tree.value, True) != None:
            if tree.parent: return tree.parent
        replace_tree(tree, child1)
        return tree
    if child1.nature == TYPE_VARIABLE:
        return return_next_tree(tree)

def operate_on_right_litteral(tree: Tree) -> None:
    child1, child2 = tree.childs
    if child2.nature == TYPE_OPERATOR:
        if child1.nature != TYPE_NUMBER and not is_litteral_calculation(child2):
                return child2
        if OPERATORS[child2.value] > OPERATORS[tree.value]:
            return return_next_tree(tree)
        if recursive_operation(tree, child1, tree.value, True) != None:
            if tree.parent: return tree.parent
        replace_tree(tree, child2)
        return tree
    if child2.nature == TYPE_VARIABLE:
        return return_next_tree(tree)


def add(tree: Tree) -> Tree:
    t = operate_on_left_litteral(tree)
    if t != None: return t
    t = operate_on_right_litteral(tree)
    if t != None: return t
    child1, child2 = tree.childs
    replace_tree(tree, Tree(str(float(child1.value) + float(child2.value)), TYPE_NUMBER))
    return find_last_parent(tree)

def substract(tree: Tree) -> Tree:
    t = operate_on_left_litteral(tree)
    if t != None: return t
    t = operate_on_right_litteral(tree)
    if t != None: return t
    child1, child2 = tree.childs
    replace_tree(tree, Tree(str(float(child1.value) - float(child2.value)), TYPE_NUMBER))
    return find_last_parent(tree)

def multiply(tree: Tree) -> Tree:
    t = operate_on_left_litteral(tree)
    if t != None: return t
    t = operate_on_right_litteral(tree)
    if t != None: return t
    child1, child2 = tree.childs
    replace_tree(tree, Tree(str(float(child1.value) * float(child2.value)), TYPE_NUMBER))
    return find_last_parent(tree)

def divide(tree: Tree) -> Tree:
    t = operate_on_left_litteral(tree)
    if t != None: return t
    t = operate_on_right_litteral(tree)
    if t != None: return t
    child1, child2 = tree.childs
    replace_tree(tree, Tree(str(float(child1.value) / float(child2.value)), TYPE_NUMBER))
    return find_last_parent(tree)


def power(tree: Tree) -> Tree:
    t = operate_on_left_litteral(tree)
    if t != None: return t
    t = operate_on_right_litteral(tree)
    if t != None: return t
    child1, child2 = tree.childs
    replace_tree(tree, Tree(str(float(child1.value) ** float(child2.value)), TYPE_NUMBER))
    return find_last_parent(tree)

def round_nbr_tree(tree: Tree) -> None:
    if tree.nature == TYPE_NUMBER and not (cut_value := modf(float(tree.value)))[0]:
        tree.value = int(cut_value[1])
    for child in tree.childs:
        round_nbr_tree(child)

def resolve(tree: Tree, debug=False) -> Tree:
    if debug:
      print("Resolver :")
    s = tree_str(tree)
    c = 0
    length = tree_length(tree)
    while True:
        print(tree_str(find_root(tree)))
        if not tree.parent and not tree.childs:
            break
        elif tree.nature == TYPE_OPERATOR:
            if tree.value == ADDITION:
                tree = add(tree)
            elif tree.value == SUBSTRACTION:
                tree = substract(tree)
            elif tree.value == MULTIPLICATION:
                tree = multiply(tree)
            elif tree.value == DIVISION:
                tree = divide(tree)
            elif tree.value == EXPONANT:
                tree = power(tree)
        print(tree_str(find_root(tree)))
        if debug:
            print('\nRoot :')
            print_tree(find_root(tree))
            print('\n')
        if c == length:
            root = find_root(tree)
            if s != (new_s := tree_str(root)):
                s = new_s
                length = tree_length(root)
                c = 0
            else:
                tree = root
                break
        else:
            c += 1
    round_nbr_tree(tree)
    return tree
    