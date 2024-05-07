#include "table.h"

bool same_type(Type type_1, Type type_2)
{
    if (type_1 == NULL && type_2 == NULL)
        return true;
    else if (type_1 != NULL && type_2 != NULL)
    {
        type_1 = type_1->kind == FUNCTION ? type_1->u.function.rt_type : type_1;
        type_2 = type_2->kind == FUNCTION ? type_2->u.function.rt_type : type_2;
        if (type_1->kind == type_2->kind)
        {
            enum KIND kind = type_1->kind;
            if (kind == BASIC)
            {

                return (type_1->u.basic == type_2->u.basic);
            }
            else if (kind == ARRAY)
            {
                return (same_type(type_1->u.array.elem, type_2->u.array.elem));
            }
            else if (kind == STRUCTURE)
            {
                bool same = true;
                FieldList a = type_1->u.structure;
                FieldList b = type_2->u.structure;
                return (strcmp(a->name, b->name) == 0);
            }
            else
                return false;
        }
        else
            return false;
    }
    return false;
}
bool same_field_type(FieldList item_1, FieldList item_2)
{
    return (item_1 == NULL && item_2 == NULL) ||
           ((item_1 != NULL && item_2 != NULL) &&
            (same_type(item_1->type, item_2->type)) &&
            (same_field_type(item_1->tail, item_2->tail)));
}

unsigned int hash(char *name)
{
    unsigned int val = 0, i;
    for (; *name; ++name)
    {
        val = (val << 2) + *name;
        if (i = val & ~0x3fff)
            val = (val ^ (i >> 12)) & 0x3fff;
    }
    return val;
}
void inithash()
{
    for (int i = 0; i < 0x3fff; ++i)
    {
        table[i] = NULL;
    }
}
FieldList try_to_find_with_no_error(char *name)
{
    if (name == NULL)
    {
        printf("nullptr\n");
        return NULL;
    }
    unsigned int index = hash(name);
    Barrel barrel = table[index];
    if (barrel == NULL)
        return NULL;
    else
    {
        FieldList temp = NULL;
        for (; barrel != NULL;)
        {
            if (strcmp(barrel->item->name, name) == 0 && barrel->item->type->kind != ERROR_TYPE)
            {
                temp = barrel->item;
            }
            barrel = barrel->next;
        }
        return temp;
    }
}
FieldList try_to_find_with_error(char *name)
{
    return find(name);
}

// 在表项里查找以name为名字的符号，如果能找到则返回FieldList,否则为NULL
FieldList find(char *name)
{
    if (name == NULL)
    {
        printf("nullptr\n");
        return NULL;
    }
    unsigned int index = hash(name);
    Barrel barrel = table[index];
    if (barrel == NULL)
        return NULL;
    else
    {
        FieldList temp = NULL;
        for (; barrel != NULL;)
        {
            if (strcmp(barrel->item->name, name) == 0)
            {
                temp = barrel->item;
                if (temp->type->kind == ERROR_TYPE)
                    break;
            }
            barrel = barrel->next;
        }
        return temp;
    }
}

// 在table插入item，不会检查是否有重名
void insert(FieldList item)
{
    if (item == NULL)
    {
        printf("nullptr fieldlist\n");
        return;
    }
    char *name = item->name;
    unsigned int index = hash(name);
    if (table[index] == NULL)
    {
        Barrel barrel = (Barrel)malloc(sizeof(struct Barrel_));
        barrel->item = item;
        barrel->next = NULL;
        table[index] = barrel;
        // printf("insert:%s,%d\n", item->name, item->type->kind);

        // if (item->type->kind == STRUCTURE)
        // {
        //     printf("CompSt:");
        //     for (FieldList i = item->type->u.structure; i != NULL; i = i->tail)
        //     {
        //         printf("%s\t", i->name);
        //     }
        //     printf("\n");
        // }
    }
    else
    {
        Barrel barrel = table[index];
        for (; barrel->next != NULL;)
            barrel = barrel->next;
        Barrel new_item = (Barrel)malloc(sizeof(struct Barrel_));
        new_item->item = item;
        new_item->next = NULL;
        barrel->next = new_item;
        // type("insert:%s,%d\n", item->name, item->type->kind);
    }
};
// Program : ExtDefList
DECLARE_FUNCTION_2(Program, ExtDefList)
//     ;
// ExtDefList : ExtDef ExtDefList
DECLARE_FUNCTION_3(ExtDefList, ExtDef, ExtDefList)
//     | /* empty */
DECLARE_FUNCTION_1(ExtDefList)
//     ;
// ExtDef : Specifier ExtDecList SEMI
DECLARE_FUNCTION_4(ExtDef, Specifier, ExtDecList, SEMI)
//     | Specifier SEMI
DECLARE_FUNCTION_3(ExtDef, Specifier, SEMI)
//     | Specifier FunDec CompSt
DECLARE_FUNCTION_4(ExtDef, Specifier, FunDec, CompSt)
//     ;
// ExtDecList : VarDec
DECLARE_FUNCTION_2(ExtDecList, VarDec)
//     | VarDec COMMA ExtDecList
DECLARE_FUNCTION_4(ExtDecList, VarDec, COMMA, ExtDecList)
//     ;

// /*Specifiers*/
// Specifier : TYPE
DECLARE_FUNCTION_2(Specifier, TYPE)
//     | StructSpecifier
DECLARE_FUNCTION_2(Specifier, StructSpecifier)
//     ;

// StructSpecifier : STRUCT OptTag LC DefList RC
DECLARE_FUNCTION_6(StructSpecifier, STRUCT, OptTag, LC, DefList, RC)
//     | STRUCT Tag
DECLARE_FUNCTION_3(StructSpecifier, STRUCT, Tag)
//     ;
// OptTag : ID
DECLARE_FUNCTION_2(OptTag, ID)
//     | /* empty */
DECLARE_FUNCTION_1(OptTag)
//     ;
// Tag : ID
DECLARE_FUNCTION_2(Tag, ID)
//     ;
/* Declarators */
// VarDec : ID
DECLARE_FUNCTION_2(VarDec, ID)
//     | FunDec
DECLARE_FUNCTION_2(VarDec, FunDec)
//     | VarDec LB INT RB
DECLARE_FUNCTION_5(VarDec, VarDec, LB, INT, RB)
//     ;
// FunDec : ID LP VarList RP
DECLARE_FUNCTION_5(FunDec, ID, LP, VarList, RP)
//     | ID LP RP
DECLARE_FUNCTION_4(FunDec, ID, LP, RP)
//     ;
// VarList : ParamDec COMMA VarList
DECLARE_FUNCTION_4(VarList, ParamDec, COMMA, VarList)
//     | ParamDec
DECLARE_FUNCTION_2(VarList, ParamDec)
//     ;
// ParamDec : Specifier VarDec
DECLARE_FUNCTION_3(ParamDec, Specifier, VarDec)
//     ;
/* Statements */
// CompSt : LC DefList StmtList RC
DECLARE_FUNCTION_5(CompSt, LC, DefList, StmtList, RC);
// ;
// StmtList : Stmt StmtList
DECLARE_FUNCTION_3(StmtList, Stmt, StmtList)
// | /* empty */
DECLARE_FUNCTION_1(StmtList)
// ;
// Stmt : Exp SEMI
DECLARE_FUNCTION_3(Stmt, Exp, SEMI)
//     | CompSt
DECLARE_FUNCTION_2(Stmt, CompSt)
//     | RETURN Exp SEMI
DECLARE_FUNCTION_4(Stmt, RETURN, Exp, SEMI)
//     | IF LP Exp RP Stmt
DECLARE_FUNCTION_6(Stmt, IF, LP, Exp, RP, Stmt)
//     | IF LP Exp RP Stmt ELSE Stmt
DECLARE_FUNCTION_8(Stmt, IF, LP, Exp, RP, Stmt, ELSE, Stmt)
//     | WHILE LP Exp RP Stmt
DECLARE_FUNCTION_6(Stmt, WHILE, LP, Exp, RP, Stmt)
//     ;
// /* Local Definations */
// DefList : Def DefList
DECLARE_FUNCTION_3(DefList, Def, DefList)
//     | /* empty */
DECLARE_FUNCTION_1(DefList)
//     ;
// Def : Specifier DecList SEMI
DECLARE_FUNCTION_4(Def, Specifier, DecList, SEMI)
//     ;
// DecList : Dec
DECLARE_FUNCTION_2(DecList, Dec)
//     | Dec COMMA DecList
DECLARE_FUNCTION_4(DecList, Dec, COMMA, DecList)
//     ;
// Dec : VarDec
DECLARE_FUNCTION_2(Dec, VarDec)
//     | VarDec ASSIGNOP Exp
DECLARE_FUNCTION_4(Dec, VarDec, ASSIGNOP, Exp)
//     ;
// /* expressions */
// Exp : Exp ASSIGNOP Exp
DECLARE_FUNCTION_4(Exp, Exp, ASSIGNOP, Exp)
//     | Exp AND Exp
DECLARE_FUNCTION_4(Exp, Exp, AND, Exp)
//     | Exp OR Exp
DECLARE_FUNCTION_4(Exp, Exp, OR, Exp)
//     | Exp RELOP Exp
DECLARE_FUNCTION_4(Exp, Exp, RELOP, Exp)
//     | Exp PLUS Exp
DECLARE_FUNCTION_4(Exp, Exp, PLUS, Exp)
//     | Exp MINUS Exp
DECLARE_FUNCTION_4(Exp, Exp, MINUS, Exp)
//     | Exp STAR Exp
DECLARE_FUNCTION_4(Exp, Exp, STAR, Exp)
//     | Exp DIV Exp
DECLARE_FUNCTION_4(Exp, Exp, DIV, Exp)
//     | LP Exp RP
DECLARE_FUNCTION_4(Exp, LP, Exp, RP)
//     | MINUS Exp
DECLARE_FUNCTION_3(Exp, MINUS, Exp)
//     | NOT Exp
DECLARE_FUNCTION_3(Exp, NOT, Exp)
//     | ID LP Args RP
DECLARE_FUNCTION_5(Exp, ID, LP, Args, RP)
//     | ID LP RP
DECLARE_FUNCTION_4(Exp, ID, LP, RP)
//     | Exp LB Exp RB
DECLARE_FUNCTION_5(Exp, Exp, LB, Exp, RB)
//     | Exp DOT ID
DECLARE_FUNCTION_4(Exp, Exp, DOT, ID)
//     | ID
DECLARE_FUNCTION_2(Exp, ID)
//     | INT
DECLARE_FUNCTION_2(Exp, INT)
//     | FLOAT
DECLARE_FUNCTION_2(Exp, FLOAT)
//     ;
// Args : Exp COMMA Args
DECLARE_FUNCTION_4(Args, Exp, COMMA, Args)
//    | Exp
DECLARE_FUNCTION_2(Args, Exp)
//    ;

void Program(Syntax_tree *root)
{
    if (root == NULL)
        return;
    // // Program : ExtDefList
    else if (is_Program_ExtDefList(root))
    {
        Syntax_tree *_extdeflist = root->child;
        ExtDefList(_extdeflist);
    }
    for (unsigned i = 0; i < 0x3fff; i++)
    {
        Barrel barrel = table[i];
        for (; barrel != NULL; barrel = barrel->next)
        {
            if (barrel->item->type->kind == FUNCTION && !barrel->item->type->u.function.defined)
            {
                ERROR(18, barrel->item->type->u.function.line)
            }
        }
    }
}
// //     ;
// together
void ExtDefList(Syntax_tree *node)
{
    if (node == NULL)
        return;
    // // ExtDefList : ExtDef ExtDefList
    else if (is_ExtDefList_ExtDef_ExtDefList(node))
    {
        Syntax_tree *_extdef = node->child;
        Syntax_tree *_extdeflist = _extdef->sibling;
        ExtDef(_extdef);
        ExtDefList(_extdeflist);
    }
    //     | /* empty */
    else if (is_ExtDefList(node))
    {
    }
}

// 全局变量的定义
void ExtDef(Syntax_tree *node)
{
    if (node == NULL)
        return;

    // ExtDef : Specifier ExtDecList SEMI
    else if (is_ExtDef_Specifier_ExtDecList_SEMI(node))
    {

        Syntax_tree *_specifier = node->child;
        Syntax_tree *_extdeclist = _specifier->sibling;
        Type type = Specifier(_specifier);
        ExtDecList(_extdeclist, type);
    }
    //     | Specifier SEMI
    else if (is_ExtDef_Specifier_SEMI(node))
    {
        Syntax_tree *_specifier = node->child;
        Syntax_tree *_semi = _specifier->sibling;
        Type type = Specifier(_specifier);
    }
    // | Specifier FunDec CompSt
    else if (is_ExtDef_Specifier_FunDec_CompSt(node))
    {
        Syntax_tree *_specifier = node->child;
        Syntax_tree *_fundec = _specifier->sibling;
        Syntax_tree *_compst = _fundec->sibling;
        Type rt_type = Specifier(_specifier);
        FieldList function = FunDec(_fundec, rt_type);
        if (function == NULL)
            ;
        else
        {
            function->type->u.function.defined = true;
        }
        CompSt(_compst, rt_type);
        // type("8\n");
    }
}

// Li
void ExtDecList(Syntax_tree *node, Type type)
{
    if (node == NULL)
        return;
    // ExtDecList : VarDec
    else if (is_ExtDecList_VarDec(node))
    {
        FieldList field = VarDec(node->child, type, ERROR_TYPE);
        // meixiewan
    }
    // | VarDec COMMA ExtDecList
    else if (is_ExtDecList_VarDec_COMMA_ExtDecList(node))
    {
        Syntax_tree *child = node->child;
        FieldList field = VarDec(child, type, ERROR_TYPE);
        ExtDecList(child->sibling->sibling, type);
        // meixiewan
    }
}
// //     ;

// shy
// // /*Specifiers*/
Type Specifier(Syntax_tree *node)
{
    if (node == NULL)
        return NULL;
    // // Specifier : TYPE
    else if (is_Specifier_TYPE(node))
    {

        Syntax_tree *_type = node->child;
        Type type = (Type)malloc(sizeof(struct Type_));
        type->kind = BASIC;
        if (strcmp(_type->val.type_string, "int") == 0)
            type->u.basic = TYPE_INT;
        else if (strcmp(_type->val.type_string, "float") == 0)
            type->u.basic = TYPE_FLOAT;
        // type("%d\n",type->u.basic);
        return type;
    }
    // StructSpecifier
    else if (is_Specifier_StructSpecifier(node))
    {
        Syntax_tree *_structspecifier = node->child;
        return StructSpecifier(_structspecifier);
    }
}
//   ;

// shy
Type StructSpecifier(Syntax_tree *node)
{

    if (node == NULL)
        return NULL;
    // StructSpecifier :TRUCT OptTag LC DefList RC
    else if (is_StructSpecifier_STRUCT_OptTag_LC_DefList_RC(node))
    {
        Syntax_tree *_struct = node->child;
        Syntax_tree *_opttag = _struct->sibling;
        Syntax_tree *_lc = _opttag->sibling;
        Syntax_tree *_deflist = _lc->sibling;
        Syntax_tree *_rc = _deflist->sibling;
        /*
            type _____ kind
                   |
                   |__ u __ structure _____ name
                                        |
                                        |__ type _____ kind
                                        |         |
                                        |         |__ u __ structure
                                        |
                                        |__ tail

        */
        Type type = (Type)malloc(sizeof(struct Type_));
        // kind
        type->kind = STRUCTURE;
        // structure
        FieldList structure = (FieldList)malloc(sizeof(struct FieldList_));
        // name
        char *name = (char *)malloc(MAX_NAME_LENGTH * sizeof(char));
        strcpy(name, OptTag(_opttag));
        FieldList found = try_to_find_with_no_error(name);
        if (found != NULL)
        {
            ERROR(16, _opttag->line);
        }
        else
        {
            structure->name = name;

            // type
            Type _type = (Type)malloc(sizeof(struct Type_));
            _type->kind = STRUCTURE;
            _type->u.structure = DefList(_deflist, STRUCTURE);
            // type("666666:%s",_type->u.structure->name);
            structure->type = _type;
            // tail
            structure->tail = NULL;

            type->u.structure = structure;
            // register

            insert(structure);
        }
        return type;
    }
    //                  | STRUCT Tag
    else if (is_StructSpecifier_STRUCT_Tag(node))
    {
        Type type = (Type)malloc(sizeof(struct Type_));

        // structure
        Syntax_tree *_struct = node->child;
        Syntax_tree *_Tag = _struct->sibling;
        FieldList structure;
        char *name = Tag(_Tag);
        // need to look up

        structure = try_to_find_with_no_error(name);
        if (structure == NULL)
        {
            // error 01
            //  return NULL;
            ERROR(17, _Tag->line);
        }
        else
        { // kind
            type->kind = STRUCTURE;
            type->u.structure = structure;
        }
        return type;
    }
}

// shy
char *OptTag(Syntax_tree *node)
{

    // // OptTag : ID
    if (is_OptTag_ID(node))
    {
        Syntax_tree *_id = node->child;
        return _id->val.type_string;
    }
    //     | /* empty */
    else if (is_OptTag(node))
    {
        return NULL;
    }
}

// shy
char *Tag(Syntax_tree *node)
{
    // Tag : ID
    if (node == NULL)
        return NULL;
    else if (is_Tag_ID(node))
    {
        Syntax_tree *_id = node->child;
        return _id->val.type_string;
    }
}
// /* Declarators */
// shy void* need to be changed
// li
// 表示对一个变量的定义
// complete register
FieldList VarDec(Syntax_tree *node, Type type, enum KIND head_field_kind)
{
    if (node == NULL)
        return NULL;
    // VarDec : ID
    else if (is_VarDec_ID(node))
    {
        Syntax_tree *_id = node->child;
        FieldList field = (FieldList)malloc(sizeof(struct FieldList_));
        GET_NAME
        // 检查变量重名
        FieldList found = try_to_find_with_no_error(name);

        if (found != NULL)
        {

            if (head_field_kind == STRUCTURE)
            {
                ERROR(15, _id->line);
                ERROR_UPLOAD(name);
                return found;
            }
            else if (head_field_kind != FUNCTION)
            {
                //
                ERROR(3, _id->line);
                ERROR_UPLOAD(name);
                return found;
            }
        }
        else
        {
            // type("666");
            field->name = name;
            field->type = type;
            field->tail = NULL;
            // register
            insert(field);
            // type("%d\n",field->type->u.basic);
            return field;
        }
    }
    // | FunDec
    else if (is_VarDec_FunDec(node))
    {
        Syntax_tree *_fundec = node->child;
        return FunDec(_fundec, type);
    }
    //     | VarDec LB INT RB
    else if (is_VarDec_VarDec_LB_INT_RB(node))
    {
        Syntax_tree *_vardec = node->child;
        Syntax_tree *_lb = _vardec->sibling;
        Syntax_tree *_int = _lb->sibling;
        Syntax_tree *_rb = _int->sibling;
        FieldList field = (FieldList)malloc(sizeof(struct FieldList_));
        Syntax_tree *temp = node;
        Type temp_type = type;

        while (is_VarDec_VarDec_LB_INT_RB(temp))
        {
            Type t = (Type)malloc(sizeof(struct Type_));
            t->kind = ARRAY;
            t->u.array.size = temp->child->sibling->sibling->val.type_int;
            t->u.array.elem = temp_type;
            temp_type = t;
            temp = temp->child;
            field->type = t;
        }
        if (is_VarDec_ID(temp))
        {
            Syntax_tree *_id = temp->child;
            GET_NAME
            FieldList found = try_to_find_with_no_error(name);
            // 检查变量重名
            if (found != NULL)
            {
                ERROR(3, _id->line);
                ERROR_UPLOAD(name)
                return NULL;
            }
            else
            {
                field->name = name;
                field->tail = NULL;
                insert(field);
                return field;
            }
        }
    }
}

// 表示对一个函数头的定义
//  shy void* need to be changed
// complete register
FieldList FunDec(Syntax_tree *node, Type rt_type /*返回值*/)
{
    if (node == NULL)
        return NULL;
    // FunDec : ID LP VarList RP
    else if (is_FunDec_ID_LP_VarList_RP(node))
    {
        Syntax_tree *_id = node->child;
        Syntax_tree *_lp = _id->sibling;
        Syntax_tree *_varlist = _lp->sibling;
        Syntax_tree *_rp = _lp->sibling;
        FieldList item = (FieldList)malloc(sizeof(struct FieldList_));
        // name
        GET_NAME
        // 判断是否会有重名
        FieldList found = try_to_find_with_no_error(name);
        // 判断是否是声明函数

        if (found != NULL)
        {
            if (found->type->u.function.defined == false)
            {
                FieldList args = VarList(_varlist);
                FieldList function_args = found->type->u.function.args;

                // 函数的参数类型是否匹配
                if (!same_field_type(args, function_args))
                {
                    ERROR(19, _varlist->line);
                    return NULL;
                }
                else
                {
                    return found;
                }
            }
            else
            {
                // error
                ERROR(4, _id->line);
                return NULL;
            }
        }
        //
        item->name = name;
        // type
        Type type = (Type)malloc(sizeof(struct Type_));
        type->kind = FUNCTION;
        type->u.function.rt_type = rt_type;
        type->u.function.line = _id->line;
        type->u.function.args = VarList(_varlist);

        item->type = type;
        // register
        insert(item); //
        return item;
    }
    //     | ID LP RP
    else if (is_FunDec_ID_LP_RP(node))
    {
        Syntax_tree *_id = node->child;
        Syntax_tree *_lp = _id->sibling;
        Syntax_tree *_rp = _lp->sibling;
        FieldList item = (FieldList)malloc(sizeof(struct FieldList_));
        // name
        GET_NAME
        // GET_NAME
        //  判断是否会有重名
        FieldList found = try_to_find_with_no_error(name);
        // 判断是否是声明函数
        if (found != NULL)
        {
            if (found->type->u.function.defined == false)
            {
                FieldList args = NULL;
                FieldList function_args = found->type->u.function.args;

                // 函数的参数类型是否匹配
                if (!same_field_type(args, function_args))
                {
                    ERROR(19, _id->line);
                    return NULL;
                }
                else
                {
                    return found;
                }
            }
            else
            {
                // error
                ERROR(4, _id->line);
                return NULL;
            }
        }
        //
        item->name = name;
        // type
        Type type = (Type)malloc(sizeof(struct Type_));
        type->kind = FUNCTION;
        type->u.function.rt_type = rt_type;
        type->u.function.args = NULL;
        type->u.function.line = _id->line;

        item->type = type;
        // register
        insert(item);
        //
        return item;
    }
}

//     ;
FieldList VarList(Syntax_tree *node)
{
    if (node == NULL)
        return NULL;

    // VarList : ParamDec COMMA VarList
    if (is_VarList_ParamDec_COMMA_VarList(node))
    {
        Syntax_tree *_paramdec = node->child;
        Syntax_tree *_comma = _paramdec->sibling;
        Syntax_tree *_varlist = _comma->sibling;
        FieldList field_list = ParamDec(_paramdec);
        field_list->tail = VarList(_varlist);
        return field_list;
    }
    //     | ParamDec
    if (is_VarList_ParamDec(node))
    {
        Syntax_tree *_paramdec = node->child;
        return ParamDec(_paramdec);
    }
}
//     ;
FieldList ParamDec(Syntax_tree *node)
{
    if (node == NULL)
        return NULL;
    // ParamDec : Specifier VarDec
    else if (is_ParamDec_Specifier_VarDec(node))
    {
        Syntax_tree *_specifier = node->child;
        Syntax_tree *_vardec = _specifier->sibling;
        Type type = Specifier(_specifier);
        FieldList field = VarDec(_vardec, type, FUNCTION);
        return field;
    }
}

// /* Statements */
void CompSt(Syntax_tree *node, Type rt_type)
{
    if (node == NULL)
        return;
    // // CompSt : LC DefList StmtList RC
    else if (is_CompSt_LC_DefList_StmtList_RC(node))
    {
        Syntax_tree *_lc = node->child;
        Syntax_tree *_defList = _lc->sibling;
        Syntax_tree *_stmtList = _defList->sibling;
        DefList(_defList, ERROR_TYPE);
        StmtList(_stmtList, rt_type);
        // printf("7\n");
    }
    // // ;
}
void StmtList(Syntax_tree *node, Type type)
{
    if (node == NULL)
        return;
    // // StmtList : Stmt StmtList
    else if (is_StmtList_Stmt_StmtList(node))
    {
        Syntax_tree *_stmt = node->child;
        Syntax_tree *_stmtList = _stmt->sibling;
        Stmt(_stmt, type);
        StmtList(_stmtList, type);
    }
    // // | /* empty */
    else if (is_StmtList(node))
    {
        return;
    }
    // // ;
}
void Stmt(Syntax_tree *node, Type rt_type)
{
    if (node == NULL)
        return;
    // Stmt : Exp SEMI
    else if (is_Stmt_Exp_SEMI(node))
    {

        Syntax_tree *_exp = node->child;
        Syntax_tree *_semi = _exp->sibling;

        Exp(_exp);
    }
    // //     | CompSt
    else if (is_Stmt_CompSt(node))
    {
        Syntax_tree *_compst = node->child;
        CompSt(_compst, rt_type);
    }
    // //     | RETURN Exp SEMI
    else if (is_Stmt_RETURN_Exp_SEMI(node))
    {
        // printf("4");
        Syntax_tree *_return = node->child;
        Syntax_tree *_exp = _return->sibling;
        Syntax_tree *semi = _exp->sibling;
        FieldList field = Exp(_exp);
        // printf("5\n");
        // error
        if (field == NULL)
        {
            // printf("6\n");
            return;
        }
        //
        // printf("???");
        if (!same_type(rt_type, field->type))
        {
            ERROR(8, _return->line);
        }
    }
    //     | IF LP Exp RP Stmt
    else if (is_Stmt_IF_LP_Exp_RP_Stmt(node))
    {
        Syntax_tree *_if = node->child;
        Syntax_tree *_lp = _if->sibling;
        Syntax_tree *_exp = _lp->sibling;
        Syntax_tree *_rp = _exp->sibling;
        Syntax_tree *_stmt = _rp->sibling;
        Exp(_exp);
        Stmt(_stmt, rt_type);
    }
    //     | IF LP Exp RP Stmt ELSE Stmt
    else if (is_Stmt_IF_LP_Exp_RP_Stmt_ELSE_Stmt(node))
    {
        Syntax_tree *_if = node->child;
        Syntax_tree *_lp = _if->sibling;
        Syntax_tree *_exp = _lp->sibling;
        Syntax_tree *_rp = _exp->sibling;
        Syntax_tree *_stmt_1 = _rp->sibling;
        Syntax_tree *_else = _stmt_1->sibling;
        Syntax_tree *_stmt_2 = _else->sibling;
        Exp(_exp);
        Stmt(_stmt_1, rt_type);
        Stmt(_stmt_2, rt_type);
    }
    //     | WHILE LP Exp RP Stmt
    else if (is_Stmt_WHILE_LP_Exp_RP_Stmt(node))
    {
        Syntax_tree *_while = node->child;
        Syntax_tree *_lp = _while->sibling;
        Syntax_tree *_exp = _lp->sibling;
        Syntax_tree *_rp = _exp->sibling;
        Syntax_tree *_stmt = _rp->sibling;
        Exp(_exp);
        Stmt(_stmt, rt_type);
    }
}

// // /* Local Definations */
FieldList DefList(Syntax_tree *node, enum KIND head_field_kind)
{
    if (node == NULL)
        return NULL;
    // DefList : Def DefList
    else if (is_DefList_Def_DefList(node))
    {
        Syntax_tree *_def = node->child;
        Syntax_tree *_deflist = _def->sibling;
        FieldList item_list = Def(_def, head_field_kind);
        FieldList tail = item_list;
        for (; tail->tail != NULL;)
            tail = tail->tail;
        tail->tail = DefList(_deflist, head_field_kind);
        return tail;
    }
    //| /* empty */
    else if (is_DefList(node))
    {
        return NULL;
    }
}

FieldList Def(Syntax_tree *node, enum KIND head_field_kind)
{
    if (node == NULL)
        return NULL;
    // Def : Specifier DecList SEMI
    else if (is_Def_Specifier_DecList_SEMI(node))
    {
        Syntax_tree *_specifier = node->child;
        Syntax_tree *_declist = _specifier->sibling;
        Syntax_tree *_semi = _declist->sibling;
        Type type = Specifier(_specifier);
        return DecList(_declist, type, head_field_kind);
    }
}

FieldList DecList(Syntax_tree *node, Type type, enum KIND head_field_kind)
{
    if (node == NULL)
        return NULL;
    // DecList : Dec
    else if (is_DecList_Dec(node))
    {
        Syntax_tree *_dec = node->child;
        return Dec(_dec, type, head_field_kind);
    }
    //     | Dec COMMA DecList
    else if (is_DecList_Dec_COMMA_DecList(node))
    {
        Syntax_tree *_dec = node->child;
        Syntax_tree *_comma = _dec->sibling;
        Syntax_tree *_declist = _comma->sibling;
        FieldList head = Dec(_dec, type, head_field_kind);
        head->tail = DecList(_declist, type, head_field_kind);
        // for(FieldList i=head;i!=NULL;i=i->tail){
        //     printf("%s???",i->name);
        // }
        // printf("\n");
        return head;
    }
}
// single var dec or assign
FieldList Dec(Syntax_tree *node, Type type, enum KIND head_field_kind)
{
    if (node == NULL)
        return NULL;
    // // Dec : VarDec
    else if (is_Dec_VarDec(node))
    {
        Syntax_tree *_vardec = node->child;
        return VarDec(_vardec, type, head_field_kind);
    }
    // //     | VarDec ASSIGNOP Exp
    else if (is_Dec_VarDec_ASSIGNOP_Exp(node))
    {
        Syntax_tree *_vardec = node->child;
        Syntax_tree *_assignop = _vardec->sibling;
        Syntax_tree *_exp = _assignop->sibling;
        if (head_field_kind == STRUCTURE)
        {
            ERROR(15, _vardec->line);
        }
        FieldList item_1 = VarDec(_vardec, type, head_field_kind);
        FieldList item_2 = Exp(_exp);
        if (same_type(item_1->type, item_2->type))
        {
            return item_1;
        }
        else
        {
            ERROR(5, _assignop->line)
            return NULL;
        }
    }
}

// // /* expressions */
// shy
FieldList Exp(Syntax_tree *node)
{

    if (node == NULL)
        return NULL;
    // // Exp : Exp ASSIGNOP Exp
    else if (is_Exp_Exp_ASSIGNOP_Exp(node))
    {
        Syntax_tree *_exp_1 = node->child;
        Syntax_tree *_assignop = _exp_1->sibling;
        Syntax_tree *_exp_2 = _assignop->sibling;
        FieldList item_1 = Exp(_exp_1);
        FieldList item_2 = Exp(_exp_2);
        if (item_1 == NULL || item_2 == NULL)
            return NULL;
        else
        {

            if (!strcmp(item_1->name, "int") || !strcmp(item_1->name, "float"))
            {
                ERROR(6, _assignop->line)
                return NULL;
            }
            else
            {
                    // printf("%s\n", item_2->name);
                    // printf("%d\n", item_2->type->kind);
                if (item_1->type->kind != ERROR_TYPE && !same_type(item_1->type, item_2->type))
                {

                    // if (item_1->type->kind == BASIC && item_2->type->kind == BASIC)
                    //     printf("%d,%d\n", item_1->type->u.basic, item_2->type->u.basic);
                    ERROR(5, _assignop->line);
                    return NULL;
                }
                return item_1;
            }
        }
    }
    // //     | Exp AND Exp
    else if (is_Exp_Exp_AND_Exp(node))
    {
        Syntax_tree *_exp_1 = node->child;
        Syntax_tree *_and = _exp_1->sibling;
        Syntax_tree *_exp_2 = _and->sibling;
        FieldList item_1 = Exp(_exp_1);
        FieldList item_2 = Exp(_exp_2);
        if (item_1 == NULL || item_2 == NULL || item_1->type->kind == ERROR_TYPE || item_2->type->kind == ERROR_TYPE)
            return NULL;
        else
        {
            if (same_field_type(item_1, item_2))
            {
                return item_1;
            }
            else if (item_1->type->kind != ERROR_TYPE && item_2->type->kind != ERROR_TYPE)
            {
                ERROR(7, _and->line);
                item_1->type->kind = ERROR_TYPE;
                return item_1;
            }
        }
    }
    // //     | Exp OR Exp
    else if (is_Exp_Exp_OR_Exp(node))
    {
        Syntax_tree *_exp_1 = node->child;
        Syntax_tree *_or = _exp_1->sibling;
        Syntax_tree *_exp_2 = _or->sibling;
        FieldList item_1 = Exp(_exp_1);
        FieldList item_2 = Exp(_exp_2);
        if (item_1 == NULL || item_2 == NULL || item_1->type->kind == ERROR_TYPE || item_2->type->kind == ERROR_TYPE)

            return NULL;
        else
        {
            if (same_field_type(item_1, item_2))
            {
                return item_1;
            }
            else if (item_1->type->kind != ERROR_TYPE && item_2->type->kind != ERROR_TYPE)
            {
                ERROR(7, _or->line);
                item_1->type->kind = ERROR_TYPE;
                return item_1;
            }
        }
    }
    // //     | Exp RELOP Exp
    else if (is_Exp_Exp_RELOP_Exp(node))
    {
        Syntax_tree *_exp_1 = node->child;
        Syntax_tree *_relop = _exp_1->sibling;
        Syntax_tree *_exp_2 = _relop->sibling;
        FieldList item_1 = Exp(_exp_1);
        FieldList item_2 = Exp(_exp_2);
        FieldList rt = (FieldList)malloc(sizeof(struct FieldList_));
        Type rt_type = (Type)malloc(sizeof(struct Type_));
        rt_type->kind = BASIC;
        rt_type->u.basic = TYPE_INT;
        if (item_1 == NULL || item_2 == NULL || item_1->type->kind == ERROR_TYPE || item_2->type->kind == ERROR_TYPE)

            return NULL;
        else
        {
            if (same_field_type(item_1, item_2))
                ;
            else if (item_1->type->kind != ERROR_TYPE && item_2->type->kind != ERROR_TYPE)
            {
                ERROR(7, _relop->line);
                rt_type->kind = ERROR_TYPE;
            }
        }
        rt->type = rt_type;
        return rt;
    }
    // //     | Exp PLUS Exp
    else if (is_Exp_Exp_PLUS_Exp(node))
    {
        Syntax_tree *_exp_1 = node->child;
        Syntax_tree *_plus = _exp_1->sibling;
        Syntax_tree *_exp_2 = _plus->sibling;
        FieldList item_1 = Exp(_exp_1);
        FieldList item_2 = Exp(_exp_2);
        if (item_1 == NULL || item_2 == NULL || item_1->type->kind == ERROR_TYPE || item_2->type->kind == ERROR_TYPE)

            return NULL;
        else
        {
            if (same_field_type(item_1, item_2))
            {
                return item_1;
            }
            else if (item_1->type->kind != ERROR_TYPE && item_2->type->kind != ERROR_TYPE)
            {
                ERROR(7, _plus->line);
                item_1->type->kind = ERROR_TYPE;
                return item_1;
            }
        }
    }
    // //     | Exp MINUS Exp
    else if (is_Exp_Exp_MINUS_Exp(node))
    {
        Syntax_tree *_exp_1 = node->child;
        Syntax_tree *_minus = _exp_1->sibling;
        Syntax_tree *_exp_2 = _minus->sibling;
        FieldList item_1 = Exp(_exp_1);
        FieldList item_2 = Exp(_exp_2);
        if (item_1 == NULL || item_2 == NULL)
            return NULL;
        else
        {
            if (same_field_type(item_1, item_2))
            {
                return item_1;
            }
            else if (item_1->type->kind != ERROR_TYPE && item_2->type->kind != ERROR_TYPE)
            {
                ERROR(7, _minus->line);
                item_1->type->kind = ERROR_TYPE;
                return item_1;
            }
        }
    }
    // //     | Exp STAR Exp
    else if (is_Exp_Exp_STAR_Exp(node))
    {
        Syntax_tree *_exp_1 = node->child;
        Syntax_tree *_star = _exp_1->sibling;
        Syntax_tree *_exp_2 = _star->sibling;
        FieldList item_1 = Exp(_exp_1);
        FieldList item_2 = Exp(_exp_2);
        if (item_1 == NULL || item_2 == NULL || item_1->type->kind == ERROR_TYPE || item_2->type->kind == ERROR_TYPE)

            return NULL;
        else
        {
            if (same_field_type(item_1, item_2))
            {
                return item_1;
            }
            else if (item_1->type->kind != ERROR_TYPE && item_2->type->kind != ERROR_TYPE)
            {
                ERROR(7, _star->line);
                item_1->type->kind = ERROR_TYPE;
                return item_1;
            }
        }
    }
    // //     | Exp DIV Exp
    else if (is_Exp_Exp_DIV_Exp(node))
    {
        Syntax_tree *_exp_1 = node->child;
        Syntax_tree *_div = _exp_1->sibling;
        Syntax_tree *_exp_2 = _div->sibling;
        FieldList item_1 = Exp(_exp_1);
        FieldList item_2 = Exp(_exp_2);
        if (item_1 == NULL || item_2 == NULL || item_1->type->kind == ERROR_TYPE || item_2->type->kind == ERROR_TYPE)

            return NULL;
        else
        {
            if (same_field_type(item_1, item_2))
            {
                return item_1;
            }
            else if (item_1->type->kind != ERROR_TYPE && item_2->type->kind != ERROR_TYPE)
            {
                ERROR(7, _div->line);
                item_1->type->kind = ERROR_TYPE;
                return item_1;
            }
        }
    }
    // //     | LP Exp RP
    else if (is_Exp_LP_Exp_RP(node))
    {
        Syntax_tree *_lp = node->child;
        Syntax_tree *_exp = _lp->sibling;
        Syntax_tree *_rp = _exp->sibling;
        return Exp(_exp);
    }
    // //     | MINUS Exp
    else if (is_Exp_MINUS_Exp(node))
    {
        Syntax_tree *_minus = node->child;
        Syntax_tree *_exp = _minus->sibling;
        return Exp(_exp);
    }
    // //     | NOT Exp
    else if (is_Exp_NOT_Exp(node))
    {
        Syntax_tree *_not = node->child;
        Syntax_tree *_exp = _not->sibling;
        return Exp(_exp);
    }
    // //     | ID LP Args RP
    else if (is_Exp_ID_LP_Args_RP(node))
    {
        Syntax_tree *_id = node->child;
        Syntax_tree *_lp = _id->sibling;
        Syntax_tree *_args = _lp->sibling;
        Syntax_tree *_rp = _args->sibling;
        // 检查函数是否定义，函数的参数类型是否匹配
        FieldList function = try_to_find_with_no_error(_id->val.type_string);
        // 检查是否定义

        if (function == NULL)
        {
            ERROR(2, _id->line);
        }
        else
        {
            if (function->type->kind != FUNCTION)
            {
                ERROR(11, _id->line)
            }
            else
            {
                FieldList args = Args(_args);
                FieldList function_args = function->type->u.function.args;

                // 函数的参数类型是否匹配
                if (!same_field_type(args, function_args))
                {
                    ERROR(9, _args->line);
                }
            }
        }
        return function;
    }
    // //     | ID LP RP
    else if (is_Exp_ID_LP_RP(node))
    {
        Syntax_tree *_id = node->child;
        Syntax_tree *_lp = _id->sibling;
        Syntax_tree *_rp = _lp->sibling;
        // 检查函数是否定义，函数的参数类型是否匹配
        FieldList function = try_to_find_with_no_error(_id->val.type_string);

        if (function->type->kind != FUNCTION)
        {
            ERROR(11, _id->line)
        }
        else
        { // 检查是否定义
            if (function == NULL)
            {
                ERROR(2, _id->line);
            }
        }
        return function;
    }
    // //     | Exp LB Exp RB
    else if (is_Exp_Exp_LB_Exp_RB(node))
    {
        Syntax_tree *_exp_1 = node->child;
        Syntax_tree *_lb = _exp_1->sibling;
        Syntax_tree *_exp_2 = _lb->sibling;
        Syntax_tree *_rb = _exp_2->sibling;
        FieldList item_1 = Exp(_exp_1);
        FieldList item_2 = Exp(_exp_2);
        if (item_1 == NULL || item_2 == NULL)
            return NULL;
        else
        {
            if (item_1->type->kind != ARRAY)
            {
                // printf("%s", item_1->name);
                ERROR(10, _rb->line)
            }
            if (item_2->type->kind != BASIC || item_2->type->u.basic != TYPE_INT)
            {
                ERROR(12, _exp_2->line);
                return NULL;
            }
            Syntax_tree *temp = node;
            FieldList temp_item = (FieldList)malloc(sizeof(struct FieldList_));
            temp_item->type = item_1->type;
            while (is_Exp_Exp_LB_Exp_RB(temp))
            {
                temp_item->type = temp_item->type->u.array.elem;
                temp = temp->child;
            }
            return temp_item;
        }
    }
    // //     | Exp DOT ID
    else if (is_Exp_Exp_DOT_ID(node))
    {
        Syntax_tree *_exp = node->child;
        Syntax_tree *_dot = _exp->sibling;
        Syntax_tree *_id = _dot->sibling;
        FieldList item_1 = Exp(_exp);
        FieldList item_2 = try_to_find_with_no_error(_id->val.type_string);
        // printf("shy:%s,%d\n", item_1->name, item_1->type->kind);
        if (item_1 == NULL || item_1->type->kind == ERROR_TYPE)
        {
            return NULL;
        }
        else
        {
            // printf("1");
            if (item_1->type->kind != STRUCTURE)
            {
                ERROR(13, _exp->line);
            }
            else
            {
                if (item_2 != NULL)
                {
                    bool flag = false;
                    FieldList item_3 = try_to_find_with_no_error(item_1->type->u.structure->name);
                    // printf("%d,", item_3->type->kind);
                    // printf("%s\n", item_3->name);
                    for (FieldList varlist = item_3->type->u.structure; varlist != NULL;)
                    {
                        if (strcmp(varlist->name, item_2->name) == 0)
                        {
                            flag = true;
                            break;
                        }
                        varlist = varlist->tail;
                    }

                    if (!flag)
                    {
                        ERROR(14, _id->line);
                        item_1->type->kind = ERROR_TYPE;
                    }
                }
                else
                {
                    ERROR(14, _id->line);
                    item_1->type->kind = ERROR_TYPE;
                }
            }
            // printf("2\n");

            return item_2;
        }
    }
    // //     | ID
    else if (is_Exp_ID(node))
    {
        Syntax_tree *_id = node->child;
        FieldList item = try_to_find_with_error(_id->val.type_string);
        if (item == NULL)
        {
            ERROR(1, _id->line);
            return NULL;
        }
        else
            return item;
    }
    // //     | INT
    else if (is_Exp_INT(node))
    {
        Syntax_tree *_int = node->child;
        FieldList item = (FieldList)malloc(sizeof(struct FieldList_));
        char *name = (char *)malloc(4 * sizeof(char));
        strcpy(name, "int");
        item->name = name;
        Type type = (Type)malloc(sizeof(struct Type_));
        type->kind = BASIC;
        item->type = type;
        item->tail = NULL;
        return item;
    }
    // //     | FLOAT
    else if (is_Exp_FLOAT(node))
    {
        Syntax_tree *_float = node->child;
        FieldList item = (FieldList)malloc(sizeof(struct FieldList_));
        char *name = (char *)malloc(6 * sizeof(char));
        strcpy(name, "float");
        item->name = name;
        Type type = (Type)malloc(sizeof(struct Type_));
        type->kind=BASIC;
        type->u.basic = TYPE_FLOAT;
        item->type = type;
        item->tail = NULL;
        return item;
    }
}

// shy
FieldList Args(Syntax_tree *node)
{
    if (node == NULL)
        return NULL;
    // // Args : Exp COMMA Args
    else if (is_Args_Exp_COMMA_Args(node))
    {
        Syntax_tree *_exp = node->child;
        Syntax_tree *_comma = _exp->sibling;
        Syntax_tree *_args = _comma->sibling;
        FieldList item = Exp(_exp);
        item->tail = Args(_args);
        return item;
    }
    // //    | Exp
    else if (is_Args_Exp(node))
    {
        Syntax_tree *_exp = node->child;
        return Exp(_exp);
    }
}
