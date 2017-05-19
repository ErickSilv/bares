#include "../include/parser.h"
#include <iterator>
#include <algorithm>

/// Converts a valid character to the corresponding terminal symbol.
Parser::terminal_symbol_t  Parser::lexer( char c_ ) const
{
    switch( c_ )
    {
        case '+':  return terminal_symbol_t::TS_PLUS;
        case '-':  return terminal_symbol_t::TS_MINUS;
        case '^':  return terminal_symbol_t::TS_EXPO;
        case '*':  return terminal_symbol_t::TS_MULT;
        case '/':  return terminal_symbol_t::TS_DIV;
        case ' ':  return terminal_symbol_t::TS_WS;
        case   9:  return terminal_symbol_t::TS_TAB;
        case '0':  return terminal_symbol_t::TS_ZERO;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':  return terminal_symbol_t::TS_NON_ZERO_DIGIT;
        case '\0': return terminal_symbol_t::TS_EOS; // end of string: the $ terminal symbol
    }
    return terminal_symbol_t::TS_INVALID;
}


/// Convert a terminal symbol into its corresponding string representation.
std::string Parser::token_str( terminal_symbol_t s_ ) const
{
    switch( s_ )
    {

        case terminal_symbol_t::TS_PLUS   : return "+";
        case terminal_symbol_t::TS_MINUS  : return "-";
        case terminal_symbol_t::TS_EXPO   : return "^";
        case terminal_symbol_t::TS_MULT   : return "*";
        case terminal_symbol_t::TS_DIV    : return "/";
        case terminal_symbol_t::TS_WS     : return " ";
        case terminal_symbol_t::TS_ZERO   : return "0";
        default                           : return "X";
    }
}

/// Consumes a valid character from the expression being parsed.
void Parser::next_symbol( void )
{
    // Get a valid symbol for processing
    std::advance( it_curr_symb, 1 );
}

/// Verifies whether the current symbol is equal to the terminal symbol requested.
bool Parser::peek( terminal_symbol_t c_ ) const
{
    // Verificar se o código fornecido no argumento corresponde
    // ao caractere na "ponta da agulha" (apontado pelo iterador).
    return ( not end_input() and
             lexer( *it_curr_symb ) == c_ );
}

/// Tries to match the current character to a symbol passed as argument.
bool Parser::accept( terminal_symbol_t c_ )
{
    // Se o caractere atual for o requisitado, o método consome o
    // caractere da entrada.
    if ( peek( c_ ) )
    {
        next_symbol();
        return true;
    }

    return false;
}

/// Verify whether the next valid symbol is the one expected; if it is so, the method accepts it.
bool Parser::expect( terminal_symbol_t c_ )
{
    // Salte todos os caracteres em branco e tente validar
    // o primeiro caractere que surgir.
    skip_ws();
    return accept( c_ );
}


/// Ignores any white space or tabs in the expression until reach a valid symbol or end of input.
void Parser::skip_ws( void )
{
    // Simplemente salta todos os caracteres em branco.
    // Lembrar de verificar se a entrada não acabou, para evitar de
    // acessar um iterador inválido.
    while ( not end_input() and
            ( lexer( *it_curr_symb ) == Parser::terminal_symbol_t::TS_WS  or
              lexer( *it_curr_symb ) == Parser::terminal_symbol_t::TS_TAB ) )
    {
        next_symbol();
    }
}

/// Checks whether we reached the end of the expression string.
bool Parser::end_input( void ) const
{
    // "Fim de entrada" ocorre quando o iterador chega ao
    // fim da string que guarda a expressão.
    return it_curr_symb == expr.end();
}

/// Converts from string to integer.
Parser::input_int_type str_to_int( std::string input_str_ )
{
    // Creating input stream.
    std::istringstream iss( input_str_ );

    // Resulting value.
    Parser::input_int_type value;
    iss >> value; // Ignore trailling white space.

    // Check for error during convertion.
    if ( iss.fail() )
        throw std::runtime_error( "str_to_int(): Erro, illegal integer format." );

    return value;
}

//=== NTS methods.

/*! Processando uma expressão.
 *
 * Produção:
 * <expr> := <term>,{ ("+"|"-"),<term> }
 *
 * De acordo com a gramática (acima), uma expressão pode ser apenas um
 * termo isolado ou seguido de um ou mais termos com um + ou - entre eles.
 *
 */
Parser::ParserResult Parser::expression()
{
    skip_ws(); // Salta todos os espaços em branco.

    // (1) Vamos validar um termo.
    auto result = term();

    // Usamos um 'enquanto' pois podem vir 1 ou mais termos ligados por +/-.
    while ( result.type == ParserResult::PARSER_OK  and not end_input() )
    {
        // ============================================================
        // Se chegamos aqui significa que um termo **válido** foi
        // consumido da entrada (expressão), ou através do método
        // term() fora do laço (na 1ª vez) ou através do método
        // term() invocado no final do laço (para as demais vezes).
        // Portanto, devemos verificar se existem novos termos
        // na entrada (expressão), precedidos por +/-.
        // ============================================================

        // (2) Pode vir um '+', ou seja, "esperamos" um '+'...
        if ( expect( terminal_symbol_t::TS_PLUS ) )
        {
            // Ok, recebemos:
            // Token( "+", OPERATOR )
            token_list.push_back( Token("+", Token::token_t::OPERATOR) );
        }
        // (3) ... mas pode vir um '-', ou seja, também "esperamos" um '-'.
        else if ( expect( terminal_symbol_t::TS_MINUS ) ) // ou um '-'
        {
            // Ok, recebemos:
            // Token( "-", OPERATOR )
            token_list.push_back( Token("-", Token::token_t::OPERATOR) );
        }
        else if ( expect( terminal_symbol_t::TS_EXPO ) )
        {
            // Ok, recebemos:
            // Token( "^", OPERATOR )
            token_list.push_back( Token("^", Token::token_t::OPERATOR) );
        }
        else if ( expect( terminal_symbol_t::TS_MULT ) )
        {
            // Ok, recebemos:
            //Token( "*", OPERATOR )
            token_list.push_back( Token("*", Token::token_t::OPERATOR) );
        }
        else if ( expect( terminal_symbol_t::TS_DIV ) )
        {
            // Ok, recebemos:
            // Token( "/", OPERATOR )
            token_list.push_back( Token("^", Token::token_t::OPERATOR) );
        }
        else if ( expect( terminal_symbol_t::TS_ZERO ) )
        {
            //Ok, recebemos:
            //Token( "0", OPERAND )
            token_list.push_back( Token("0", Token::token_t::OPERAND) );
        }
        else if ( expect( terminal_symbol_t::TS_NON_ZERO_DIGIT ) )
        {
            //Ok, recebemos:
            //Token( "1->9", OPERAND )
            auto result = term();
            token_list.push_back( Token( result, OPERAND ) )

        }
        // ... se não for =/-, quer dizer que a expressão acabou.
        else return result;

        // (4) Se chegamos aqui é porque recebemos com sucesso um "+"
        // ou um "-". Então agora TEM QUE VIR UM TERMO!.
        // Se não vier um termo, então temos um erro de sintaxe.

        result = term(); // consumir um termo da entrada (expressão).
        if ( result.type != ParserResult::PARSER_OK ) // deu pau, não veio um termo.
        {
            // Se o termo não foi encontrado, atualizamos a mensagem
            // de erro (ParserResult) recebida com um tipo mais
            // explicativo para o cliente.
            result.type = ParserResult::MISSING_TERM;
            return result;
        }
    }

    return result;
}

Parser::ParserResult Parser::term()
{
    skip_ws();
    auto begin_token( it_curr_symb ); // Guardar o início da string do token.
    auto result = integer();

    // TODO: Capturar o Token( [begin_token, it_curr_symbol), OPERAND )
    
    return result;
}

Parser::ParserResult Parser::integer()
{
    // Serah que eh um zero?
    if ( lexer( *it_curr_symb ) ==  terminal_symbol_t::TS_ZERO ) 
    {
        return ParserResult( ParserResult::PARSER_OK ); 
    }
    // Tratar o '-' unário.
    accept( terminal_symbol_t::TS_MINUS );

    return natural_number();
}

Parser::ParserResult Parser::natural_number()
{
    if ( digit_excl_zero() )
    {
        while( digit() ) /* empty */ ;

        return ParserResult( ParserResult::PARSER_OK );
    }

    return ParserResult( ParserResult::ILL_FORMED_INTEGER,
            std::distance( expr.begin(), it_curr_symb ) );
}

bool Parser::digit_excl_zero()
{
    return accept( terminal_symbol_t::TS_NON_ZERO_DIGIT );
}

bool Parser::digit()
{
    return ( accept( terminal_symbol_t::TS_ZERO ) or
             accept( terminal_symbol_t::TS_NON_ZERO_DIGIT ) );
}

/*!
 * This is the parser's entry point.
 * This method tries to (recursivelly) validate the expression.
 * During this process, we also stored the tokens into a container.
 *
 * \param e_ The string with the expression to parse.
 * \return The parsing result.
 */
Parser::ParserResult
Parser::parse( std::string e_ )
{
    // We reset the parsing process each new expression.
    expr = e_;  // The expression in a string.
    it_curr_symb = expr.begin(); // Iterator to the 1st character in the expression.
    token_list.clear(); // Clear the list of tokens.

    // Default result.
    ParserResult result( ParserResult::PARSER_OK );

    skip_ws();
    if ( end_input() )
    {
        return ParserResult( ParserResult::UNEXPECTED_END_OF_EXPRESSION,
                std::distance( expr.begin(), it_curr_symb ) );
    }

    // Tentar validar a expressao...
    result = expression();

    // TODO: incluir verificação de "lixo" no final da string.
    // Gerar erro: ParserResult::EXTRANEOUS_SYMBOL
    if ( result.type == ParserResult::PARSER_OK )
    {
        skip_ws();
        std::cout << ">>> O que sobrou foi: \"";
        std::copy( it_curr_symb, expr.end(), 
                std::ostream_iterator<char>( std::cout, "" ) );
        std::cout << "\"\n";
        if ( not end_input() )
        {
            std::cout << "Erro aqui\n";
            return ParserResult( ParserResult::EXTRANEOUS_SYMBOL, 
                    std::distance( expr.begin(), it_curr_symb ) );
        }
    }

    return result;
}


/// Return the list of tokens, which is the by-product created during the syntax analysis.
std::vector< Token >
Parser::get_tokens( void ) const
{
    return token_list;
}



//==========================[ End of parse.cpp ]==========================//
