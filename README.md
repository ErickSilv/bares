# Introduction

This programming project implements a simple parser for an EBNF (_Extended Backus-Naur Form_) grammar.

The first objective is to provide a simple example of EBNF grammar and a parser, so that students may understand the concept.
The second objective is to reuse this code to do the parsing stage in the [BARES programming project](http://projetos.imd.ufrn.br/LP1_20162/bares.git).

Information on **EBNF grammar** may be found [here](https://en.wikipedia.org/wiki/Extended_Backus–Naur_Form).

Information on **recursive descendent parsing** may be found [here](https://en.wikipedia.org/wiki/Recursive_descent_parser).

# The Grammar

The gramar we want to parse represents arithmetic expressions with addition and subtraction of integers.

    <expr>            := ("("),<term>,{ ("+"|"-"|"*"|"/"|"%"|"^"),<term>,(")") };
    <term>            := <integer>;
    <integer>         := 0 | ["-"],<natural_number>;
    <natural_number>  := <digit_excl_zero>,{<digit>};
    <digit_excl_zero> := "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9";
    <digit>           := "0"| <digit_excl_zero>;

This grammar _accepts_ expressions like:

* "13 + 39 - 0   + -14 / 2"
* " 19"
* "-11 %     -8"
* " -54"
* "-21 * (23 + 1234)"
* "3 + 3 * 3 % 4"
* "(3^5) % 10"

And _rejects_ expressions like:

* "01 + 3"
* " - 3 + 4"
* "2 + "
* "  "
* "(+2 + 5"
* "2 + 4)"
* "2/0" 
* "1000000000 - 1"
* ""

# Compiling
To compiling this project is necessary access, in terminal, the repository.
After that write, in terminal, the command : 
> make

# Running 
To execute is necessary write the follow command :   
>  ./parser archive_list_of_expression_name.* .

The char '*' refer to a text type of an expression archive. 

# Limitations
This BARES don't accept more than one unary '-'.
Only numbers and final results of expression beetwen the range [−32,768, +32,767] are accepted (The types has been defined in bares.cpp and main.cpp).

# Authorship

Program developed by Erick de Oliveira Silva (<erickoliveira.eos@gmail.com>), graduation student of IT in UFRN.

# Contributors

In Parser and Token codes has some contribuiton of Selan (selan@dimap.ufrn.br) and students from LP1, 2017.1 with the codes
writed in LP1 classes.

&copy; IMD/UFRN 2017.1

