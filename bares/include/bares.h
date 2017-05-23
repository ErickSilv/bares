#include <iostream>  // cout, endl
#include <stack>     // stack
#include <string>    // string
#include <cassert>   // assert
#include <cmath>     // pow
#include <stdexcept> // std::runtime_error
#include "parser.h"
#include "token.h"


// To help change type if we want to.
using value_type = long int;


/// Simple helper functions.
bool is_operator( Token t_ );
bool is_operand( Token t_ );
bool is_opening_scope( Token t_ );
bool is_closing_scope( Token t_ );

/// Determines qhether the first operator is > than the second operator.
bool has_higher_precedence( Token op1, Token op2 );

/// Converts a expression in infix notation to a corresponding profix representation.
std::vector<Token> infix_to_postfix( std::vector<Token> infix_ );

/// Converts a std::string into an integer.
Parser::input_int_type str_to_int( Token input_tk_ );

/// Execute the binary operator on two operands and return the result.
value_type execute_operator( value_type, value_type, std::string );

/// Change an infix expression into its corresponding postfix representation.
value_type evaluate_postfix( std::vector<Token> );
