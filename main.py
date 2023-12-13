from constants import *
from tree import *
from parser import *
from resolve import *

def solve(expr, debug=False):
    parsed = parser(expr, debug=debug)
    tree = to_tree(parsed, debug=debug)
    solved_tree = resolve(tree, debug=debug)
    return tree_str(solved_tree)

while True:
    print(solve(input(), True))