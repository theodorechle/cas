from constants import *
from tree import *
from parser import *
from resolve import *

parsed = parser('5*((10-a-7)+3)*7')
# print('parsed :')
# for t in parsed:
#     print_tree(t)
tree = to_tree(parsed)
# print('\ntree :')
# print_tree(tree)
tree = resolve(tree)
# print('\nresolved tree :')
# print_tree(tree)
string = tree_str(tree)
print(string)