%option yylineno 

/* %locations */
%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "syntax_tree.h"
    #include "syntax.tab.h"
    void raise_error();
    int yycolumn = 1;
    int last_error_line = -1;
    #define YY_USER_ACTION \
    yylloc.first_line = yylloc.last_line = yylineno;\
    yylloc.first_column = yycolumn;\
    yylloc.last_column = yycolumn + yyleng - 1;\
    yycolumn += yyleng;
%}

/*sub define*/
digit [0-9]
letter [_a-zA-Z]
decimalism [1-9]{digit}*|0
octal 0[0-7]+
invalid_octal 0[0-9]+
hex (0X[0-9a-f]+)|(0x[0-9A-F]+)
invalid_hex (0X[0-9a-z]+)|(0x[0-9A-Z]+)

space [ ]
enter \n
carriage_return \r
tab \t


decimal {decimalism}\.{digit}+
base {digit}*\.{digit}+|{digit}+\.
exp {base}[Ee][+-]?{digit}+

/* int,float.id*/
yyint  {decimalism}|{octal}|{hex}
yyfloat {decimal}|{exp}
invalid_float (\.{digit}+)|([\.0-9]*\.[0-9]*\.[\.0-9]*)
yyid {letter}([_a-zA-Z0-9]*)
/* invalid_id [_a-zA-Z0-9]+ */
/* operators*/
yysemi ";"
yycomma ","
yyassignop "="
yyrelop ">"|"<"|">="|"<="|"=="|"!="
yyplus "+"
yyminus "-"
yystar "*"
yydiv "/"
yyand "&&"
yyor "||"
yydot "."
yynot "!"
yytype "int"|"float"
yylp "("
yyrp ")"
yylb "["
yyrb "]"
yylc "{"
yyrc "}"
yystruct "struct"
yyreturn "return"
yyif "if"
yyelse "else"
yywhile "while"
%%
{octal} {yylval.node = (Syntax_tree*)malloc(sizeof(Syntax_tree));yylval.node->val.type_int=strtol(yytext,NULL,8);return INT;}
{invalid_octal} {char msg[0x40];sprintf(msg,"invalid octal number \"%s\"",yytext);raise_error(A,yylloc.last_line,msg);return INT;last_error_line=yylloc.first_line;}
{hex} {yylval.node = (Syntax_tree*)malloc(sizeof(Syntax_tree));yylval.node->val.type_int=strtol(yytext,NULL,16);return INT;}
{invalid_hex} {char msg[0x40];sprintf(msg,"invalid hex number \"%s\"",yytext);raise_error(A,yylloc.last_line,msg);return INT;last_error_line=yylloc.first_line;}
{yyint} {yylval.node = (Syntax_tree*)malloc(sizeof(Syntax_tree));yylval.node->val.type_int = atoi(yytext);return INT;}
{yyfloat} {yylval.node = (Syntax_tree*)malloc(sizeof(Syntax_tree));yylval.node->val.type_float=atof(yytext);return FLOAT;}
{invalid_float} {char msg[0x40];sprintf(msg,"invalid float number \"%s\"",yytext);raise_error(A,yylloc.last_line,msg);return INT;last_error_line=yylloc.first_line;}
{yysemi} { return SEMI;}
{yycomma} { return COMMA;}
{yyassignop} { return ASSIGNOP;}
{yyrelop} { return RELOP;}
{yyplus} { return PLUS;}
{yyminus} { return MINUS;}
{yystar} { return STAR;}
{yydiv} { return DIV;}
{yyand} { return AND;}
{yyor} { return OR;}
{yydot} { return DOT;}
{yynot} { return NOT;}
{yytype} {yylval.node = (Syntax_tree*)malloc(sizeof(Syntax_tree)); strcpy(yylval.node->val.type_string,yytext);return TYPE;}
{yylp} { return LP;}
{yyrp} { return RP;}
{yylb} { return LB;}
{yyrb} { return RB;}
{yylc} { return LC;}
{yyrc} { return RC;}
{yystruct} { return STRUCT;}
{yyreturn} { return RETURN;}
{yyif} { return IF;}
{yyelse} { return ELSE;}
{yywhile} { return WHILE;}
{yyid} {yylval.node = (Syntax_tree*)malloc(sizeof(Syntax_tree)); strcpy(yylval.node->val.type_string,yytext);return ID;}
{enter} {yycolumn = 1;}
{space}|{tab}|{carriage_return} {}
. {char msg[0x40];sprintf(msg,"Mysterious character \"%s\"",yytext);raise_error(A,yylloc.last_line,msg);last_error_line=yylloc.first_line;}
%%
