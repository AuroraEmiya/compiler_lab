#include<string.h>

#define bool int
#define false 0
#define true 1
#define TYPE_INT 0
#define TYPE_FLOAT 1
#define TYPE_DOUBLE 2
#define TYPE_STRING 3
#define TYPE_UNDEFINED -1
#define MAX_CHILDREN_NUM 7
#define MAX_NAME_LENGTH (0x40+1)

#define CHILDREN_1(child) children[0] = child;
#define CHILDREN_2(child_1, child_2) CHILDREN_1(child_1);children[1] = child_2;
#define CHILDREN_3(child_1, child_2, child_3) CHILDREN_2(child_1, child_2);children[2] = child_3;
#define CHILDREN_4(child_1, child_2, child_3, child_4) CHILDREN_3(child_1, child_2, child_3);children[3] = child_4;
#define CHILDREN_5(child_1, child_2, child_3, child_4, child_5) CHILDREN_4(child_1, child_2, child_3, child_4);children[4] = child_5;
#define CHILDREN_6(child_1, child_2, child_3, child_4, child_5,child_6)CHILDREN_5(child_1, child_2, child_3, child_4,child_5);children[5] = child_6;
#define CHILDREN_7(child_1, child_2, child_3, child_4, child_5,child_6,child_7) CHILDREN_6(child_1, child_2, child_3, child_4,child_5,child_6);children[6] = child_7; 

#define CLEAN children[0] = NULL;children[1] = NULL;children[2] = NULL;children[3] = NULL;children[4] = NULL;children[5] = NULL;children[6] = NULL;

#define SPAWN(dollar, at, name) spawn(name,&(dollar->val),at.first_line) ;

#define SPAWN_1(dollar_0,at_0,name_0) dollar_0 = SPAWN(dollar_0,at_0,name_0);
#define SPAWN_2(dollar_0,at_0,name_0,dollar_1,at_1,name_1) SPAWN_1(dollar_0,at_0,name_0);dollar_1 =SPAWN(dollar_1,at_1,name_1);
#define SPAWN_3(dollar_0,at_0,name_0,dollar_1,at_1,name_1,dollar_2,at_2,name_2) SPAWN_2(dollar_0,at_0,name_0,dollar_1,at_1,name_1);dollar_2=SPAWN(dollar_2,at_2,name_2);
#define SPAWN_4(dollar_0,at_0,name_0,dollar_1,at_1,name_1,dollar_2,at_2,name_2,dollar_3,at_3,name_3) SPAWN_3(dollar_0,at_0,name_0,dollar_1,at_1,name_1,dollar_2,at_2,name_2);dollar_3 =SPAWN(dollar_3,at_3,name_3);
#define SPAWN_5(dollar_0,at_0,name_0,dollar_1,at_1,name_1,dollar_2,at_2,name_2,dollar_3,at_3,name_3,dollar_4,at_4,name_4) SPAWN_4(dollar_0,at_0,name_0,dollar_1,at_1,name_1,dollar_2,at_2,name_2,dollar_3,at_3,name_3);dollar_4 =SPAWN(dollar_4,at_4,name_4);
#define SPAWN_6(dollar_0,at_0,name_0,dollar_1,at_1,name_1,dollar_2,at_2,name_2,dollar_3,at_3,name_3,dollar_4,at_4,name_4,dollar_5,at_5,name_5) SPAWN_5(dollar_0,at_0,name_0,dollar_1,at_1,name_1,dollar_2,at_2,name_2,dollar_3,at_3,name_3,dollar_4,at_4,name_4);dollar_5 =SPAWN(dollar_5,at_5,name_5);
#define SPAWN_7(dollar_0,at_0,name_0,dollar_1,at_1,name_1,dollar_2,at_2,name_2,dollar_3,at_3,name_3,dollar_4,at_4,name_4,dollar_5,at_5,name_5,dollar_6,at_6,name_6) SPAWN_6(dollar_0,at_0,name_0,dollar_1,at_1,name_1,dollar_2,at_2,name_2,dollar_3,at_3,name_3,dollar_4,at_4,name_4,dollar_5,at_5,name_5);dollar_6 =SPAWN(dollar_6,at_6,name_6);
// #define SPAWN_7(dollar_0,at_0,name_0,dollar_1,at_1,name_1,dollar_2,at_2,name_2,dollar_3,at_3,name_3,dollar_4,at_4,name_4,dollar_5,at_5,name_5,dollar_6,at_6,name_6,dollar_7,at_7,name_7) SPAWN_6(dollar_0,at_0,name_0,dollar_1,at_1,name_1,dollar_2,at_2,name_2,dollar_3,at_3,name_3,dollar_4,at_4,name_4,dollar_5,at_5,name_5,dollar_6,at_6,name_6);SPAWN(dollar_7,at_7,name_7);

// #define DEBUG
// #define YYDEBUG 1
#define A 0
#define B 1

#define UPDATE last_error_line=yylloc.last_line


#define DECLARE_FUNCTION_1(name_1)\
    bool is_##name_1(Syntax_tree* node){\
        bool flag = false;\
        if (node !=NULL)\
        {\
          if (node->child==NULL && strcmp(node->name,#name_1)==0){\
            flag = true;\
            }\
        }\
        return flag;\
    }
#define DECLARE_FUNCTION_2(name_1,name_2)\
    bool is_##name_1##_##name_2(Syntax_tree* node){\
        bool flag = false;\
        if(node!=NULL){\
            if (strcmp(node->name,#name_1)==0){\
                if(node->child != NULL){\
                    Syntax_tree* cur_node = node->child;\
                    if(cur_node->sibling == NULL && strcmp(cur_node->name,#name_2)==0){\
                        flag = true;\
                    }\
                }\
            }\
        }\
        return flag;\
    }
#define DECLARE_FUNCTION_3(name_1,name_2,name_3)\
    bool is_##name_1##_##name_2##_##name_3(Syntax_tree* node){\
        bool flag = false;\
        if(node!=NULL){\
            if (strcmp(node->name,#name_1)==0){\
                if(node->child != NULL){\
                    Syntax_tree* cur_node = node->child;\
                    if(cur_node->sibling!=NULL&&strcmp(cur_node->name,#name_2)==0){\
                        cur_node = cur_node->sibling;\
                        if(cur_node->sibling == NULL && strcmp(cur_node->name,#name_3)==0){\
                            flag = true;\
                        }\
                    }\
                }\
            }\
        }\
        return flag;\
    }

#define DECLARE_FUNCTION_4(name_1,name_2,name_3,name_4)\
    bool is_##name_1##_##name_2##_##name_3##_##name_4(Syntax_tree* node){\
        bool flag = false;\
        if(node!=NULL){\
            if (strcmp(node->name,#name_1)==0){\
                if(node->child != NULL){\
                    Syntax_tree* cur_node = node->child;\
                    if(cur_node->sibling != NULL && strcmp(cur_node->name,#name_2)==0){\
                        cur_node = cur_node->sibling;\
                        if(cur_node->sibling != NULL && strcmp(cur_node->name,#name_3)==0){\
                            cur_node = cur_node->sibling;\
                            if(cur_node->sibling == NULL && strcmp(cur_node->name,#name_4)==0){\
                                flag = true;\
                            }\
                        }\
                    }\
                }\
            }\
        }\
        return flag;\
    }

#define DECLARE_FUNCTION_5(name_1,name_2,name_3,name_4,name_5)\
    bool is_##name_1##_##name_2##_##name_3##_##name_4##_##name_5(Syntax_tree* node){\
        bool flag = false;\
        if(node!=NULL){\
            if (strcmp(node->name,#name_1)==0){\
                if(node->child != NULL){\
                    Syntax_tree* cur_node = node->child;\
                    if(cur_node->sibling != NULL && strcmp(cur_node->name,#name_2)==0){\
                        cur_node = cur_node->sibling;\
                        if(cur_node->sibling != NULL && strcmp(cur_node->name,#name_3)==0){\
                            cur_node = cur_node->sibling;\
                            if(cur_node->sibling != NULL && strcmp(cur_node->name,#name_4)==0){\
                                cur_node = cur_node->sibling;\
                                if(cur_node->sibling == NULL && strcmp(cur_node->name,#name_5)==0){\
                                    flag = true;\
                                }\
                            }\
                        }\
                    }\
                }\
            }\
        }\
        return flag;\
    }

#define DECLARE_FUNCTION_6(name_1,name_2,name_3,name_4,name_5,name_6)\
    bool is_##name_1##_##name_2##_##name_3##_##name_4##_##name_5##_##name_6(Syntax_tree* node){\
        bool flag = false;\
        if(node!=NULL){\
            if (strcmp(node->name,#name_1)==0){\
                if(node->child != NULL){\
                    Syntax_tree* cur_node = node->child;\
                    if(cur_node->sibling != NULL && strcmp(cur_node->name,#name_2)==0){\
                        cur_node = cur_node->sibling;\
                        if(cur_node->sibling != NULL && strcmp(cur_node->name,#name_3)==0){\
                            cur_node = cur_node->sibling;\
                            if(cur_node->sibling != NULL && strcmp(cur_node->name,#name_4)==0){\
                                cur_node = cur_node->sibling;\
                                if(cur_node->sibling != NULL && strcmp(cur_node->name,#name_5)==0){\
                                    cur_node = cur_node->sibling;\
                                    if(cur_node->sibling == NULL && strcmp(cur_node->name,#name_6)==0){\
                                        flag = true;\
                                    }\
                                }\
                            }\
                        }\
                    }\
                }\
            }\
        }\
        return flag;\
    }

#define DECLARE_FUNCTION_7(name_1,name_2,name_3,name_4,name_5,name_6,name_7)\
    bool is_##name_1##_##name_2##_##name_3##_##name_4##_##name_5##_##name_6##_##name_7(Syntax_tree* node){\
        bool flag = false;\
        if(node!=NULL){\
            if (strcmp(node->name,#name_1)==0){\
                if(node->child != NULL){\
                    Syntax_tree* cur_node = node->child;\
                    if(cur_node->sibling != NULL && strcmp(cur_node->name,#name_2)==0){\
                        cur_node = cur_node->sibling;\
                        if(cur_node->sibling != NULL && strcmp(cur_node->name,#name_3)==0){\
                            cur_node = cur_node->sibling;\
                            if(cur_node->sibling != NULL && strcmp(cur_node->name,#name_4)==0){\
                                cur_node = cur_node->sibling;\
                                if(cur_node->sibling != NULL && strcmp(cur_node->name,#name_5)==0){\
                                    cur_node = cur_node->sibling;\
                                    if(cur_node->sibling != NULL && strcmp(cur_node->name,#name_6)==0){\
                                        cur_node = cur_node->sibling;\
                                        if(cur_node->sibling == NULL && strcmp(cur_node->name,#name_7)==0){\
                                            flag = true;\
                                        }\
                                    }\
                                }\
                            }\
                        }\
                    }\
                }\
            }\
        }\
        return flag;\
    }

#define DECLARE_FUNCTION_8(name_1,name_2,name_3,name_4,name_5,name_6,name_7,name_8)\
    bool is_##name_1##_##name_2##_##name_3##_##name_4##_##name_5##_##name_6##_##name_7##_##name_8(Syntax_tree* node){\
        bool flag = false;\
        if(node!=NULL){\
            if (strcmp(node->name,#name_1)==0){\
                if(node->child != NULL){\
                    Syntax_tree* cur_node = node->child;\
                    if(cur_node->sibling != NULL && strcmp(cur_node->name,#name_2)==0){\
                        cur_node = cur_node->sibling;\
                        if(cur_node->sibling != NULL && strcmp(cur_node->name,#name_3)==0){\
                            cur_node = cur_node->sibling;\
                            if(cur_node->sibling != NULL && strcmp(cur_node->name,#name_4)==0){\
                                cur_node = cur_node->sibling;\
                                if(cur_node->sibling != NULL && strcmp(cur_node->name,#name_5)==0){\
                                    cur_node = cur_node->sibling;\
                                    if(cur_node->sibling != NULL && strcmp(cur_node->name,#name_6)==0){\
                                        cur_node = cur_node->sibling;\
                                        if(cur_node->sibling != NULL && strcmp(cur_node->name,#name_7)==0){\
                                            cur_node = cur_node->sibling;\
                                            if(cur_node->sibling == NULL && strcmp(cur_node->name,#name_8)==0){\
                                                flag = true;\
                                            }\
                                        }\
                                    }\
                                }\
                            }\
                        }\
                    }\
                }\
            }\
        }\
        return flag;\
    }

#define GET_NAME char * name = (char*)malloc(MAX_NAME_LENGTH*sizeof(char));strcpy(name,_id->val.type_string);

//error type [错误类型] at Line [行号]: [说明文字].
#define ERROR(error_type,line)\
    if(no_error)no_error = false;\
    printf("Error type %d at Line %d : %s\n",error_type,line,error_type_##error_type##_description);
    
#define ERROR_UPLOAD(name)\
    FieldList item = find(name);\
    if(item!=NULL&&item->type->kind==ERROR_TYPE);\
    else{\
        FieldList item = (FieldList)malloc(sizeof(struct FieldList_));\
        Type _type = (Type)malloc(sizeof(struct Type_));\
        _type->kind = ERROR_TYPE;\
        _type->u.basic = TYPE_UNDEFINED;\
        char * _name = (char*)malloc(MAX_NAME_LENGTH*sizeof(char));\
        strcpy(_name,name);\
        item->type = _type;\
        item->name = _name;\
        insert(item);\
    }

