#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "macro.h"
// #include "table.h"
#include "syntax_tree.h"

// extern int lines,words,chars;
// extern FILE* yyin;
// extern int yylex();
extern void Program();
extern void yyrestart();
extern int yyparse();
extern int yylineno;
extern int yydebug;
extern Syntax_tree *root;
bool no_error = true;

int main(int argc, char **argv)
{

    for (int i = 1; i < argc; i++)
    {
        FILE *f = fopen(argv[i], "r");
        if (!f)
        {
            perror(argv[i]);
            return 1;
        }

        yyrestart(f);
        // yyrestart(),reinitialize the lines of a file
        yylineno = 1;
        yyparse();
        // printf("4");
#ifdef DEBUG

        print_tree(root);
#else
        if (no_error)
        {
            // print_tree(root);
            Program(root);
        }
#endif

        fclose(f);
    }
    return 0;
}
