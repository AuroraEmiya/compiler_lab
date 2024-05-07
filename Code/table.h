#pragma once
#include <stdio.h>
#include "syntax_tree.h"
#include <string.h>
// #include "macro.h"

typedef struct Type_ *Type;
typedef struct FieldList_ *FieldList;

// basic 包含 TYPE_INT TYPE_FLOAT TYPE_UNDEFINED
// 如果出现未定义的类，将其的u.basic 值赋为 TYPE_UNDEFINED
enum KIND
{
    FUNCTION,
    BASIC,
    ARRAY,
    STRUCTURE,
    ERROR_TYPE
};
struct Type_
{
    enum KIND kind;
    union
    {
        // 函数，包括函数返回值，函数名，函数参数
        struct
        {
            // 函数返回值
            Type rt_type;
            // 函数结构体
            FieldList args;
            bool defined;
            int line;
        } function;

        // 基本类型
        int basic;
        // 数组类型信息包括元素类型与数组大小构成
        struct
        {
            Type elem;
            int size;
        } array;
        // 结构体类型信息是一个链表
        FieldList structure;
    } u;
};

// tell if two args of the same type
bool same_type(Type type_1, Type type_2);
bool same_field_type(FieldList item_1, FieldList item_2);

struct FieldList_
{
    char *name;     // 域的名字
    Type type;      // 域的类型
    FieldList tail; // 下一个域
};
typedef struct Barrel_* Barrel;
struct Barrel_ {
    FieldList item;
    Barrel next;
};

Barrel table[0x3fff];

// hash function
unsigned int hash(char *name);

void inithash();

FieldList find(char *name);

void insert(FieldList item);

// semantic analysis
void Program(Syntax_tree *root);
void ExtDefList(Syntax_tree *node);
void ExtDef(Syntax_tree *node);
void ExtDecList(Syntax_tree *node, Type type);
void CompSt(Syntax_tree *node, Type rt_type);
void StmtList(Syntax_tree *node, Type type);
void Stmt(Syntax_tree *node, Type rt_type);
FieldList FunDec(Syntax_tree *node, Type rt_type /*返回值*/);

char *OptTag(Syntax_tree *node);
char *Tag(Syntax_tree *node);

Type Specifier(Syntax_tree *node);
Type StructSpecifier(Syntax_tree *node);

FieldList VarDec(Syntax_tree *node, Type type, enum KIND head_field_kind);
FieldList VarList(Syntax_tree *node);
FieldList ParamDec(Syntax_tree *node);
FieldList DefList(Syntax_tree *node, enum KIND head_field_kind);
FieldList Def(Syntax_tree *node, enum KIND head_field_kind);
FieldList DecList(Syntax_tree *node, Type type, enum KIND head_field_kind);
FieldList Dec(Syntax_tree *node, Type type, enum KIND head_field_kind);
FieldList Exp(Syntax_tree *node);
FieldList Args(Syntax_tree *node);


// （1）错误类型 1：变量在使用时未经定义。
char error_type_1_description[] = "变量在使用时未经定义";

// （2）错误类型 2：函数在调用时未经定义
char error_type_2_description[] = "函数在调用时未经定义";

// （3）错误类型 3：变量出现重复定义，或变量与前面定义过的结构体名字重复。
char error_type_3_description[] = "变量出现重复定义，或变量与前面定义过的结构体名字重复";

// （4）错误类型 4：函数出现重复定义（即同样的函数名出现了不止一次定义）。
char error_type_4_description[] = "函数出现重复定义.";

// （5）错误类型 5：赋值号两边的表达式类型不匹配。
char error_type_5_description[] = "赋值号两边的表达式类型不匹配";

// （6）错误类型 6：赋值号左边出现一个只有右值的表达式。
char error_type_6_description[] = "赋值号左边出现一个只有右值的表达式";

// （7）错误类型 7：操作数类型不匹配或操作数类型与操作符不匹配（例如整型变量与数组变
// 量相加减，或数组（或结构体）变量与数组（或结构体）变量相加减）。
char error_type_7_description[] = "操作数类型不匹配或操作数类型与操作符不匹配";

// （8）错误类型 8：return 语句的返回类型与函数定义的返回类型不匹配。
char error_type_8_description[] = "return 语句的返回类型与函数定义的返回类型不匹配";

// （9）错误类型 9：函数调用时实参与形参的数目或类型不匹配。
char error_type_9_description[] = "函数调用时实参与形参的数目或类型不匹配";

// （10）错误类型 10：对非数组型变量使用“[…]”（数组访问）操作符。
char error_type_10_description[] = "对非数组型变量使用“[…]”（数组访问）操作符";

// （11）错误类型 11：对普通变量使用“(…)”或“()”（函数调用）操作符。
char error_type_11_description[] = "对普通变量使用“(…)”或“()”（函数调用）操作符";

// （12）错误类型 12：数组访问操作符“[…]”中出现非整数（例如 a[1.5]）。
char error_type_12_description[] = "数组访问操作符“[…]”中出现非整数";

// （13）错误类型 13：对非结构体型变量使用“.”操作符。
char error_type_13_description[] = "对非结构体型变量使用“.”操作符";

// （14）错误类型 14：访问结构体中未定义过的域。
char error_type_14_description[] = "访问结构体中未定义过的域";

// （15）错误类型 15：结构体中域名重复定义（指同一结构体中），或在定义时对域进行初始
// 化（例如 struct A {int a=0;}）。
char error_type_15_description[] = "结构体中域名重复定义（指同一结构体中），或在定义时对域进行初始化";

// （16）错误类型 16：结构体的名字与前面定义过的结构体或变量的名字重复。
char error_type_16_description[] = "结构体的名字与前面定义过的结构体或变量的名字重复";

// （17）错误类型 17：直接使用未定义过的结构体来定义变量
char error_type_17_description[] = "直接使用未定义过的结构体来定义变量";

// 要求 3.1：修改前面的 C−−语言假设 3，使其变为“函数除了在定义之外还可以进行声
// 明”。函数的定义仍然不可以重复出现，但函数的声明在相互一致的情况下可以重复出现。任一
// 函数无论声明与否，其定义必须在源文件中出现。在新的假设 3 下，程序还需要检查两类新的错
// 误和增加新的产生式：

// 错误类型 18：函数进行了声明，但没有被定义。
char error_type_18_description[] = "函数进行了声明，但没有被定义";

// 错误类型 19：函数的多次声明互相冲突（即函数名一致，但返回类型、形参数量或者形
// 参类型不一致），或者声明与定义之间互相冲突。
char error_type_19_description[] = "函数的多次声明互相冲突";