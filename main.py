from constants import *
from tree import *
from parser import *

parsed = parser('5*((10-7)+3)*7*5')
# for value in parsed:
#     print_tree(value)
tree = to_tree(parsed)
print_tree(tree)