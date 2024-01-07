#define TYPE_NULL 0
#define TYPE_VARIABLE 1
#define TYPE_NUMBER 2
#define TYPE_OPERATOR 3
#define TYPE_OPENING_PARENTHESIS 4
#define TYPE_CLOSING_PARENTHESIS 5
#define TYPE_FUNCTION 6

#define ADDITION_SIGN "+"
#define SUBSTRACTION_SIGN "-"
#define MULTIPLICATION_SIGN "*"
#define DIVISION_SIGN "/"
#define EXPONANT_SIGN "**"
#define OPENING_PARENTHESIS_SIGN "("
#define CLOSING_PARENTHESIS_SIGN ")"

#define ADDITION_PRIORITY 1
#define SUBSTRACTION_PRIORITY 1
#define MULTIPLICATION_PRIORITY 2
#define DIVISION_PRIORITY 2
#define EXPONANT_PRIORITY 3

#define NB_OPE 5

extern const unsigned char TYPES[7][4];
extern const unsigned char* MULTIPLE_CHARS_OPERATORS[];
extern const unsigned char* OPERATORS[];