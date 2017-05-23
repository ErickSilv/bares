
#include "../include/bares.h"

#include <fstream> //ifstream


int main( int argc, char * argv[] )
{
    if ( argc > 1 )
    {
        std::string archive_name = argv[1];
        //std::vector < std::string > expression_list;

        std::ifstream expressions_file;
        expressions_file.open( archive_name );

        if ( expressions_file.is_open() )
        {
            while ( ! expressions_file.eof() )
            {
                std::string expression_at_line;
                std::getline ( expressions_file, expression_at_line );
                
                Parser obj;

                Parser::ParserResult result = obj.parse( expression_at_line );

                if ( result.type != Parser::ParserResult::PARSER_OK )
                {
                    // Imprime mensagem de erro.
                    obj.print_msg( result, expression_at_line );
                }
                else
                {
                    

                    std::vector<Token> expression_tokenized = obj.get_tokens();
                    std::vector<Token> postfix_tokenized = infix_to_postfix( expression_tokenized );

                    auto resultado_expr = evaluate_postfix( postfix_tokenized );
                    
                    std::cout << " >>>> Valid Expression! <<<< \n";

                    std::cout << " >>>> Resultado de "  << " " << expression_at_line << " = " << resultado_expr << " <<<< \n" << std::endl;

                }



                //expression_list.push_back( expression_at_line );
            }

            expressions_file.close();
            std::cout << " >>>> END OF EXECUTION <<<<\n";
        } 
    }    
    else 
    {
        std::cout << " >>>> Invalid or inexistent archive <<<<\n";
        std::cout << " >>>> Try run again with a valid archive name <<<<\n";
    } 


       /* // Se deu pau, imprimir a mensagem adequada.
        
        */

    return EXIT_SUCCESS;
}
