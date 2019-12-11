#include<iostream>
#include<string>
#include<stdio.h>
#include<fstream>
#include<algorithm>
#include "init.h"
using namespace std;
//初始化
void init();
//获取一个词
int getch();
//生成目标代码
int gen(enum fct x, int y, int z);
//词法分析，获取词的类型
int getsym();
//变量声明
int vardeclaration(int* ptx, int lev, int* pdx);
//查找变量或函数在不在表中及在表中的位置
int position(string idt, int tx);
//项处理
int term(int* ptx, int lev);
//表达式处理
int expression(int* ptx, int lev);
//因子处理
int factor(int* ptx, int lev);
//将变量或函数加入名字表中
void enter(enum symbol k, int* ptx, int lev, int* pdx, int type);
//错误处理
void error(int n);
//语句处理
void statement(int* ptx, int lev, bool isOut);
//显示目标代码并将其保存到文件
void listcode(int cx0);
//函数或变量声明分析
int block(int lev, int tx);
//判断是不是主函数
bool isMain();

int main() {
	init();
	cc = cx = ll = 0;
	ch = ' ';
	getsym();
	block(0, 0);
	ifs.close();
	listcode(0);
	order.close();
}

void init()
{
	int i;
	//设置单字符符号
	for (i = 0; i < 256; i++)
	{
		ssym[i] = nul;
	}

	ssym['+'] = _plus;
	ssym['-'] = _minus;
	ssym['*'] = times;
	ssym['/'] = slash;
	ssym['('] = lparen;
	ssym[')'] = rparen;
	ssym['{'] = lbrace;
	ssym['}'] = rbrace;
	ssym['='] = eql;
	ssym[','] = comma;
	ssym['.'] = period;
	ssym[';'] = semicolon;
	word[0] = "else";
	word[1] = "function";
	word[2] = "if";
	word[3] = "int";
	word[4] = "printf";
	word[5] = "return";
	word[6] = "scanf";
	word[7] = "void";
	word[8] = "while";

	//为了使用二分对关键词进行排序
	/*sort(word, word + 9);

	for (int i = 0; i < 10; i++)
		cout << word[i] << endl;*/

	wsym[0] = elsesym;
	wsym[1] = function;
	wsym[2] = ifsym;
	wsym[3] = _int;
	wsym[4] = printfsym;
	wsym[5] = returnsym;
	wsym[6] = scanfsym;
	wsym[7] = _void;
	wsym[8] = whilesym;
	mnemonic[LIT] = "LIT";
	mnemonic[LOD] = "LOD";
	mnemonic[STO] = "STO";
	mnemonic[CAL] = "CAL";
	mnemonic[INT] = "INT";
	mnemonic[JMP] = "JMP";
	mnemonic[JPC] = "JPC";
	mnemonic[ADD] = "ADD";
	mnemonic[SUB] = "SUB";
	mnemonic[MUL] = "MUL";
	mnemonic[DIV] = "DIV";
	mnemonic[RED] = "RED";
	mnemonic[WRT] = "WRT";
	mnemonic[RET] = "RET";

	//读取源代码文件
	string path;
	cout << "请输入文件名:" << endl;

	cin >> path;

	ifs.open(path);
}

bool isMain() {
	char key[5] = { 'm', 'a', 'i', 'n' };
	key[4] = '\0';
	for (int i = 0; i < 3; i++) {
		if (id[i] != key[i])
			return false;
	}
	return true;
}

int getch()
{
	if (cc == ll)
	{
		//文件已经读完
		if (ifs.eof())
		{
			isOver = 1;
			//cout << "program incomplete" << endl;
			return -1;
		}
		ll = 0;
		cc = 0;
		//cout << cx << endl;
		ch = ' ';
		//读取一行
		while (ch != 10)
		{
			ch = ifs.get();
			if (ch == EOF)
			{
				line[ll] = 0;
				break;
			}
			//cout << ch << endl;
			line[ll] = ch;
			ll++;
		}
		//cout << "\n";
	}
	ch = line[cc];
	cc++;
	return 0;
}

int gen(enum fct x, int y, int z) {
	if (cx > cxmax) {
		cout << "程序过长" << endl;
		return -1;
	}
	code[cx].f = x;
	code[cx].l = y;
	code[cx].a = z;

	cx++;
	return 0;
}

int getsym()
{
	int i, j, k;
	//忽略空格换行等
	while (ch == ' ' || ch == 10 || ch == 9)
	{
		getch();
	}
	//如果是变量或函数名（以下划线或字母开头）
	if (ch >= 'a' && ch <= 'z' || ch == '_' || ch >= 'A' && ch <= 'Z')
	{
		k = 0;
		do
		{
			if (k < al)
			{
				a[k] = ch;
				k++;
			}
			else {
				cout << "标识符过长" << endl;
			}
			getch();
		} while (ch >= 'a' && ch <= 'z' || ch >= '0' && ch <= '9' || ch == '_' || ch >= 'A' && ch <= 'Z');
		//可以包含字母，下划线和数字
		a[k] = 0;
		strcpy_s(id, a);
		//开始进行二分查找，查看是不是关键字
		i = 0;
		j = norw - 1;
		do
		{
			k = (i + j) / 2;
			if (id <= word[k])
			{
				j = k - 1;
			}
			if (id >= word[k])
			{
				i = k + 1;
			}
		} while (i <= j);
		//如果可以找到，则是关键字
		if (i - 1 > j)
		{
			sym = wsym[k];
		}//找不到就是标识符
		else
		{
			sym = ident;
		}
	}//如果是数字
	else if (ch > '0' && ch <= '9')
	{
		k = 0;//记录数字的位数
		num = 0;
		sym = number;
		do
		{
			num = 10 * num + ch - '0';
			k++;
			getch();
		} while (ch >= '0' && ch <= '9');
		k--;
		//数字太长的话
		if (k > nmax)
		{
			//error(30);
			cout << "数字太长" << endl;
		}
	}//赋值号
	else if (ch == '=')
	{
		sym = becomes;
		getch();
	}//否则是其他符号
	else
	{
		sym = ssym[ch];
		if (sym != period)
		{
			getch();
		}
	}
	//否则不是可识别符号
	return 0;
}

int vardeclaration(int* ptx, int lev, int* pdx)//变量声明处理
{
	//将变量加入名字表中
	enter(_int, ptx, lev, pdx, _int);

	return 0;
}

int position(string idt, int tx) {
	int i;
	//设置哨兵进行扫描查找
	table[0].name = idt;
	i = tx;
	while (table[i].name != idt) {
		i--;
	}
	return i;
}

int term(int* ptx, int lev)//项处理
{
	enum symbol mulop; //保存乘除号
	factor(ptx, lev);
	while (sym == times || sym == slash)
	{
		mulop = sym;
		getsym();
		factor(ptx, lev);
		if (mulop == times)
		{
			gen(MUL, 0, 0);//生成乘法指令
		}
		else
		{
			gen(DIV, 0, 0);//生成除法指令
		}
	}
	return 0;
}

int expression(int* ptx, int lev)//表达式处理
{
	enum symbol addop;//用于保存正负号
	bool isNeg = false;
	if (sym == _plus || sym == _minus)
	{
		addop = sym;
		getsym();
		term(ptx, lev);
		if (addop == _minus)
		{
			isNeg = true;
			gen(LIT, 0, 0);
		}
	}
	else
	{
		term(ptx, lev);
	}
	while (sym == _plus || sym == _minus)
	{
		addop = sym;
		getsym();
		term(ptx, lev);
		if (addop == _plus)
		{
			gen(ADD, 0, 0);//生成加法指令
		}
		else
		{
			gen(SUB, 0, 0);//生成减法指令
		}
	}
	if (isNeg)
	{
		gen(SUB, 0, 0);
	}
	return 0;
}

int factor(int* ptx, int lev)//因子处理
{
	int i;
	if (sym == ident)//因子变量或函数
	{
		i = position(id, *ptx);//查找名字
		if (i == 0)//返回0，没找到
		{
			cout << "标识符未声明" << endl;
		}
		else
		{
			int adr = 0;
			switch (table[i].kind)
			{
			case _int://名字为常量
				//判断是不是全局
				if (table[i].level == 0) adr = 1;
				gen(LOD, adr, table[i].addr);//直接把常量入栈
				break;
			case function:
				getsym();
				if (sym != lparen) {
					cout << "函数调用缺少左括号" << endl;
				}
				getsym();
				if (sym != rparen) {
					cout << "函数调用缺少右括号" << endl;
				}
				//调用函数
				gen(CAL, 0, table[i].addr);
				//若函数有返回值，则取出返回值
				if (table[i].type == _int)
					gen(LOD, 1, rt);
			}
		}
		getsym();
	}
	else if (sym == number)//因子为数
	{
		//将常数入栈
		gen(LIT, 0, num);
		getsym();
	}
	else if (sym == lparen)//因子为表达式
	{
		getsym();
		//进行表达式处理
		expression(ptx, lev);
		if (sym == rparen)
		{
			getsym();
		}
		else
		{
			cout << "缺少右括号" << endl;
		}
	}
	return 0;
}

void enter(enum symbol k, int* ptx, int lev, int* pdx, int type) {
	int i = position(id, *ptx);
	if (i != 0) {
		if (table[i].level == lev) {
			cout << "变量或函数" << id << "重定义" << endl;
			exit(0);
		}
	}
	(*ptx)++;
	table[(*ptx)].name = id;
	table[(*ptx)].kind = k;
	switch (k)
	{
	case _int:
		table[(*ptx)].level = lev;
		table[(*ptx)].addr = (*pdx);
		(*pdx)++;
		break;
	case function:
		table[(*ptx)].level = lev;
		table[(*ptx)].addr = cx;
		table[(*ptx)].type = type;
		break;
	default:
		break;
	}
}

void statement(int* ptx, int lev, bool isOut) {
	int i, cx1, cx2;
	bool nxtlev[symnum];
	//变量声明
	if (sym == _int) {
		int dx = 0;

		getsym();
		if (sym == ident) {
			getsym();
		}
		else {
			cout << "缺少变量名" << endl;
		}
		//若含有','，则是多变量
		if (sym == comma) {
			vardeclaration(ptx, lev, &dx);
			while (sym == comma) {
				getsym();
				vardeclaration(ptx, lev, &dx);
				getsym();
			}
			if (sym == semicolon) {
				gen(INT, 0, dx);
			}
			else {
				cout << "缺少分号" << endl;
			}
		}//单变量
		else if (sym == semicolon) {
			vardeclaration(ptx, lev, &dx);
			gen(INT, 0, 1);
		}
		else {
			cout << "缺少分号" << endl;
		}
	}//标识符处理
	else if (sym == ident) {
		bool isBecome = false;
		i = position(id, *ptx);
		if (i == 0)
		{
			cout << "变量或函数未声明" << endl;
		}
		else {
			//如果为变量，则为赋值
			if (table[i].kind == _int)
			{
				getsym();
				if (sym == becomes) {
					isBecome = true;
					getsym();
				}
				else {
					cout << "未检测到等号" << endl;
				}
			}
			//否则直接进行表达式处理
			expression(ptx, lev);
			//如果是赋值,则将值赋给变量
			if (i != 0 && isBecome) {
				gen(STO, lev - table[i].level, table[i].addr);
			}
		}
	}//输入处理
	else if (sym == scanfsym) {
		getsym();
		if (sym != lparen)
		{
			cout << "缺少左括号" << endl;
		}
		else {
			getsym();
			if (sym == ident) {
				i = position(id, *ptx);
			}
			else {
				i = -1;
			}
			if (i == 0) {
				cout << "变量未声明" << endl;
			}
			else if (i == -1) {
				cout << "缺少变量名" << endl;
			}
			else {
				//开始读取输入
				gen(RED, 0, 0);
				gen(STO, lev - table[i].level, table[i].addr);
			}
			getsym();
			if (sym != rparen) {
				cout << "缺少右括号" << endl;
			}
			else {
				getsym();
			}
		}
	}//输出处理
	else if (sym == printfsym) {
		getsym();
		if (sym == lparen) {
			getsym();
			if (sym != ident) {
				cout << "缺少变量名" << endl;
			}
			//进行表达式处理
			expression(ptx, lev);
			//开始输出
			gen(WRT, 0, 0);
			if (sym != rparen) {
				cout << "缺少右括号" << endl;
			}
			else {
				getsym();
			}
		}
		else {
			cout << "缺少左括号" << endl;
		}
	}//if语句
	else if (sym == ifsym) {
		getsym();
		if (sym == lparen) {
			getsym();
			if (sym != ident) {
				cout << "缺少变量名" << endl;
			}
			expression(ptx, lev);
			if (sym != rparen) {
				cout << "缺少右括号" << endl;
			}
			else {
				getsym();
				//保存需要跳转的语句地址
				cx1 = cx;
				jpc = cx;
				gen(JPC, 0, 0);
				//判断if中是否存在return语句，存在则完成后不需要进行跳转
				hasReturn = false;
				statement(ptx, lev, false);
				//若没有return，完成后需要跳转到if-else整体语句块之外
				if (!hasReturn) {
					//保存elseif的数量
					ifcx[ifcount++] = cx;
					//每个else-if完成后都需要跳转到if-else整体语句块之外
					gen(JMP, 0, 0);
				}
				//条件不成立跳转到的地址
				code[cx1].a = cx;
			}
		}
		else {
			cout << "缺少左括号" << endl;
		}
		//记录上一句是if语句
		isIf = true;
	}//else语句处理
	else if (sym == elsesym) {
		getsym();
		statement(ptx, lev, false);
		//处理每个if分句的结束跳转
		for (int i = 0; i < ifcount; i++) {
			code[ifcx[i]].a = cx;
		}
		ifcount = 0;

	}
	//while语句
	else if (sym == whilesym) {
		//保存开始的地址，以便满足条件进行跳回
		cx1 = cx;
		getsym();
		if (sym == lparen) {
			getsym();
			if (sym != ident) {
				cout << "缺少变量名" << endl;
			}
			expression(ptx, lev);
			//保存需要跳转的语句地址
			cx2 = cx;
			gen(JPC, 0, 0);
			if (sym != rparen) {
				cout << "缺少右括号" << endl;
			}
			else {
				getsym();
				statement(ptx, lev, false);
				//满足条件跳回
				gen(JMP, 0, cx1);
				//处理不满足条件的跳出地址
				code[cx2].a = cx;
			}
		}
		else {
			cout << "缺少左括号" << endl;
		}
	}//复合语句
	else if (sym == lbrace) {
		//记录嵌套次数
		lp++;
		//进程复合语句处理
		getsym();
		statement(ptx, lev, false);
		while (sym == semicolon || (lp != 0 && isOut)) {
			getsym();
			//如果if后无else则if不需要跳转
			if (isIf && sym != elsesym && isOut) {
				cx--;
				code[jpc].a--;
				ifcount = 0;
			}
			isIf = false;
			//一层嵌套结束
			if (sym == rbrace) lp--;
			statement(ptx, lev, false);
		}
		//如果函数无返回值且未显式写出return
		if (!hasReturn && isOut) {
			gen(RET, 0, 0);
		}
	}
	else if (sym == returnsym) {
		getsym();
		hasReturn = true;
		//无返回值
		if (sym == semicolon) {
			gen(RET, 0, 0);
		}//有返回值
		else if (sym == ident) {
			expression(ptx, lev);
			gen(STO, 1, rt);
			gen(RET, 0, 0);
		}
		else if (sym == number)
		{
			gen(LIT, 0, num);
			gen(STO, 1, rt);
			gen(RET, 0, 0);
		}
		else {
			cout << "return后缺少变量或分号" << endl;
		}
	}
}

void listcode(int cx0)//输出目标代码并写入文件
{
	int i;
	order.open("orders.txt", ios::out);  //创建一个文件
	if (!order) //检查文件是否创建成功
	{
		cout << "error open" << endl;
	}

	for (i = cx0; i < cx; i++)
	{
		cout << mnemonic[code[i].f] << " " << code[i].l << " " << code[i].a << endl;//输出
		order << mnemonic[code[i].f] << " " << code[i].l << " " << code[i].a << endl;//写入order文件
	}
}

int block(int lev, int tx) {
	int dx;
	dx = 0;

	if (lev > levmax) {
		cout << "程序太长" << endl;
	}
	//判断是不是已经全局初始化
	bool flag = false;

	do {
		if (sym == _int || sym == _void) {
			if (sym == _int) {
				getsym();
				if (sym == ident) {
					getsym();
				}
				else {
					cout << "缺少变量或函数名" << endl;
				}
				//若int后声明的是函数
				if (sym == lparen) {
					//若未全局初始化，则进行全局初始化
					if (!flag) {
						gen(INT, 0, 1);
						gen(CAL, 0, 0);
						cm = cx - 1;
						gen(RET, 0, 0);
						rt = 0;
						flag = true;
					}
					lev++;
					gen(JMP, 0, cx + 1);
					enter(function, &tx, lev, &dx, _int);
					getsym();
					if (sym == rparen) {
						getsym();
						if (sym == lbrace) {
							//对函数内的语句进行处理
							statement(&tx, lev, true);
							if (sym == rbrace) {
								getsym();
							}
							else {
								cout << "缺少右花括号" << endl;
							}
						}
						else {
							cout << "缺少左花括号" << endl;
						}
					}
					else {
						cout << "缺少右括号";
					}
				}//若是多个变量
				else if (sym == comma) {
					vardeclaration(&tx, lev, &dx);
					while (sym == comma) {
						getsym();
						vardeclaration(&tx, lev, &dx);
						getsym();
					}
					if (sym == semicolon) {
						if (!flag) {
							gen(INT, 0, dx + 1);
							gen(CAL, 0, 0);
							cm = cx - 1;
							gen(RET, 0, 0);
							rt = dx;
							lev++;
							flag = true;
						}
						else {
							gen(INT, 0, dx);
							lev++;
						}
						getsym();
					}
					else {
						cout << "变量后缺少分号" << endl;
					}

				}//若是单个变量
				else if (sym == semicolon) {
					vardeclaration(&tx, lev, &dx);
					lev++;
					getsym();
				}
				else {
					cout << "变量后缺少分号" << endl;
				}
			}
			else if (sym == _void) {
				//若未全局初始化，则进行全局初始化
				if (!flag) {
					gen(INT, 0, 1);
					gen(CAL, 0, 0);
					cm = cx - 1;
					gen(RET, 0, 0);
					rt = 0;
					flag = true;
				}
				lev++;
				getsym();
				if (sym == ident) {
					//判断是主函数就处理全局里主函数的跳转
					if (!isMain())
						gen(JMP, 0, cx + 1);
					else code[cm].a = cx;
					enter(function, &tx, lev, &dx, _void);
					getsym();
				}
				else {
					cout << "缺少变量或函数名" << endl;
				}
				if (sym == lparen) {
					getsym();
					if (sym == rparen) {
						getsym();
						if (sym == lbrace) {
							statement(&tx, lev, true);
							if (sym == rbrace) {
								getsym();
							}
							else {
								cout << "缺少右花括号" << endl;
							}
						}
						else {
							cout << "缺少左花括号" << endl;
						}
					}
					else {
						cout << "缺少右括号" << endl;
					}
				}
				else {
					cout << "缺少左括号" << endl;
				}
			}
		}
		else {
			cout << "缺少类型识别符" << endl;
		}

	} while (!isOver);//直到文件读完

	return 1;
}