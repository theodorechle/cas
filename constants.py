TYPE_VARIABLE = 1
TYPE_NUMBER = 2
TYPE_OPERATOR = 3
TYPE_OPENING_PARENTHESIS = 4
TYPE_CLOSING_PARENTHESIS = 5
TYPE_FUNCTION = 6

TYPES = {
    0: 'NUL',
    TYPE_VARIABLE: 'var',
    TYPE_NUMBER: 'nbr',
    TYPE_OPERATOR: 'opt',
    TYPE_OPENING_PARENTHESIS: 'opa',
    TYPE_CLOSING_PARENTHESIS: 'cpa',
    TYPE_FUNCTION: 'func'
}

ADDITION = '+'
SUBSTRACTION = '-'
MULTIPLICATION = '*'
DIVISION = '/'
EXPONANT = '**'

OPERATORS = {
    ADDITION: 1,
    SUBSTRACTION: 1,
    MULTIPLICATION: 2,
    DIVISION: 2,
    EXPONANT: 3,
}

MULTIPLE_CHARS_OPERATORS = [ope for ope in OPERATORS.keys() if len(ope) > 1]

NO_TOGGLE = '-/'