from constants import *
from tree import *
from parser import *

parsed = parser('5*((10-7)+3)*7*5')

tree = to_tree(parsed)
print_tree(tree)