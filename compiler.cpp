#include<iostream>
#include<string>
#include<stdio.h>
#include<fstream>
#include<algorithm>
#include "init.h"
using namespace std;
//��ʼ��
void init();
//��ȡһ����
int getch();
//����Ŀ�����
int gen(enum fct x, int y, int z);
//�ʷ���������ȡ�ʵ�����
int getsym();
//��������
int vardeclaration(int* ptx, int lev, int* pdx);
//���ұ��������ڲ��ڱ��м��ڱ��е�λ��
int position(string idt, int tx);
//���
int term(int* ptx, int lev);
//���ʽ����
int expression(int* ptx, int lev);
//���Ӵ���
int factor(int* ptx, int lev);
//�����������������ֱ���
void enter(enum symbol k, int* ptx, int lev, int* pdx, int type);
//������
void error(int n);
//��䴦��
void statement(int* ptx, int lev, bool isOut);
//��ʾĿ����벢���䱣�浽�ļ�
void listcode(int cx0);
//�����������������
int block(int lev, int tx);
//�ж��ǲ���������
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
	//���õ��ַ�����
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

	//Ϊ��ʹ�ö��ֶԹؼ��ʽ�������
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

	//��ȡԴ�����ļ�
	string path;
	cout << "�������ļ���:" << endl;

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
		//�ļ��Ѿ�����
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
		//��ȡһ��
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
		cout << "�������" << endl;
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
	//���Կո��е�
	while (ch == ' ' || ch == 10 || ch == 9)
	{
		getch();
	}
	//����Ǳ��������������»��߻���ĸ��ͷ��
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
				cout << "��ʶ������" << endl;
			}
			getch();
		} while (ch >= 'a' && ch <= 'z' || ch >= '0' && ch <= '9' || ch == '_' || ch >= 'A' && ch <= 'Z');
		//���԰�����ĸ���»��ߺ�����
		a[k] = 0;
		strcpy_s(id, a);
		//��ʼ���ж��ֲ��ң��鿴�ǲ��ǹؼ���
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
		//��������ҵ������ǹؼ���
		if (i - 1 > j)
		{
			sym = wsym[k];
		}//�Ҳ������Ǳ�ʶ��
		else
		{
			sym = ident;
		}
	}//���������
	else if (ch > '0' && ch <= '9')
	{
		k = 0;//��¼���ֵ�λ��
		num = 0;
		sym = number;
		do
		{
			num = 10 * num + ch - '0';
			k++;
			getch();
		} while (ch >= '0' && ch <= '9');
		k--;
		//����̫���Ļ�
		if (k > nmax)
		{
			//error(30);
			cout << "����̫��" << endl;
		}
	}//��ֵ��
	else if (ch == '=')
	{
		sym = becomes;
		getch();
	}//��������������
	else
	{
		sym = ssym[ch];
		if (sym != period)
		{
			getch();
		}
	}
	//�����ǿ�ʶ�����
	return 0;
}

int vardeclaration(int* ptx, int lev, int* pdx)//������������
{
	//�������������ֱ���
	enter(_int, ptx, lev, pdx, _int);

	return 0;
}

int position(string idt, int tx) {
	int i;
	//�����ڱ�����ɨ�����
	table[0].name = idt;
	i = tx;
	while (table[i].name != idt) {
		i--;
	}
	return i;
}

int term(int* ptx, int lev)//���
{
	enum symbol mulop; //����˳���
	factor(ptx, lev);
	while (sym == times || sym == slash)
	{
		mulop = sym;
		getsym();
		factor(ptx, lev);
		if (mulop == times)
		{
			gen(MUL, 0, 0);//���ɳ˷�ָ��
		}
		else
		{
			gen(DIV, 0, 0);//���ɳ���ָ��
		}
	}
	return 0;
}

int expression(int* ptx, int lev)//���ʽ����
{
	enum symbol addop;//���ڱ���������
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
			gen(ADD, 0, 0);//���ɼӷ�ָ��
		}
		else
		{
			gen(SUB, 0, 0);//���ɼ���ָ��
		}
	}
	if (isNeg)
	{
		gen(SUB, 0, 0);
	}
	return 0;
}

int factor(int* ptx, int lev)//���Ӵ���
{
	int i;
	if (sym == ident)//���ӱ�������
	{
		i = position(id, *ptx);//��������
		if (i == 0)//����0��û�ҵ�
		{
			cout << "��ʶ��δ����" << endl;
		}
		else
		{
			int adr = 0;
			switch (table[i].kind)
			{
			case _int://����Ϊ����
				//�ж��ǲ���ȫ��
				if (table[i].level == 0) adr = 1;
				gen(LOD, adr, table[i].addr);//ֱ�Ӱѳ�����ջ
				break;
			case function:
				getsym();
				if (sym != lparen) {
					cout << "��������ȱ��������" << endl;
				}
				getsym();
				if (sym != rparen) {
					cout << "��������ȱ��������" << endl;
				}
				//���ú���
				gen(CAL, 0, table[i].addr);
				//�������з���ֵ����ȡ������ֵ
				if (table[i].type == _int)
					gen(LOD, 1, rt);
			}
		}
		getsym();
	}
	else if (sym == number)//����Ϊ��
	{
		//��������ջ
		gen(LIT, 0, num);
		getsym();
	}
	else if (sym == lparen)//����Ϊ���ʽ
	{
		getsym();
		//���б��ʽ����
		expression(ptx, lev);
		if (sym == rparen)
		{
			getsym();
		}
		else
		{
			cout << "ȱ��������" << endl;
		}
	}
	return 0;
}

void enter(enum symbol k, int* ptx, int lev, int* pdx, int type) {
	int i = position(id, *ptx);
	if (i != 0) {
		if (table[i].level == lev) {
			cout << "��������" << id << "�ض���" << endl;
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
	//��������
	if (sym == _int) {
		int dx = 0;

		getsym();
		if (sym == ident) {
			getsym();
		}
		else {
			cout << "ȱ�ٱ�����" << endl;
		}
		//������','�����Ƕ����
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
				cout << "ȱ�ٷֺ�" << endl;
			}
		}//������
		else if (sym == semicolon) {
			vardeclaration(ptx, lev, &dx);
			gen(INT, 0, 1);
		}
		else {
			cout << "ȱ�ٷֺ�" << endl;
		}
	}//��ʶ������
	else if (sym == ident) {
		bool isBecome = false;
		i = position(id, *ptx);
		if (i == 0)
		{
			cout << "��������δ����" << endl;
		}
		else {
			//���Ϊ��������Ϊ��ֵ
			if (table[i].kind == _int)
			{
				getsym();
				if (sym == becomes) {
					isBecome = true;
					getsym();
				}
				else {
					cout << "δ��⵽�Ⱥ�" << endl;
				}
			}
			//����ֱ�ӽ��б��ʽ����
			expression(ptx, lev);
			//����Ǹ�ֵ,��ֵ��������
			if (i != 0 && isBecome) {
				gen(STO, lev - table[i].level, table[i].addr);
			}
		}
	}//���봦��
	else if (sym == scanfsym) {
		getsym();
		if (sym != lparen)
		{
			cout << "ȱ��������" << endl;
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
				cout << "����δ����" << endl;
			}
			else if (i == -1) {
				cout << "ȱ�ٱ�����" << endl;
			}
			else {
				//��ʼ��ȡ����
				gen(RED, 0, 0);
				gen(STO, lev - table[i].level, table[i].addr);
			}
			getsym();
			if (sym != rparen) {
				cout << "ȱ��������" << endl;
			}
			else {
				getsym();
			}
		}
	}//�������
	else if (sym == printfsym) {
		getsym();
		if (sym == lparen) {
			getsym();
			if (sym != ident) {
				cout << "ȱ�ٱ�����" << endl;
			}
			//���б��ʽ����
			expression(ptx, lev);
			//��ʼ���
			gen(WRT, 0, 0);
			if (sym != rparen) {
				cout << "ȱ��������" << endl;
			}
			else {
				getsym();
			}
		}
		else {
			cout << "ȱ��������" << endl;
		}
	}//if���
	else if (sym == ifsym) {
		getsym();
		if (sym == lparen) {
			getsym();
			if (sym != ident) {
				cout << "ȱ�ٱ�����" << endl;
			}
			expression(ptx, lev);
			if (sym != rparen) {
				cout << "ȱ��������" << endl;
			}
			else {
				getsym();
				//������Ҫ��ת������ַ
				cx1 = cx;
				jpc = cx;
				gen(JPC, 0, 0);
				//�ж�if���Ƿ����return��䣬��������ɺ���Ҫ������ת
				hasReturn = false;
				statement(ptx, lev, false);
				//��û��return����ɺ���Ҫ��ת��if-else��������֮��
				if (!hasReturn) {
					//����elseif������
					ifcx[ifcount++] = cx;
					//ÿ��else-if��ɺ���Ҫ��ת��if-else��������֮��
					gen(JMP, 0, 0);
				}
				//������������ת���ĵ�ַ
				code[cx1].a = cx;
			}
		}
		else {
			cout << "ȱ��������" << endl;
		}
		//��¼��һ����if���
		isIf = true;
	}//else��䴦��
	else if (sym == elsesym) {
		getsym();
		statement(ptx, lev, false);
		//����ÿ��if�־�Ľ�����ת
		for (int i = 0; i < ifcount; i++) {
			code[ifcx[i]].a = cx;
		}
		ifcount = 0;

	}
	//while���
	else if (sym == whilesym) {
		//���濪ʼ�ĵ�ַ���Ա�����������������
		cx1 = cx;
		getsym();
		if (sym == lparen) {
			getsym();
			if (sym != ident) {
				cout << "ȱ�ٱ�����" << endl;
			}
			expression(ptx, lev);
			//������Ҫ��ת������ַ
			cx2 = cx;
			gen(JPC, 0, 0);
			if (sym != rparen) {
				cout << "ȱ��������" << endl;
			}
			else {
				getsym();
				statement(ptx, lev, false);
				//������������
				gen(JMP, 0, cx1);
				//��������������������ַ
				code[cx2].a = cx;
			}
		}
		else {
			cout << "ȱ��������" << endl;
		}
	}//�������
	else if (sym == lbrace) {
		//��¼Ƕ�״���
		lp++;
		//���̸�����䴦��
		getsym();
		statement(ptx, lev, false);
		while (sym == semicolon || (lp != 0 && isOut)) {
			getsym();
			//���if����else��if����Ҫ��ת
			if (isIf && sym != elsesym && isOut) {
				cx--;
				code[jpc].a--;
				ifcount = 0;
			}
			isIf = false;
			//һ��Ƕ�׽���
			if (sym == rbrace) lp--;
			statement(ptx, lev, false);
		}
		//��������޷���ֵ��δ��ʽд��return
		if (!hasReturn && isOut) {
			gen(RET, 0, 0);
		}
	}
	else if (sym == returnsym) {
		getsym();
		hasReturn = true;
		//�޷���ֵ
		if (sym == semicolon) {
			gen(RET, 0, 0);
		}//�з���ֵ
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
			cout << "return��ȱ�ٱ�����ֺ�" << endl;
		}
	}
}

void listcode(int cx0)//���Ŀ����벢д���ļ�
{
	int i;
	order.open("orders.txt", ios::out);  //����һ���ļ�
	if (!order) //����ļ��Ƿ񴴽��ɹ�
	{
		cout << "error open" << endl;
	}

	for (i = cx0; i < cx; i++)
	{
		cout << mnemonic[code[i].f] << " " << code[i].l << " " << code[i].a << endl;//���
		order << mnemonic[code[i].f] << " " << code[i].l << " " << code[i].a << endl;//д��order�ļ�
	}
}

int block(int lev, int tx) {
	int dx;
	dx = 0;

	if (lev > levmax) {
		cout << "����̫��" << endl;
	}
	//�ж��ǲ����Ѿ�ȫ�ֳ�ʼ��
	bool flag = false;

	do {
		if (sym == _int || sym == _void) {
			if (sym == _int) {
				getsym();
				if (sym == ident) {
					getsym();
				}
				else {
					cout << "ȱ�ٱ���������" << endl;
				}
				//��int���������Ǻ���
				if (sym == lparen) {
					//��δȫ�ֳ�ʼ���������ȫ�ֳ�ʼ��
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
							//�Ժ����ڵ������д���
							statement(&tx, lev, true);
							if (sym == rbrace) {
								getsym();
							}
							else {
								cout << "ȱ���һ�����" << endl;
							}
						}
						else {
							cout << "ȱ��������" << endl;
						}
					}
					else {
						cout << "ȱ��������";
					}
				}//���Ƕ������
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
						cout << "������ȱ�ٷֺ�" << endl;
					}

				}//���ǵ�������
				else if (sym == semicolon) {
					vardeclaration(&tx, lev, &dx);
					lev++;
					getsym();
				}
				else {
					cout << "������ȱ�ٷֺ�" << endl;
				}
			}
			else if (sym == _void) {
				//��δȫ�ֳ�ʼ���������ȫ�ֳ�ʼ��
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
					//�ж����������ʹ���ȫ��������������ת
					if (!isMain())
						gen(JMP, 0, cx + 1);
					else code[cm].a = cx;
					enter(function, &tx, lev, &dx, _void);
					getsym();
				}
				else {
					cout << "ȱ�ٱ���������" << endl;
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
								cout << "ȱ���һ�����" << endl;
							}
						}
						else {
							cout << "ȱ��������" << endl;
						}
					}
					else {
						cout << "ȱ��������" << endl;
					}
				}
				else {
					cout << "ȱ��������" << endl;
				}
			}
		}
		else {
			cout << "ȱ������ʶ���" << endl;
		}

	} while (!isOver);//ֱ���ļ�����

	return 1;
}