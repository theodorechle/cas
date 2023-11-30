TYPE_VARIABLE = 1
TYPE_NUMBER = 2
TYPE_OPERATOR = 3
OPENING_PARENTHESIS = 4
CLOSING_PARENTHESIS = 5
TYPE_FUNCTION = 6

TYPES = {
    0: 'NUL',
    TYPE_VARIABLE: 'var',
    TYPE_NUMBER: 'nbr',
    TYPE_OPERATOR: 'opt',
    OPENING_PARENTHESIS: 'opa',
    CLOSING_PARENTHESIS: 'cpa',
    TYPE_FUNCTION: 'func'
}

OPERATORS = {
    '+': 1,
    '-': 1,
    '*': 2,
    '/': 2,
    '**': 3,
}
