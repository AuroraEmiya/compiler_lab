
%{
    #include <stdio.h>
    #include "lex.yy.c"
    //所有的宏的定义都放在macro.h中
    #include "macro.h"
    extern bool no_error;
    // #include"syntax_tree.h"
    void yyerror (char const *);
    void raise_error(int TYPE,int line ,const char* msg);
    Syntax_tree* children[MAX_CHILDREN_NUM];
    extern int yylineno;
    Syntax_tree *root ;
%}
%locations
//修改TTYTYPE
%union{
    Syntax_tree* node;
}


/* declared tokens定义运算终结符，并标定它们的优先级*/
%token <node>INT 
%token <node>FLOAT 
%token <node>TYPE
%token <node>SEMI COMMA
%token <node>ID
%token <node>STRUCT
%token <node>RETURN
%token <node>IF ELSE WHILE
%token <node>ASSIGNOP OR AND RELOP PLUS MINUS STAR DIV NOT LP RP LB RB LC RC DOT

/*declared type token*/
%type  <node>Program ExtDecList ExtDef ExtDefList
%type  <node>Specifier StructSpecifier OptTag Tag
%type  <node>VarDec FunDec VarList ParamDec
%type  <node>CompSt StmtList Stmt
%type  <node>DefList Def DecList Dec
%type  <node>Exp Args

/*op*/
%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left LP RP LB RB DOT
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%%
/*High level Definations*/
Program : ExtDefList                                    {SPAWN_1($$,@$,"Program");root=($$);CHILDREN_1($1);build_tree($$,children,1);CLEAN;} 
    ;
ExtDefList : ExtDef ExtDefList                          {SPAWN_1($$,@$,"ExtDefList");CHILDREN_2($1,$2);build_tree($$,children,2);CLEAN;}
    | /* empty */                                       {SPAWN_1($$,@$,"ExtDefList");CLEAN;}
    ;
ExtDef : Specifier ExtDecList SEMI                      {SPAWN_2($$,@$,"ExtDef",$3,@3,"SEMI");CHILDREN_3($1,$2,$3);build_tree($$,children,3);CLEAN;}
    | Specifier SEMI                                    {SPAWN_2($$,@$,"ExtDef",$2,@2,"SEMI");CHILDREN_2($1,$2);build_tree($$,children,2);CLEAN;}
    | Specifier FunDec CompSt                           {SPAWN_1($$,@$,"ExtDef");;CHILDREN_3($1,$2,$3);build_tree($$,children,3);CLEAN;}
    /* | Specifier FunDec SEMI                               */
    | error ExtDecList  SEMI                            {raise_error(B,yylloc.last_line,"can not assign when declaring");}
    | Specifier error CompSt                            {raise_error(B,@$.first_line,"bad definations of func");}
    /* | ID ID SEMI                                     {raise_error(B,@$.first_line,"bad type");} */
    /* | Specifier error SEMI                           {raise_error(B,yylloc.first_line,"can not assign when declaring");} */
    ;
ExtDecList : VarDec                                     {SPAWN_1($$,@$,"ExtDecList");CHILDREN_1($1);build_tree($$,children,1);CLEAN;}
    | VarDec COMMA ExtDecList                           {SPAWN_2($$,@$,"ExtDecList",$2,@2,"COMMA");CHILDREN_3($1,$2,$3);build_tree($$,children,3);CLEAN;}
    ;
/*Specifiers*/
Specifier : TYPE                                        {SPAWN_2($$,@$,"Specifier",$1,@1,"TYPE");CHILDREN_1($1);build_tree($$,children,1);CLEAN;}
    | StructSpecifier                                   {SPAWN_1($$,@$,"Specifier");CHILDREN_1($1);build_tree($$,children,1);CLEAN;}
    
    /* | error Tag                                      {raise_error(B,yylloc.first_line,"illigal type specifier,to be done....");}   */
    ;

StructSpecifier : STRUCT OptTag LC DefList RC           {SPAWN_4($$,@$,"StructSpecifier",$1,@1,"STRUCT",$3,@3,"LC",$5,@5,"RC");CHILDREN_5($1,$2,$3,$4,$5);build_tree($$,children,5);CLEAN;}
    | STRUCT Tag                                        {SPAWN_2($$,@$,"StructSpecifier",$1,@1,"STRUCT");CHILDREN_2($1,$2);build_tree($$,children,2);CLEAN;}
    | STRUCT error LC DefList RC                        {raise_error(B,@$.first_line,"bad name for struct QAQ");}
    | STRUCT error                                      {raise_error(B,@$.first_line,"need name for struct QAQ");}
    /* | error  */
    ;
OptTag : ID                                             {SPAWN_2($$,@$,"OptTag",$1,@1,"ID");CHILDREN_1($1);build_tree($$,children,1);CLEAN;}
    | /* empty */                                       {SPAWN_1($$,@$,"OptTag");CLEAN;}
    ;
Tag : ID                                                 {SPAWN_2($$,@$,"Tag",$1,@1,"ID");CHILDREN_1($1);build_tree($$,children,1);CLEAN;}
    ;
/* Declarators */
VarDec : ID                                             {SPAWN_2($$,@$,"VarDec",$1,@1,"ID");CHILDREN_1($1);build_tree($$,children,1);CLEAN;}
    | FunDec                                            {SPAWN_1($$,@$,"VarDec");CHILDREN_1($1);build_tree($$,children,1);CLEAN;}
    | VarDec LB INT RB                                  {SPAWN_4($$,@$,"VarDec",$2,@3,"LB",$3,@3,"INT",$4,@4,"RB");CHILDREN_4($1,$2,$3,$4);build_tree($$,children,4);CLEAN;}
    | VarDec LB  error RB                               {raise_error(B,yylloc.last_line,"only can you use INT");}                                    
    ;
FunDec : ID LP VarList RP                               {SPAWN_4($$,@$,"FunDec",$1,@1,"ID",$2,@2,"LP",$4,@4,"RP");CHILDREN_4($1,$2,$3,$4);build_tree($$,children,4);CLEAN;}
    | ID LP RP                                          {SPAWN_4($$,@$,"FunDec",$1,@1,"ID",$2,@2,"LP",$3,@3,"RP");CHILDREN_3($1,$2,$3);build_tree($$,children,3);CLEAN;}
    ;
VarList : ParamDec COMMA VarList                        {SPAWN_2($$,@$,"VarList",$2,@2,"COMMA");CHILDREN_3($1,$2,$3);build_tree($$,children,3);CLEAN;}
    | ParamDec                                          {SPAWN_1($$,@$,"VarList");CHILDREN_1($1);build_tree($$,children,1);CLEAN;}
    ;
ParamDec : Specifier VarDec                             {SPAWN_1($$,@$,"ParamDec");CHILDREN_2($1,$2);build_tree($$,children,2);CLEAN;}
    ;
/* Statements */ 
CompSt : LC DefList StmtList RC                         {SPAWN_3($$,@$,"CompSt",$1,@1,"LC",$4,@4,"RC");CHILDREN_4($1,$2,$3,$4);build_tree($$,children,4);CLEAN;}
    /* |  LC error  RC                                  {raise_error(B,@$.first_line,"bad definations");}  */
    /* | LC DefList error RC                            {raise_error(B,@$.first_line,"bad definations");} */
    /* | LC error DefList StmtList RC                   {raise_error(B,@$.first_line,"bad definations");}  */
    /* | LC DefList error  StmtList RC                  {raise_error(B,@$.first_line,"bad definations");}  */
    
    ;
StmtList : Stmt StmtList                                {SPAWN_1($$,@$,"StmtList");CHILDREN_2($1,$2);build_tree($$,children,2);CLEAN;}
    | /* empty */                                       {SPAWN_1($$,@$,"StmtList");CLEAN;}
    ;
Stmt : Exp SEMI                                         {SPAWN_2($$,@$,"Stmt",$2,@2,"SEMI");CHILDREN_2($1,$2);build_tree($$,children,2);CLEAN;}
    | CompSt                                            {SPAWN_1($$,@$,"Stmt");CHILDREN_1($1);build_tree($$,children,1);CLEAN;}
    | RETURN Exp SEMI                                   {SPAWN_3($$,@$,"Stmt",$1,@1,"RETURN",$3,@3,"SEMI");CHILDREN_3($1,$2,$3);build_tree($$,children,3);CLEAN;}
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE           {SPAWN_4($$,@$,"Stmt",$1,@1,"IF",$2,@2,"LP",$4,@4,"RP");CHILDREN_5($1,$2,$3,$4,$5);build_tree($$,children,5);CLEAN;}
    | IF LP Exp RP Stmt ELSE Stmt                       {SPAWN_5($$,@$,"Stmt",$1,@1,"IF",$2,@2,"LP",$4,@4,"RP",$6,@6,"ELSE");CHILDREN_7($1,$2,$3,$4,$5,$6,$7);build_tree($$,children,7);CLEAN;}
    | WHILE LP Exp RP Stmt                              {SPAWN_4($$,@$,"Stmt",$1,@1,"WHILE",$2,@2,"LP",$4,@4,"RP");CHILDREN_5($1,$2,$3,$4,$5);build_tree($$,children,5);CLEAN;}
    | IF LP Exp RP error ELSE Stmt                      {raise_error(B,@$.last_line,"bad if stmt");}
    | IF LP Exp RP Stmt ELSE error                      {raise_error(B,@$.last_line,"bad if stmt");}
    /* | error Exp SEMI */
    | error SEMI                                        {raise_error(B,@$.last_line,"not a stmt");} 
    /* | WHILE LP error RP LC error RC                          {raise_error(B,@$.first_line,"expression error,perhaps operations's sake x_x ");}  */
    /* | WHILE LP error RP Stmt                            {raise_error(B,@$.first_line,"expression error,perhaps operations's sake x_x ");}  */
    | WHILE LP Exp error Stmt                           {raise_error(B,@$.first_line,"bad while structure");}   
    /* | Exp error                                         {raise_error(B,@$.first_line,"I guess is a expression error ,but whatever >_>");}  */
    | Exp error SEMI                                    {raise_error(B,@$.first_line,"bad stmt");}
    ;
/* Local Definations */ 
DefList : Def DefList                                   {SPAWN_1($$,@$,"DefList");CHILDREN_2($1,$2);build_tree($$,children,2);CLEAN;}
    
    /* | DefList error */
    /* | error DefList                                             {raise_error(B,yylloc.first_line,"illigal type specifier,to be done....");}  */
    | /* empty */                                       {SPAWN_1($$,@$,"DefList");CLEAN;}
    ;
Def : Specifier DecList SEMI                            {SPAWN_2($$,@$,"Def",$3,@3,"SEMI");CHILDREN_3($1,$2,$3);build_tree($$,children,3);CLEAN;}
    | Specifier error SEMI                              {raise_error(B,@$.last_line,"Bad local declaration,maybe lost id ");} 
    /* | ID DecList SEMI                                   {raise_error(B,@$.first_line,"bad type");}  */
    /* | error DecList SEMI */
    /* | error DecList SEMI                                {raise_error(B,yylloc.first_line,"Illigal type specifier");}  */
    ;
DecList : Dec                                           {SPAWN_1($$,@$,"DecList");CHILDREN_1($1);build_tree($$,children,1);CLEAN;}
    | Dec COMMA DecList                                 {SPAWN_2($$,@$,"DecList",$2,@2,"COMMA");CHILDREN_3($1,$2,$3);build_tree($$,children,3);CLEAN;}
    ;
Dec : VarDec                                            {SPAWN_1($$,@$,"Dec");CHILDREN_1($1);build_tree($$,children,1);CLEAN;}
    | VarDec ASSIGNOP Exp                               {SPAWN_2($$,@$,"Dec",$2,@2,"ASSIGNOP");CHILDREN_3($1,$2,$3);build_tree($$,children,3);CLEAN;}                    
    ;
/* expressions */ 
Exp : Exp ASSIGNOP Exp                                  {SPAWN_2($$,@$,"Exp",$2,@2,"ASSIGNOP");CHILDREN_3($1,$2,$3);build_tree($$,children,3);CLEAN;}
    | Exp AND Exp                                       {SPAWN_2($$,@$,"Exp",$2,@2,"AND");CHILDREN_3($1,$2,$3);build_tree($$,children,3);CLEAN;}
    | Exp OR Exp                                        {SPAWN_2($$,@$,"Exp",$2,@2,"OR");CHILDREN_3($1,$2,$3);build_tree($$,children,3);CLEAN;}
    | Exp RELOP Exp                                     {SPAWN_2($$,@$,"Exp",$2,@2,"RELOP");CHILDREN_3($1,$2,$3);build_tree($$,children,3);CLEAN;}
    | Exp PLUS Exp                                      {SPAWN_2($$,@$,"Exp",$2,@2,"PLUS");CHILDREN_3($1,$2,$3);build_tree($$,children,3);CLEAN;}
    | Exp MINUS Exp                                     {SPAWN_2($$,@$,"Exp",$2,@2,"MINUS");CHILDREN_3($1,$2,$3);build_tree($$,children,3);CLEAN;}
    | Exp STAR Exp                                      {SPAWN_2($$,@$,"Exp",$2,@2,"STAR");CHILDREN_3($1,$2,$3);build_tree($$,children,3);CLEAN;}                    
    | Exp DIV Exp                                       {SPAWN_2($$,@$,"Exp",$2,@2,"DIV");CHILDREN_3($1,$2,$3);build_tree($$,children,3);CLEAN;}
    | LP Exp RP                                         {SPAWN_3($$,@$,"Exp",$1,@1,"LP",$3,@3,"RP");CHILDREN_3($1,$2,$3);build_tree($$,children,3);CLEAN;}
    | MINUS Exp                                         {SPAWN_2($$,@$,"Exp",$1,@1,"MINUS");CHILDREN_2($1,$2);build_tree($$,children,2);CLEAN;}                    
    | NOT Exp                                           {SPAWN_2($$,@$,"Exp",$1,@1,"NOT");CHILDREN_2($1,$2);build_tree($$,children,2);CLEAN;}                    
    | ID LP Args RP                                     {SPAWN_4($$,@$,"Exp",$1,@1,"ID",$2,@2,"LP",$4,@4,"RP");CHILDREN_4($1,$2,$3,$4);build_tree($$,children,4);CLEAN;}                        
    | ID LP RP                                          {SPAWN_4($$,@$,"Exp",$1,@1,"ID",$2,@2,"LP",$3,@3,"RP");CHILDREN_3($1,$2,$3);build_tree($$,children,3);CLEAN;}
    | Exp LB Exp RB                                     {SPAWN_3($$,@$,"Exp",$2,@2,"LB",$4,@4,"RB");CHILDREN_4($1,$2,$3,$4);build_tree($$,children,4);CLEAN;}
    | Exp DOT ID                                        {SPAWN_3($$,@$,"Exp",$2,@2,"DOT",$3,@3,"ID");CHILDREN_3($1,$2,$3);build_tree($$,children,3);CLEAN;}
    | ID                                                {SPAWN_2($$,@$,"Exp",$1,@1,"ID");CHILDREN_1($1);build_tree($$,children,1);CLEAN;}
    | INT                                               {SPAWN_2($$,@$,"Exp",$1,@1,"INT");CHILDREN_1($1);build_tree($$,children,1);CLEAN;}
    | FLOAT                                             {SPAWN_2($$,@$,"Exp",$1,@1,"FLOAT");CHILDREN_1($1);build_tree($$,children,1);CLEAN;}
    | Exp LB error RB                                   {raise_error(B,@$.first_line,"[?] need to be INT");}
    | Exp RELOP error                                   {raise_error(B,@$.first_line,"Expression error at relop");last_error_line = yylloc.last_line;}
    | Exp PLUS error                                    {raise_error(B,@$.first_line,"Expression error at \"+\"");UPDATE;}
    | Exp MINUS error                                   {raise_error(B,@$.first_line,"Expression error at \"-\"");UPDATE;}
    //| LP Exp error                                        {raise_error(B,yylloc.last_line,"Expression error at \"-\"");UPDATE;} 
    /* | Exp ASSIGNOP ASSIGNOP Exp                         {raise_error(B,yylloc.last_line,"???");} */
    /* | Exp error Exp                                     {raise_error(B,yylloc.last_line,"you need to check your operators (need no space!)");} */
    
    //| Exp PLUS error                                    {raise_error(B,yylloc.last_line,"undefine");} 
    ;
Args : Exp COMMA Args                                   {SPAWN_2($$,@$,"Args",$2,@2,"COMMA");CHILDREN_3($1,$2,$3);build_tree($$,children,3);CLEAN;}
    | Exp                                               {SPAWN_1($$,@$,"Args");CHILDREN_1($1);build_tree($$,children,1);CLEAN;}
    ;

%%
void yyerror(const char * s){
    /* fprintf(stderr,"%s.\n",s); */
    return ;
}

void raise_error(int TYPE,int line ,const char* msg){
    if (line == last_error_line)return ;
    char error[0x80];
    char at_line[0x4];
    if (TYPE==A){
        sprintf(error,"Error type A at line %d: ",line);
        strcat(error,msg);
        fprintf(stdout,"%s\n",error);
        /* yyerror(error); */
        no_error = false;
    }
    else if (TYPE==B){
        sprintf(error,"Error type B at line %d: ",line);
        strcat(error,msg);
        fprintf(stdout,"%s\n",error);
        /* yyerror(error); */
        no_error = false;
    }
    else {
        fprintf(stdout,"UNDEFINED ERROR TYPE");
        no_error = false;
    }
}

/* https://github.com/NijikaIjichi/nju-compiler-test/tree/master */
/* python3 test.py [-p parser路径] [-g 组号] [-a] [-w] [-t 时限] [-m] */
/* https://www.jq22.com/textDifference */
/* https://compiler.pascal-lab.net */
/* 实验报告:https://tex.nju.edu.cn/share/0f5a442a827a4b1fb28cde622dacc392 */
/*

python3 test.py -p /home/sakura/complier_lab/Lab/Code/parser -g 1
*/