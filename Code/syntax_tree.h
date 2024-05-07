/*
This file is about to build a syntax tree for the compiler.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "macro.h"


typedef union
{
    int type_int;
    float type_float;
    double type_double;
    char type_string[0x41];
} Val;

typedef struct Syntax_tree
{
    char name[16];
    int line;
    bool need_print;
    int type;
    Val val;
    struct Syntax_tree *child;
    struct Syntax_tree *sibling;
} Syntax_tree;

Syntax_tree *spawn(const char *name, Val* val, int line);

void build_tree(Syntax_tree *parent, Syntax_tree **child, int num_of_child);

void print_tree(Syntax_tree *root);

void print_node(Syntax_tree *parent, int depth);

void print_val(const char *name, const Val val, int line);

bool valuable(const char *name);

bool is_non_terminal(const char *name);