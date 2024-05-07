#include "syntax_tree.h"

#ifdef DEBUG
int line_count = 0;
#endif

Syntax_tree *spawn(const char *name, Val *val, int line)
{
    Syntax_tree *node = (Syntax_tree *)malloc(sizeof(Syntax_tree));
    if (valuable(name))
        node->val = *val;
    strcpy(node->name, name);
    node->child = NULL;
    node->sibling = NULL;
    node->line = line;
    return node;
}
void build_tree(Syntax_tree *parent, Syntax_tree **child_list, int num_of_child)
{
    // printf("building!\n");

    parent->child = *(child_list++);

    Syntax_tree *current = parent->child;
    for (int i = 1; i < num_of_child; i++, child_list++)
    {
        if (*child_list == NULL)
        {
            printf("in line %d ,error when i is %d,but num_of_child is %d\n", parent->line, i, num_of_child);
            exit(1);
        }
        current->sibling = *child_list;
        current = current->sibling;
    }
};

void print_tree(Syntax_tree *root)
{
    if (root != NULL)
    {

        printf("%s (%d)\n", root->name, root->line);
#ifdef DEBUG
        line_count++;
#endif
        print_node(root->child, 1);
    }
#ifdef DEBUG
    printf("%d lines in total.\n", line_count);
#endif
}

void print_node(Syntax_tree *self, int depth)
{
    if (!(is_non_terminal(self->name) && self->child == NULL))
    {

        for (int i = 0; i < depth; i++)
            printf("  ");

#ifdef DEBUG
        printf("%d:", depth);

        line_count++;

#endif

        printf("%s", self->name);
        print_val(self->name, self->val, self->line);
        printf("\n");
        if (self->child != NULL)
            print_node(self->child, depth + 1);
    }
    if (self->sibling != NULL)
        print_node(self->sibling, depth);
}
void print_val(const char *name, const Val val, int line)
{
    if ((!strcmp(name, "TYPE") || (!strcmp(name, "ID"))))

        printf(": %s", val.type_string);

    else if (!strcmp(name, "INT"))

        printf(": %d", val.type_int);

    else if (!strcmp(name, "FLOAT"))

        printf(": %f", val.type_float);

    else if (is_non_terminal(name))

        printf(" (%d)", line);
}

bool valuable(const char *name)
{
    return !(strcmp(name, "TYPE") && strcmp(name, "ID") && strcmp(name, "INT") && strcmp(name, "FLOAT"));
}

bool is_non_terminal(const char *name)
{
    return (strcmp(name, "TYPE") &&
            strcmp(name, "ID") &&
            strcmp(name, "INT") &&
            strcmp(name, "FLOAT") &&
            strcmp(name, "SEMI") &&
            strcmp(name, "COMMA") &&
            strcmp(name, "STRUCT") &&
            strcmp(name, "RETURN") &&
            strcmp(name, "IF") &&
            strcmp(name, "ELSE") &&
            strcmp(name, "WHILE") &&
            strcmp(name, "ASSIGNOP") &&
            strcmp(name, "OR") &&
            strcmp(name, "AND") &&
            strcmp(name, "RELOP") &&
            strcmp(name, "PLUS") &&
            strcmp(name, "MINUS") &&
            strcmp(name, "STAR") &&
            strcmp(name, "DIV") &&
            strcmp(name, "NOT") &&
            strcmp(name, "LP") &&
            strcmp(name, "RP") &&
            strcmp(name, "LB") &&
            strcmp(name, "RB") &&
            strcmp(name, "LC") &&
            strcmp(name, "RC") &&
            strcmp(name, "DOT"));
}
