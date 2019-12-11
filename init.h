#pragma once
#include <cstdio>
#include <string>
#include <fstream>
using namespace std;

char ch;
#define norw 9    //关键字个数
#define txmax 100  //名字表容量
#define nmax 14    //number的最大位数
#define al 10      //符号的最大长度
#define symnum 25  
#define fctnum 14
#define levmax 2
#define cxmax 500 //最大指令数
enum fct
{
	LIT,
	LOD,
	STO,
	CAL,
	INT,
	JMP,
	JPC,
	ADD,
	SUB,
	MUL,
	DIV,
	RED,
	WRT,
	RET,
};
enum symbol
{
	nul,
	ident,
	number,
	_plus,
	_minus,
	eql,
	times,
	slash,
	lparen,
	rparen,
	lbrace,
	rbrace,
	comma,
	semicolon,
	period,
	becomes,
	ifsym,
	elsesym,
	whilesym,
	returnsym,
	scanfsym,
	printfsym,
	_int,
	_void,
	function,
};
enum symbol sym;//当前符号的类型
char id[al + 1];//当前ident，多出的一个字节用于存放0
int num;//当前number
int cc, ll;//getch使用的计数器，cc表示当前字符（ch）的位置
int cx;
int rt; //返回值偏移量

string word[norw];//保留字
char line[100];//读取行缓冲区
char a[al + 1];
struct instruction
{
	enum fct f;
	int l;
	int a;
};
instruction code[cxmax];
enum symbol wsym[norw];//保留字对应的符号值
enum symbol ssym[256];//单字符的符号值
string mnemonic[fctnum];
struct tablestruct
{
	string name;
	enum symbol kind;
	int level;
	int addr;
	int type;//函数类型
};
tablestruct table[txmax];
fstream order;
fstream ifs;
int lp = 0;
int cm;//主函数跳转指令的地址
int isOver = 0;
int ifcx[1000];
int ifcount = 0;
bool isIf = false;
bool hasReturn = false;
int jpc;