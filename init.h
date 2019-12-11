#pragma once
#include <cstdio>
#include <string>
#include <fstream>
using namespace std;

char ch;
#define norw 9    //�ؼ��ָ���
#define txmax 100  //���ֱ�����
#define nmax 14    //number�����λ��
#define al 10      //���ŵ���󳤶�
#define symnum 25  
#define fctnum 14
#define levmax 2
#define cxmax 500 //���ָ����
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
enum symbol sym;//��ǰ���ŵ�����
char id[al + 1];//��ǰident�������һ���ֽ����ڴ��0
int num;//��ǰnumber
int cc, ll;//getchʹ�õļ�������cc��ʾ��ǰ�ַ���ch����λ��
int cx;
int rt; //����ֵƫ����

string word[norw];//������
char line[100];//��ȡ�л�����
char a[al + 1];
struct instruction
{
	enum fct f;
	int l;
	int a;
};
instruction code[cxmax];
enum symbol wsym[norw];//�����ֶ�Ӧ�ķ���ֵ
enum symbol ssym[256];//���ַ��ķ���ֵ
string mnemonic[fctnum];
struct tablestruct
{
	string name;
	enum symbol kind;
	int level;
	int addr;
	int type;//��������
};
tablestruct table[txmax];
fstream order;
fstream ifs;
int lp = 0;
int cm;//��������תָ��ĵ�ַ
int isOver = 0;
int ifcx[1000];
int ifcount = 0;
bool isIf = false;
bool hasReturn = false;
int jpc;