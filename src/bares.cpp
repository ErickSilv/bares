/*!
 * Infix to postfix conversion in C++
 * Input Postfix expression must be in a desired format.
 * Operands and operator.
 * Only '+', '-', '*', '%', '/', and '^' (for exponentiation) operators are expected;
 * Any other character is just ignored.
 */
#include "../include/bares.h"

/* @brief Converte um vector de Token de sua forma infixa para sua forma posfixa. 
 * @param infix_ Um vector de Token para ser tranformado.
 * @return Um vector de Token da forma posfixa da expressao.
 */
std::vector<Token> infix_to_postfix( std::vector<Token> infix_ )
{
    // Stores the postfix expression.
    std::vector<Token> postfix; // output.
    // Stack to help us convert the expression.
    std::stack< Token > s;

    // Traverse the expression.
    for( auto tk : infix_ )
    {
        // Operand goes straight to the output queue.
        if ( is_operand( tk ) ) // 1 23 100, etc.
        {
            postfix.push_back( tk );
        }
        else if ( is_operator( tk ) ) // + - ^ % etc.
        {
            // Pop out all the element with higher priority.
            while( not s.empty() and
                   has_higher_precedence( s.top() , tk ) ) 
            {
                postfix.push_back( s.top() );
                s.pop();
            }
            
            // The incoming operator always goes into the stack.
            s.push( tk );
        }
        else if ( is_opening_scope( tk ) ) // "("
        {
            s.push( tk );
        }
        else if ( is_closing_scope( tk ) ) // ")"
        {
            // pop out all elements that are not '('.
            while( not s.empty() and not is_opening_scope( s.top() ) )
            {
                postfix.push_back( s.top() ); // goes to the output.
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
        //Addicionate the top of the stack in the back of the vector.
        postfix.push_back( s.top() );
        s.pop();
    }

    return postfix;
}

bool is_operator( Token t_ )
{
    //std::string operators = "+-%^/*";
    return t_.type == Token::token_t::OPERATOR;
}

bool is_operand( Token t_ )
{
    return t_.type == Token::token_t::OPERAND;
}

bool is_opening_scope( Token t_ )
{
    return t_.type == Token::token_t::OPENING_SCOPE;
}
bool is_closing_scope( Token t_ )
{
    return t_.type == Token::token_t::CLOSING_SCOPE;
}

bool is_right_association( Token op )
{
    return op.value == "^";
}

int get_precedence( Token t_ )
{
    int weight(0);

    if ( t_.value == "^" )
    {
        return 3; //Weight = 3;
    }  
    // ==================================================
    else if ( t_.value == "*" or t_.value ==   "%" or t_.value == "/" )
    {
        return 2;
    }
    // ==================================================
    else if ( t_.value == "+" or t_.value == "-" )
    {
        return 1;
    }
    // ==================================================
    else if ( t_.value == ")" )
    {
        return 0;
    }

    return weight;
}

/// Determines qhether the first operator is > than the second operator.
bool has_higher_precedence( Token op1, Token op2 )
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

//Converte a string value do Token em um inteiro.
Parser::input_int_type str_to_int( Token input_tk_ )
{
    // Creating input stream.
    std::istringstream iss( input_tk_.value );

    // Resulting value.
    Parser::input_int_type value;
    iss >> value; // Ignore trailling white space.

    // Check for error during convertion.
    if ( iss.fail() )
        throw std::runtime_error( "str_to_int(): Erro, illegal integer format." );

    return value;
}
/* @brief Coloca operadores na pilha de acordo com seu peso e controla o resultado. 
 * @param postfix_ Um vector de Token para ser avaliado e calculadoo seu valor.
 * @return Um value_type com o valor final da expressão.
 */
value_type evaluate_postfix( std::vector<Token> postfix_ )
{
    std::stack< value_type > s;

    for( auto tk : postfix_ )
    {
        if ( is_operand( tk ) )
        {
            s.push( str_to_int(tk) );
        }
        else if ( is_operator(tk) )
        {
            // Recover the two operands in reverse order.
            auto op2 = s.top(); s.pop();
            auto op1 = s.top(); s.pop();

            // std::cout << "\n>>> Performing " << op1 << " " << tk << " " << op2 << "\n";
            auto result = execute_operator( op1, op2, tk.value );

            //std::cout << ">>> The result is: "  << result << std::endl;
            s.push(result);
        }
        else
        {
            assert(false);
        }
    }

    return s.top();
}
/* @brief Aplica um tipo de operação dependendo da entrada e retorna um value_type. 
 * @param n1 É um número do tipo value_type.
 * @param n2 É um número do tipo value_type.
 * @param opr Operação a ser realizada.
 * @return Um value_type após feita uma operação entre o n1 e n2.
 */
value_type execute_operator( value_type n1, value_type n2, std::string opr )
{
    value_type result(0);
           
           // Aplica um tipo de operação dependendo da entrada e retorna um value_type. 
           if ( opr == "^")
           {
               result = static_cast<value_type>( pow( n1, n2 ) );
           }
           else if( opr == "*")
           {
                result =  n1 * n2;
           }
           else if( opr == "/")
           {
                if ( n2 == 0 )
                {
                    std::cout << "Division by zero!\n";
                    return 0;
                }
                       
                result = n1/n2;
           }
           else if(opr == "%")
           {
                if ( n2 == 0 )
                {
                    std::cout << "Division by zero!\n";
                    return 0;
                }
                   
                
                result = n1%n2;
           }
           else if(opr == "+")
           {
                result = n1 + n2;
           }
           else if(opr == "-")
           {
                result =  n1 - n2;
           }

    return result;

}