/*!
 * Infix to postfix conversion in C++
 * Input Postfix expression must be in a desired format.
 * Operands and operator, **both must be single character**.
 * Only '+', '-', '*', '%', '/', and '^' (for exponentiation) operators are expected;
 * Any other character is just ignored.
 */
#include <iostream>  // cout, endl
#include <stack>     // stack
#include <string>    // string
#include <cassert>   // assert
#include <cmath>     // pow
#include <stdexcept> // std::runtime_error


// To help change type if we want to.
using value_type = long int;


/// Simple helper functions.
bool is_operator( char c );
bool is_operand( char c );
bool is_opening_scope( char c );
bool is_closing_scope( char c );

/// Determines qhether the first operator is > than the second operator.
bool has_higher_precedence( char op1, char op2 );

/// Converts a expression in infix notation to a corresponding profix representation.
std::string infix_to_postfix( std::string );

/// Converts a char (1-digit operand) into an integer.
value_type char2integer( char ch );

/// Execute the binary operator on two operands and return the result.
value_type execute_operator( value_type, value_type, char );

/// Change an infix expression into its corresponding postfix representation.
value_type evaluate_postfix( std::string );


int main()
{
    std::string expression = "4/(5^2)+(6^2^3)";
    std::string expression2 = "1+ 3 * ( 4 + 8 * 3^7)";
    //std::string expression = "A+(B*C-(D/E^F)+G)*H";
    auto postfix = infix_to_postfix(expression2);
    std::cout << ">>> Input (infix)    = " << expression2 << "\n";
    std::cout << ">>> Output (postfix) = " << postfix << "\n";

    auto result = evaluate_postfix( postfix );
    std::cout << ">>> Result is: " << result << std::endl;
    std::cout << "\n>>> Normal exiting...\n";

    return EXIT_SUCCESS;
}

std::string infix_to_postfix( std::string infix_ )
{
    // Stores the postfix expression.
    std::string postfix; // output.
    // Stack to help us convert the expression.
    std::stack< char > s;

    // Traverse the expression.
    for( auto ch : infix_ )
    {
        // Operand goes straight to the output queue.
        if ( is_operand( ch ) ) // 1 23 100, etc.
        {
            postfix += ch;
        }
        else if ( is_operator( ch ) ) // + - ^ % etc.
        {
            // Pop out all the element with higher priority.
            while( not s.empty() and
                   has_higher_precedence( s.top() , ch ) ) 
            {
                postfix += s.top();
                s.pop();
            }
            
            // The incoming operator always goes into the stack.
            s.push( ch );
        }
        else if ( is_opening_scope( ch ) ) // "("
        {
            s.push( ch );
        }
        else if ( is_closing_scope( ch ) ) // ")"
        {
            // pop out all elements that are not '('.
            while( not s.empty() and not is_opening_scope( s.top() ) )
            {
                postfix += s.top(); // goes to the output.
                s.pop();
            }
            s.pop(); // Remove the '(' that was on the stack.
        }
        else // anything else.
        {
            // ignore this char.
        }
    }

    // Pop out all the remaining operators in the stack.
    while( not s.empty() )
    {
        postfix += s.top();
        s.pop();
    }

    return postfix;
}

bool is_operator( char c )
{
    //std::string operators = "+-%^/*";
    return std::string("+-%^/*").find( c ) != std::string::npos;
}

bool is_operand( char c )
{
    return ( c >= '0' and c<= '9');
}

bool is_opening_scope( char c )
{
    return c == '(';
}
bool is_closing_scope( char c )
{
    return c == ')';
}

bool is_right_association( char op )
{
    return op == '^';
}

int get_precedence( char c )
{
    int weight(0);
    switch( c )
    {
        case '^': weight = 3;
                  break;
        // ==================================================
        case '*':
        case '%':
        case '/': weight = 2;
                  break;
        // ==================================================
        case '+':
        case '-': weight = 1;
                  break;
        // ==================================================
        case '(':
                  weight = 0;
                  break;
        // ==================================================
        default:
            assert( false );
    }

    return weight;
}

/// Determines qhether the first operator is > than the second operator.
bool has_higher_precedence( char op1, char op2 )
{
    auto p1 = get_precedence( op1 );
    auto p2 = get_precedence( op2 );

    // special case: has the same precedence and is right association.
    if ( p1 == p2 and is_right_association( op1 ) )
    {
        return false;
    }

    return p1 >= p2 ;
}

value_type char2integer( char ch )
{
    return ch - '0';
}

value_type evaluate_postfix( std::string postfix_ )
{
    std::stack< value_type > s;

    for( auto ch : postfix_ )
    {
        if ( is_operand( ch ) )
        {
            s.push( char2integer(ch) );
        }
        else if ( is_operator(ch) )
        {
            // Recover the two operands in reverse order.
            auto op2 = s.top(); s.pop();
            auto op1 = s.top(); s.pop();

            std::cout << "\n>>> Performing " << op1 << " " << ch << " " << op2 << "\n";
            auto result = execute_operator( op1, op2, ch );

            std::cout << ">>> The result is: "  << result << std::endl;
            s.push(result);
        }
        else
        {
            assert(false);
        }
    }

    return s.top();
}
value_type execute_operator( value_type n1, value_type n2, char opr )
{
    value_type result(0);
    switch ( opr )
    {
        case '^' : result = static_cast<value_type>( pow( n1, n2 ) );
                   break;
        case '*' : result =  n1 * n2;
                   break;
        case '/' : if ( n2 == 0 )
                       throw std::runtime_error( "Division by zero" );
                   result = n1/n2;
                   break;
        case '%' : if ( n2 == 0 )
                       throw std::runtime_error( "Division by zero" );
                   result = n1%n2;
                   break;
        case '+' : result = n1 + n2;
                   break;
        case '-' : result =  n1 - n2;
                   break;
        default: assert(false);
    }

    return result;

}
