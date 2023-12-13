from constants import *
from tree import *

def is_nature(nature: int, check_nature: int):
    """
    Return True if the nature is empty or if it's like check_empty
    """
    return not nature or nature == check_nature

def parser(expr: str, debug=False) -> list[Tree]:
    """
    Parse a string mathematical expression into a list
    Add implicit mutliplications
    """
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
                # implicit multiplication
                if not value and expr_list and expr_list[-1].nature in (TYPE_NUMBER, CLOSING_PARENTHESIS):
                    value = MULTIPLICATION
                    nature = TYPE_OPERATOR
                    index_ -= 1
                else:
                    value += char
                    nature = TYPE_VARIABLE
                    new_tree = False
            else:
                index_ -= 1
        elif char == '(':
            if not nature:
                # implicit multiplication
                if expr_list and expr_list[-1].nature in (TYPE_NUMBER, TYPE_VARIABLE, CLOSING_PARENTHESIS):
                    value = MULTIPLICATION
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
                # check for operators with multiple characters, like "**" for power
                found = False
                for ope in MULTIPLE_CHARS_OPERATORS:
                    if found:
                        break
                    l = len(ope)
                    if l > len(expr_list):
                        continue
                    c = char
                    for i, v in enumerate(ope):
                        if c != v:
                            break
                        if i == l - 1:
                            for _ in range(i-1):
                                expr_list[-1].pop()
                            expr_list[-1].value = ope
                            found = True
                            new_tree = False
                            break
                        c = expr_list[-i-1].value
                if not found:
                    value = char
                    nature = TYPE_OPERATOR
            else:
                index_ -= 1
        if new_tree:
            expr_list.append(Tree(value, nature))
            value = ''
            nature = 0
        index_ += 1
    
    if value or nature:
        expr_list.append(Tree(value, nature))
    
    if debug:
      for l in expr_list:
        print_tree(l)
    return expr_list

def find_root_or_parenthesis(tree: Tree) -> Tree:
    if not tree.parent or tree.parent.nature == OPENING_PARENTHESIS:
        return tree
    return find_root_or_parenthesis(tree.parent)

def remove_parenthesis(tree: Tree) -> Tree:
    if tree.nature == CLOSING_PARENTHESIS:
        replace_tree(tree, tree.childs[0])
    for child in tree.childs:
        remove_parenthesis(child)

def to_tree(expr_list: list[Tree], debug=False) -> Tree:
    if debug:
      print("Parser :")
    tree = Tree()
    for t in expr_list:
        if t.nature == TYPE_NUMBER or t.nature == TYPE_VARIABLE:
            replace_tree(tree, t)
            tree = find_root_or_parenthesis(tree)
        elif t.nature == TYPE_OPERATOR:
            if not (is_opr:=tree.nature == TYPE_OPERATOR) or OPERATORS[t.value] <= OPERATORS[tree.value]:
                if not tree.nature and t.value == '-':
                    replace_tree(tree, Tree('0', TYPE_NUMBER))
                replace_tree(t.append_child(), tree)
                replace_tree(tree, t)
                tree = tree.append_child()
            elif is_opr:
                child: Tree = tree.childs[-1]
                while child.nature == TYPE_OPERATOR and OPERATORS[t.value] > OPERATORS[child.value]:
                        child = child.childs[-1]
                replace_tree(t.append_child(), child)
                tree = replace_tree(child.parent.childs[-1], t).append_child()
        elif t.nature == OPENING_PARENTHESIS:
#            print("WWWW")
#            print_tree(tree)
#            tree = tree.append_child()
            if not TYPES[tree.nature] == 'NUL':
              tree = tree.append_child()
            replace_tree(tree, t)
#            print("545")
#            print_tree(tree)
            tree = tree.append_child()
#            print("4256145")
#            print_tree(tree)
        elif t.nature == CLOSING_PARENTHESIS:
            tree.parent.nature = CLOSING_PARENTHESIS
            tree = find_root_or_parenthesis(tree)
        if debug:
            print('\nRoot :')
            print_tree(find_root(tree))
            print('\n')
    remove_parenthesis(find_root(tree))
    return find_root(tree)
