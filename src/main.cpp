
#include "../include/bares.h"
#include <fstream> //ifstream
using value_t = short int;

int main( int argc, char * argv[] )
{   
    //If have a archive verify if is valid.
    if ( argc > 1 )
    {
        std::string archive_name = argv[1];
        
        //Instanciate a ifstream object and try to open an archive.
        std::ifstream expressions_file;
        expressions_file.open( archive_name );

        if ( expressions_file.is_open() )
        {
            //Look in the all archive.
            while ( ! expressions_file.eof() )
            {
                //Analysis line by line of an archive of expressions.
                std::string expression_at_line;
                std::getline ( expressions_file, expression_at_line );
                
                Parser obj;

                //! Transform the expression string in a vector of Token verifind 
                //      your validate.
                Parser::ParserResult result = obj.parse( expression_at_line );

                if ( result.type != Parser::ParserResult::PARSER_OK )
                {
                        // Imprime mensagem de erro.
                        obj.print_msg( result, expression_at_line );
                }
                else
                {
                    //Get the tokens of an actual parsed expression.
                    std::vector<Token> expression_tokenized = obj.get_tokens();


                    //Transform the tokens by infix form to postfix form.
                    std::vector<Token> postfix_tokenized = infix_to_postfix( expression_tokenized );


                    //Calculate the result of expression with bases in postfix form.
                    auto resultado_expr = evaluate_postfix( postfix_tokenized );
                    
                    /*Verify if the result is out of limit at value_t type,
                     *and apresents a error message : "Numeric overflow error!".
                     *Otherwise shows the result of expression.
                     */
                    if ( (  resultado_expr < std::numeric_limits< value_t >::min() )
                        or ( resultado_expr > std::numeric_limits< value_t >::max() ) )
                    {
                        std::cout << "Numeric overflow error!\n";
                    }
                    else 
                    {   
                        std::cout << resultado_expr << std::endl;
                    }
                }

            }

            expressions_file.close();
        }
        //If have as enter an invalid or inexistent archive.  
        else 
        {
            std::cout << " >>>> Invalid or inexistent archive <<<<\n";
            std::cout << " >>>> Try run again with a valid archive name <<<<\n";
        } 
    }
    //If don't have as enter an archive.    
    else 
    {
        std::cout << " >>>> No one archive entered <<<<\n";
        std::cout << " >>>> Try run again with a valid archive name <<<<\n";
    } 

    return EXIT_SUCCESS;
}
