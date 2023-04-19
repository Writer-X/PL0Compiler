//-------------------------------
//语法分析程序
//CREATED BY TONG
//--------------------------------

#include <bits/stdc++.h>
//#include "pretty.cpp"
using namespace std;

int dx = 3;
int tempFatherList[5] = {-1,-1,-1,-1,-1};
int nowProcList[5] = {-1,-1,-1,-1,-1};
int nowProc = -1;
vector<string> mainCall;
bool isMainCall = true;


//名字表的结构体
struct symble
{
	string name;
	string kind;
	int addr;
	int attribute;//val/value  
    vector<int> nulladdr;

    vector<string> cancall;
    string father;

    symble(){}
    symble(string str1, string str2)
	{
		name = str1, kind = str2;
	}
};
vector<symble *> table;
//symble table[100000];
int tableId = 0;//当前名字表的下标
//在符号表里查找某个标识符
int findSymble(string str)
{
	for (int i = 0; i < tableId; i++)
	{
		if (table[i]->name == str)
			return i;
	}
	return -1;
}

//在符号表里查找某个标识符
int findSymble2(string str, int lev)
{
	for (int i = tableId - 1; i >=0; i--)
	{
		if (table[i]->name == str && (table[i]->attribute <= lev || table[i]->kind == "const") )
			return i;
	}
	return -1;
}
//目标指令的结构体：
struct TargetCode
{
	string funcCode;
	int levelDiff;
	int displacement;
};
vector<TargetCode> code;
int codeId = 0;

//添加code
void codeAdd(string str, int lev, int place)
{
    TargetCode t;
    t.funcCode = str;
    t.levelDiff = lev;
    t.displacement = place;
    code.push_back(t);
	codeId++;
}

//把字符串转化为数字
int strToint(string str)
{
	int len = str.length();
	int count = 0;
	for (int i = 0; i < len; i++)
	{
		if (str.at(i) <= 57 && str.at(i) >= 48)
			count = count + (str.at(i) - 48)*pow(10, len - i - 1);
		else
			return -1;
	}
	return count;
}


//-------------------------------
// 打印符号表
//-------------------------------
void PrintInCode()
{
    freopen("program.code","w",stdout);
    for(int i = 0; i < codeId; i++)
    {
        cout << code[i].funcCode << " " << code[i].levelDiff << " " << code[i].displacement << endl;
    }    
    freopen("CON","w",stdout);
}

//-------------------------
//打印目标代码
//-------------------------
void PrintTargerCode()
{
    cout << "TargerCode    f     l      d" << endl;
    for(int i = 0; i < codeId; i++)
    {
        cout << i << "           " << code[i].funcCode << "     " << code[i].levelDiff << "     " << code[i].displacement << endl;
    }
}

//--------------------------------------
//常量声明
//--------------------------------------
//关键字表
string KeywordList[13] = {"CONST","VAR","PROCEDURE","BEGIN","END",
    "ODD","IF","THEN","CALL","WHILE","DO","READ","WRITE"};
//操作符表
string OperatorList[11] = {"=",":=","+","-","*","/","#","<",
    "<=",">",">="};
//分隔符表
char SeparatorList[5] = {';',',','.','(',')'};

//-----------------------------------------
//变量声明
//-----------------------------------------

//词法分析结果字符流
string result;
//是否词法分析错误
bool LexicalError = false;

//--------------------------------------
//封装函数使用
//--------------------------------------

/*判断是否是A-Z或者a-z*/
bool IsLetter(char c)
{return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));}

/*判断是否是0-9*/
bool IsDigit(char c)
{return (c >= '0' && c <= '9');}

/*判断是否是操作符*/
bool IsOperator(string c)
{
	for(int i = 0; i < 11; i++)
		if(c == OperatorList[i]) return true;
	return false;
}

/*判断是否是分割符*/
bool IsSeparator(char c)
{
	for(int i = 0; i < 5; i++)
		if(c == SeparatorList[i]) return true;
	return false;
}

/*判断是否是关键字*/
bool IsKeyword(string c)
{
	for(int i = 0; i < 13; i++)
	    if(c == KeywordList[i]) return true;
	return false;
}

//--------------------------------
//词法分析具体函数
//---------------------------------

/*截取字符串,保存词语，返回结束位置*/
/* s : 输入字符串， i ：开始查找位置 */
int Split(string s, int i)
{
	int l = i, r = i;
	char c = s[i];
	while(IsLetter(c) || IsDigit(c))  //如果是字符或者数字的话即一直截取。
	{
		if(c >= 'a' && c <= 'z') s[r] = s[r] - 32; //如果是小写字符则转大写
		r++;
		c = s[r];
	}
	string word = s.substr(l,r - l);   //word存放截取结果

	c = word[0];   
	if(IsDigit(c)) //判断开头为数字
	{
		for(int i = 1; i < word.length(); i++)   //检验剩余部分是否全为数字
		{
			char temp = word[i];
			if(!IsDigit(temp))
			{
				LexicalError = true;
				return -1;
			}
		}
		int p = 0;  //第一个不为0的数字的位置
		for(int i = 0; i < word.length() - 1; i++)  //删除数字前多余的0
		{
			if(word[p] == '0') p++;
			else break;
		}
		string num = word.substr(p);
		result += "NUMBER ";
		result += num;
		result += '\n';
	}
	else   //此时开头为字母
	{
		if(IsKeyword(word))   //判断是关键字
		{
			result += word;
			result += '\n';
		}
		else   //不是关键字，即为标识符
		{
			if(word.length() > 10)   //如果长度超过10
			{
				LexicalError = true;
				return -1;
			}
			result += "IDENTIFIER ";
			result += word;
			result += '\n';
		}
	}
	return r - l;
}
/*词法分析程序结果输出*/
/*return result：语法分析结果词语流|语法分析错误*/
string Lexical_answer(string filename)   //词法分析输出词语流接口
{
	string s;
    //freopen(filename.c_str(), "r", stdin);
	while(cin >> s)
	{
		for(int i = 0; i < s.length(); i++)
		{
			char c = s[i]; string s1 = s.substr(i,1); 

			string s2;
			if(i < s.length() - 1)  s2 = s.substr(i,2);  //如果指针不在输入字符串的最后一个位置

			if(IsLetter(c) || IsDigit(c))   //判断是否是数字或者字母，是的话进行截取保存。
			{
				int to = Split(s,i);  
				if(to == -1) break; //如果词法分析错误直接跳出
				i = i + to - 1;   //保证下一次循环从该单词的下一个位置开始
			}
			else if (IsSeparator(c)) //判断是分割符，直接处理
			{
				result += c;
				result += '\n';
			}
			else if(IsOperator(s1) || IsOperator(s2))  //判断是操作符
			{
				if(IsOperator(s2))   //先判断两个符号的操作符
				{
					result += s2;
					result += '\n';
					i++;
				}
				else  //再判断一个符号的分割符
				{
					result += s1;
					result += '\n';
				}
			}
			else
			{
				LexicalError = true;
				break;  //如果词法分析错误跳出
			}
		}
	}
	//freopen("CON", "r", stdin);  //重定向回屏幕
    if(LexicalError) //如果语法分析错误，输出语法分析错误，终止程序
	{
        cout << "Lexical Error";
        exit(-1);
	}
	return result;	
}

//--------------------------------------
//常量声明
//----------------------------------------
/*字符串常量声明部分*/
string StringConstant[23] = {"PROGRAM","SUBPROG","CONSTANTDECLARE",
    "CONSTANTDEFINE","VARIABLEDECLARE","PROCEDUREDECLARE","PROCEDUREHEAD",
    "SENTENCE","ASSIGNMENT","COMBINED","CONDITION","EXPRESSION","ITEM",
    "FACTOR","IFSENTENCE","CALLSENTENCE","WHILESENTENCE","READSENTENCE",
    "WRITESENTENCE","EMPTY","LP","RP","COMMA"};

//--------------------------------------------
//结构体定义
//--------------------------------------------
/*语法树节点*/
struct SyntaxTreeNode
{
    string content;   //内容
    int childNumber;  //子节点数目
    vector<SyntaxTreeNode *> children;
    SyntaxTreeNode *father;  //记录父亲节点
    SyntaxTreeNode()   //构造函数,初始化为空字符串即可
    {
       content = "";
       childNumber = 0;
    }
    SyntaxTreeNode(string c)   //复制构造函数
    {
        content = c;
        childNumber = 0;
    }
};

/*语法树类型*/
struct SyntaxTree
{
    SyntaxTreeNode *root;
    //int count; 
};

//------------------------------------------------------
//变量声明
//----------------------------------------------------
bool SyntaxError = false;      //判断语法分析失败
int procedurdeclare_num = 0;   //过程说明层数
SyntaxTree syntaxTree;         //语法树
string s;    //词法分析字符串
int l,oldl;  //字符串扫描位置，上次扫描位置
string pretty_result; //规范输出结果
bool issemicolon = true;//判断复合语句中是否没有语句

//---------------------------------------------------
//通用函数封装
//------------------------------------------------

/*语法分析报错*/
void PrintSyntaxError()
{
	SyntaxError = true;
	printf("Syntax Error\n");
	exit(-1);
}

/*字符串分割函数*/
string Cut()   //注意为&l
{
    if(l >= s.length())//如果当前位置超过词语流长度，则输出语法分析错误，并结束程序
    {
        PrintSyntaxError();
    }
    int i = l;
    for(; i < s.length(); i++)    //从左侧开始查找，到回车符或者空白符停止,截取词语进行匹配
    {
        if(s[i] == '\n' || s[i] == ' ') break;
    }
    string c = s.substr(l,i - l);  
    oldl = l;  //更新词语流扫描位置
    l = i + 1;   
    return c;
}

/*字符串多截取后的恢复函数*/
void Recover()
{
    l = oldl;
}

/*语法树节点添加孩子节点*/
void AddChildNode(SyntaxTreeNode *fatherNode,SyntaxTreeNode *childNode)
{
    fatherNode->children.push_back(childNode);
    fatherNode->childNumber++;
    childNode->father = fatherNode;
}   

/*输出树，并添加括号*/
void PrintSyntaxTree(SyntaxTreeNode *node)
{
    cout << node->content;
    pretty_result += node->content;
    if(node->childNumber > 0)  //如果存在孩子节点
    {
        cout << "(";
        pretty_result += "(";
        PrintSyntaxTree(node->children[0]); //迭代输出孩子节点
        for(int i = 1; i < node->childNumber; i++)  
        {
            cout << ","; //孩子与孩子之间输出，
            pretty_result += ",";
            PrintSyntaxTree(node->children[i]);
        }
        cout << ")";        
        pretty_result += ")";
    }
}

/*添加string类型的孩子节点*/
/*@string :要添加的孩子节点的context*/
void AddStringChild(SyntaxTreeNode *fnode,string c)
{
    SyntaxTreeNode *node = new SyntaxTreeNode(c);   //分程序节点，为root的孩子节点
    AddChildNode(fnode,node);
}
//-------------------------------------------------
//语法分析程序具体函数
//--------------------------------------------------

//------------------------------------------
//后位置使用函数声明，无特殊意义
//------------------------------------------
void sentence(SyntaxTreeNode *fnode, string c);
void subprog(SyntaxTreeNode *fnode);  
void expression(SyntaxTreeNode *fnode);

/*具体常量判断*/
void specific_constantdefine(SyntaxTreeNode *fnode)
{
    string c = Cut();  //首先截取的应该为IDENTIFIER
    if(c != "IDENTIFIER")  
    {
        PrintSyntaxError();
    }
    c = Cut(); //标识符
    AddStringChild(fnode,c);
    symble *s = new symble(c,"const");

    for(int i = 0; i < tableId; i++)
    {
        if(table[i]->name == c)
        {
            cout << "redefine" <<endl;
            exit(-1);
        }
    }

    c = Cut(); //=号的添加
    if(c != "=")
    { 
        PrintSyntaxError();
    }
    AddStringChild(fnode,c);

    c = Cut(); //此处截取的应该Number
    if(c != "NUMBER")  
    {
        PrintSyntaxError();
    }
    c = Cut(); //数字
    AddStringChild(fnode,c);

    s->attribute = strToint(c);//把string给转换成int
    table.push_back(s);
	tableId++;//上面的操作中，只差一个地址没有填，其他的全部填写了
}

/*常量定义部分*/
void constantdefine(SyntaxTreeNode *fnode) //常量定义部分
{
    SyntaxTreeNode *node = new SyntaxTreeNode(StringConstant[3]);   //常量定义节点
    AddChildNode(fnode,node);
    
    specific_constantdefine(node);  //具体常量定义

    string c = Cut();   //截取分割符号
    if(c == ",")
    {
        AddStringChild(fnode,"COMMA");    //添加，的文字形式COMMA
        constantdefine(fnode);  //继续添加具体常量定义
    }
    else if(c == ";")  //如果是分号
    {
        node = new SyntaxTreeNode(c);   
        AddChildNode(fnode,node);        
    }
    else
    {
        PrintSyntaxError();
    }
}

/*常量说明部分*/
void constantdeclare(SyntaxTreeNode *fnode)  
{
    AddStringChild(fnode,"CONST");
    constantdefine(fnode);
}

/*具体变量说明部分*/
void specific_variabledeclare(SyntaxTreeNode *fnode)
{
    string c = Cut();  //首先截取的应该为IDENTIFIER
    if(c != "IDENTIFIER")  
    {
        PrintSyntaxError();
    }
    c = Cut(); //标识符
    AddStringChild(fnode,c);
    
    symble *s = new symble(c,"var");
	s->attribute = procedurdeclare_num;//后面的这个参数是记录当前的过程层数
	s->addr = dx;//当前层数的偏移地址

    string fatherName;
    if(procedurdeclare_num == 0)
    {
        fatherName = "main";
    }
    else
    {
        int tempFather = tempFatherList[procedurdeclare_num];
        fatherName = table[tempFather]->name;
    }
    s->father = fatherName;
    for(int i = 0; i < tableId; i++)
    {
        if(table[i]->name == c && table[i]->attribute == procedurdeclare_num && table[i]->father == fatherName)
        {
            cout << c << "redefine" <<endl;
            exit(-1);
        }
    }

    table.push_back(s);
    
	tableId++;
	dx++;

    c = Cut();  //再截取的应该为，或者；
    if(c == ",")
    {
        AddStringChild(fnode,"COMMA");
        specific_variabledeclare(fnode);//再添加一个变量
    }
    else if(c == ";")
    {
        AddStringChild(fnode,c);       
    }
    else
    {
       PrintSyntaxError();
    }
}

/*变量说明部分VARIABLEDECLARE*/
void variabledeclare(SyntaxTreeNode *fnode)  
{
    AddStringChild(fnode,"VAR");
    specific_variabledeclare(fnode);
}



/*过程首部判断*/
void specific_procedurehead(SyntaxTreeNode *fnode)
{
    SyntaxTreeNode *node = new SyntaxTreeNode("PROCEDURE");   //PROCEDUCER符号添加
    AddChildNode(fnode,node);       

    string c = Cut();   //标识符IDENTIFIER截取
    if(c == "IDENTIFIER")  
    {
        c = Cut();
        AddStringChild(fnode,c);

        if(isMainCall) mainCall.push_back(c);

        symble *s = new symble(c,"proc");
		s->addr = -1;
		s->attribute = procedurdeclare_num - 1;

        //先声明的同层次的函数
        for(int i = 0; i <tableId; i++)
        {
            if(table[i]->attribute ==  procedurdeclare_num - 1 && table[i]->kind == "proc")
            {
                s->cancall.push_back(table[i]->name);
                //cout <<s->name << " push " << table[i]->name << endl;
            }
        }
        //自己
        s->cancall.push_back(s->name);
        //cout <<s->name << " push " << s->name << endl;
        //包含自己的父函数
        for(int i = 0; i <= procedurdeclare_num - 1; i++)
        {
            int tempFather = tempFatherList[i];
            if(tempFather != -1)
            {
                s->cancall.push_back(table[tempFather]->name);
                //cout <<s->name << " push " << table[tempFather]->name << endl;
            }
        }

        //父函数也加入当前
        int tempFather = tempFatherList[procedurdeclare_num - 1];

        if(tempFather != -1)
        {
            table[tempFather]->cancall.push_back(s->name);
            //cout  << table[tempFather]->name<< " push " << s->name  << endl;
        }
        table.push_back(s);
        //下一层父函数赋值
        tempFatherList[procedurdeclare_num] = tableId;
		tableId++;
    }
    else
    {
        PrintSyntaxError();
    }

    c = Cut(); //;分割符截取
    if(c == ";")
    {
        AddStringChild(fnode,c);      
    }
    else
    {
        PrintSyntaxError();
    }
}

/*过程首部*/
void procedurehead(SyntaxTreeNode *fnode)
{
    SyntaxTreeNode *node = new SyntaxTreeNode(StringConstant[6]);   //过程说明部分
    AddChildNode(fnode,node);  

    specific_procedurehead(node);  //过程首部判断调用
}

/*过程说明部分*/
void proceduredeclare(SyntaxTreeNode *fnode)
{
    SyntaxTreeNode *node = new SyntaxTreeNode(StringConstant[5]);   //过程说明部分
    AddChildNode(fnode,node);   
    procedurehead(node); //过程首部部分调用
    subprog(node);  //分程序部分
    string c = Cut();   //判断之后是否还是过程说明部分,可能不是
    if(c == "PROCEDURE")
    {
        proceduredeclare(node); //过程说明部分
    }     
    else
    {
        Recover();
    }
}
/*因子具体判断*/
void specific_factor(SyntaxTreeNode *fnode,string c)
{
   if(c == "IDENTIFIER")   //标识符
    {
        c = Cut(); 
        AddStringChild(fnode,c);      

 		//判断标识符为变量还是常量
		int tableIndex = findSymble2(c,procedurdeclare_num);//这是到table里面找，去找有没有这个名字
		if (tableIndex != -1)
		{
			if (table[tableIndex]->kind == "const")
			{
				codeAdd("lit", 0, table[tableIndex]->attribute);
			}
			else if (table[tableIndex]->kind == "var")
			{
				codeAdd("lod", abs(table[tableIndex]->attribute - procedurdeclare_num), table[tableIndex]->addr);
			}
            else if(table[tableIndex]->kind == "proc")
            {
                exit(-1);
            }
		} 
        else
        {
            cout << "no value" << endl;
            exit(-1);
        }        
    }
    else if(c == "NUMBER")   //无符号整数
    {
        c = Cut();
        AddStringChild(fnode,c);   

        codeAdd("lit",0,strToint(c));
    }
    else if(c == "(")
    {
        AddStringChild(fnode,"LP");   

        expression(fnode);      

        c = Cut();
        if(c == ")")
        {
            AddStringChild(fnode,"RP");            
        }
        else
        {
            PrintSyntaxError();
        }
    }
    else
    {
        PrintSyntaxError();
    }
}

/*因子*/
void factor(SyntaxTreeNode *fnode,string c)
{
    SyntaxTreeNode *node = new SyntaxTreeNode(StringConstant[13]);      //FACTOR添加
    AddChildNode(fnode,node);   

    specific_factor(node,c);
}

/*项的判断具体*/
void specific_item(SyntaxTreeNode *fnode,string c)
{
    //此时应该调用因子的判断
    factor(fnode,c);
    c = Cut(); //此时再截取可能为乘除运算符,则后面还是因子
    if(c == "*" || c == "/")
    {
        string op = c;
        AddStringChild(fnode,c);     
        c = Cut();       
        specific_item(fnode,c);

        if(op == "*") codeAdd("opr", 0, 3);
        else codeAdd("opr", 0, 4);
    }
    else
    {
        Recover();
    }
}

/*项*/
void item(SyntaxTreeNode *fnode,string c)
{
    SyntaxTreeNode *node = new SyntaxTreeNode(StringConstant[12]);      //ITEM添加
    AddChildNode(fnode,node);   
    //项的具体判断
    specific_item(node,c);  
}
bool isFu = true;
/*表达式判断具体*/
void specific_expression(SyntaxTreeNode *fnode)
{
    string c = Cut();   //首先截取,这里可能会截取到；
    
    if(c == "+" || c == "-")   //如果是+-
    {
        //if(isFu) codeAdd("lit", 0, 0);
        string op = c; 
        SyntaxTreeNode *node = new SyntaxTreeNode(c);      //+-号添加
        AddChildNode(fnode,node);      
        c = Cut();

        if(c == "IDENTIFIER" || c == "NUMBER" || c == "(")   //项,此时截取的应该为标识符或者无符号数字或者(
        {
            item(fnode,c);//此时应该调用项的判断
            specific_expression(fnode); 
        }   
        if(op == "+") codeAdd("opr", 0, 1);
        else if(isFu == false)codeAdd("opr", 0, 2);
        else if(isFu == true) 
        {
            isFu = false;
            codeAdd("opr", 0, 14);
        }
    }
    else if(c == "IDENTIFIER" || c == "NUMBER" || c == "(")   //项,此时截取的应该为标识符或者无符号数字或者(
    {
        isFu = false;
        item(fnode,c);//此时应该调用项的判断
        specific_expression(fnode); 
    }    
    else
    {
        Recover();   //截取到其他符号，如；等恢复。
    }
}

/*表达式判断EXPRESSION*/
void expression(SyntaxTreeNode *fnode)
{
    SyntaxTreeNode *node = new SyntaxTreeNode(StringConstant[11]);      //EXPRESSION添加
    AddChildNode(fnode,node);   
    isFu = true;
    specific_expression(node);
}

/*赋值语句*/
void assignment(SyntaxTreeNode *fnode)
{
    string c = Cut(); //具体标识符添加
    SyntaxTreeNode *node = new SyntaxTreeNode(c);    
    AddChildNode(fnode,node);    

	int tableIndex = findSymble2(c,procedurdeclare_num);
	if (tableIndex == -1)
	{
		cout << "the valuable has not been defined" << endl;
		exit(-1);
	}
    else if(table[tableIndex]->kind == "const" || table[tableIndex]->kind == "proc")
    {
        cout << "Assign a value to a constant" << endl;
		exit(-1);
    }

    c = Cut();  //:=节点
    if(c == ":=")
    {
        node = new SyntaxTreeNode(c);    
        AddChildNode(fnode,node);         
    }
    else
    {
        PrintSyntaxError();
    }
    expression(fnode);     //此处应为表达式

    codeAdd("sto", abs(table[tableIndex]->attribute - procedurdeclare_num), table[tableIndex]->addr);
}

/*条件*/
void specific_condition(SyntaxTreeNode *fnode)
{
    string c = Cut();
    if(c == "ODD")
    {
        SyntaxTreeNode *node = new SyntaxTreeNode("ODD");   //赋值语句节点
        AddChildNode(fnode,node);       
        expression(fnode);   //表达式 

        codeAdd("opr",0,5);
    }
    else 
    {
        Recover();
        expression(fnode);   //表达式 
        c = Cut();
        if(c == "=" ||c == "#" || c == "<" || c == "<=" || c == ">" || c == ">=")
        {
            AddStringChild(fnode,c);
            expression(fnode);  

             /*关系运算目标代码添加*/
            if(c == "=") codeAdd("opr",0,6); 
            else if(c == "#") codeAdd("opr",0,7);
            else if(c == "<") codeAdd("opr",0,8);
            else if(c == "<=") codeAdd("opr",0,9);
            else if(c == ">") codeAdd("opr",0,10);
            else if(c == ">=") codeAdd("opr",0,11); 
        }
        else
        {
            PrintSyntaxError();
        }
    }
}

/*条件*/
void condition(SyntaxTreeNode *fnode)
{
    SyntaxTreeNode *node = new SyntaxTreeNode(StringConstant[10]);   //Condition节点
    AddChildNode(fnode,node);
    specific_condition(node);
}

/*条件语句*/
void ifsentence(SyntaxTreeNode * fnode)
{
    AddStringChild(fnode,"IF");      
    condition(fnode);   //条件
    
    codeAdd("jpc", 0, 0);//暂时先填着地址0
    int tempId = codeId - 1;

    string c = Cut();   //THEN截取
    if(c == "THEN")
    {
        AddStringChild(fnode,"THEN");
        c = Cut();   //语句
        sentence(fnode,c);       

        code[tempId].displacement = codeId; //回填之前的jpc
    }
    else
    {
        PrintSyntaxError();
    }
}

/*当型循环语句*/
void whilesentence(SyntaxTreeNode * fnode)
{
    int tempAddr = codeId;//while循环的入口地址

    AddStringChild(fnode,"WHILE");     
    condition(fnode);   //条件

    codeAdd("jpc", 0, 0);//暂时先填着地址0 
    int tempId = codeId-1;

    string c = Cut();   //THEN截取
    if(c == "DO")
    {
        AddStringChild(fnode,"DO");
        c = Cut();   //语句
        sentence(fnode,c);       

        codeAdd("jmp", 0, tempAddr);
		//回填之前的jpc
		code[tempId].displacement = codeId;
    }
    else
    {
        PrintSyntaxError();
    }
}



/*call语句*/
void callsentence(SyntaxTreeNode *fnode)
{
    AddStringChild(fnode,"CALL");
    string c = Cut();
    if(c == "IDENTIFIER")  
    {
        c = Cut();
        AddStringChild(fnode,c);

        //目标代码生成，过程调用
		int tempAddr = 0;
		int tempId = 0;
        int i = 0;
		for (; i < tableId; i++)
		{
			if (table[i]->name == c)
			{
				tempAddr = table[i]->addr;
				tempId = i;
				break;
			}
		}
        
        if(table[tempId]->kind == "const" || table[tempId]->kind == "var" || i == tableId)
        {
            cout << "nodefine"<< c << endl;
            exit(-1);
        }

        if(table[tempId]->kind == "proc")
        {
            if(nowProc == -1)
            {
                int num = mainCall.size();
                for(int i = 0; i < num; i++)
                {
                    if(table[tempId]->name == mainCall[i]) break;
                }
                if(i == num)
                {
                    cout << "main wrong call" << table[tempId]->name<<endl;
                    //exit(0);
                }
            }
            else
            {
                int num = table[nowProc]->cancall.size();
                for(int i = 0; i < num; i++)
                {
                    if(table[tempId]->name == table[i]->name) break;
                }
                if(i == num)
                {
                    cout << table[nowProc]->name <<"wrong call" <<table[tempId]->name <<endl;
                    exit(-1);
                }
            }
        }
        if (tempAddr == -1)
        {
            table[tempId]->nulladdr.push_back(codeId);
        }
		codeAdd("cal",abs(procedurdeclare_num-table[tempId]->attribute),tempAddr);
    
        nowProcList[procedurdeclare_num] = nowProc;   //当前层的nowProc保存
        nowProc = tempId;
        
    }
    else
    {
        PrintSyntaxError();
    }
}

/*读语句中具体判断*/
void specific_readsentence(SyntaxTreeNode *fnode)
{
    codeAdd("opr", 0, 12);

    string c = Cut();   
    if(c == "IDENTIFIER")  
    {
        c = Cut(); // 裁剪标识符
        AddStringChild(fnode,c); 

        int tableIndex = findSymble2(c,procedurdeclare_num);
		if (tableIndex == -1 || table[tableIndex]->kind == "const" || table[tableIndex]->kind == "proc" )
		{
			cout << "the valuable has not been defined" << endl;
			exit(-1);
		}
		codeAdd("sto", abs(table[tableIndex]->attribute - procedurdeclare_num), table[tableIndex]->addr);
    }
    else
    {
        PrintSyntaxError();
    }
    c = Cut();  //裁剪为，或者）
    if(c == ",")   //如果是，
    {
        AddStringChild(fnode,"COMMA");
        specific_readsentence(fnode);
    }  
    else if(c == ")")   //如果是)
    {
        AddStringChild(fnode,"RP");
    }
    else
    {
        PrintSyntaxError();
    }
}

/*读语句*/
void readsentence(SyntaxTreeNode *fnode)
{
    SyntaxTreeNode *node = new SyntaxTreeNode("READ"); //READ    
    AddChildNode(fnode,node);  

    string c = Cut();   //裁剪左括号并添加
    if(c == "(")  
    {
        node = new SyntaxTreeNode("LP"); 
        AddChildNode(fnode,node);  
        specific_readsentence(fnode);
    }
    else
    {
        PrintSyntaxError();
    }
}

/*写语句中具体判断*/
void specific_writesentence(SyntaxTreeNode *fnode)
{
    string c = Cut();   
    if(c == "IDENTIFIER")  
    {
        c = Cut(); // 裁剪标识符
        AddStringChild(fnode,c); 

        int tableIndex = findSymble2(c,procedurdeclare_num);
		if (tableIndex == -1 || table[tableIndex]->kind == "proc" )
		{
			cout << "write the valueable has not been defined" << endl;
			exit(-1);
		}
        if(table[tableIndex]->kind == "const") codeAdd("lit", 0, table[tableIndex]->attribute);
		else codeAdd("lod", abs(table[tableIndex]->attribute - procedurdeclare_num), table[tableIndex]->addr);
        codeAdd("opr", 0, 13);
    }
    else
    {
        PrintSyntaxError();
    }
    c = Cut();  //裁剪为，或者）
    if(c == ",")   //如果是，
    {
        AddStringChild(fnode,"COMMA");
        specific_writesentence(fnode);
    }  
    else if(c == ")")   //如果是)
    {
        AddStringChild(fnode,"RP");
    }
    else
    {
        PrintSyntaxError();
    }
}


/*写语句*/
void writesentence(SyntaxTreeNode *fnode)
{
    SyntaxTreeNode *node = new SyntaxTreeNode("WRITE"); //write    
    AddChildNode(fnode,node);  

    string c = Cut();   //裁剪左括号并添加
    if(c == "(")  
    {
        node = new SyntaxTreeNode("LP"); 
        AddChildNode(fnode,node);  
        specific_writesentence(fnode);
    }
    else
    {
        PrintSyntaxError();
    }
}

void specific_combined(SyntaxTreeNode *fnode)
{
    string c = Cut();
    if(c == "IDENTIFIER" || c == "IF" || c == "WHILE" || c == "CALL" || c == "READ" || c == "WRITE" || c == "BEGIN" || c == "")  //如果是语句
    {
        sentence(fnode,c);
        c = Cut();
        if(c == ";")
        {
            AddStringChild(fnode,";");   
            issemicolon = true;
        }
        else
        {
            Recover();
            issemicolon = false;
        }
        specific_combined(fnode);  //下一条还是语句
    }  
    else if(c == "END") //直到碰到end
    {
        //前面有一条空语句
        if(issemicolon)
        {
            string cc = "";
            sentence(fnode,cc);
        }
        AddStringChild(fnode,"END");  
    }
    else if(c == ";")  //如果直接是；
    {
        sentence(fnode,"");  //添加一条空语句       
        AddStringChild(fnode,";");  
        specific_combined(fnode);
    }
    else
    {
        PrintSyntaxError();
    }
}

/*复合语句*/
void combined(SyntaxTreeNode *fnode)
{
    AddStringChild(fnode,"BEGIN");  
    specific_combined(fnode); 
}

/*具体为哪种语句
*c:截取的字符，用于判断是哪种语句
*/
void specific_sentence(SyntaxTreeNode *fnode, string c)
{
    if(c == "IDENTIFIER")   //此时为赋值语句
    {
        SyntaxTreeNode *node = new SyntaxTreeNode(StringConstant[8]);   //赋值语句节点
        AddChildNode(fnode,node);
        assignment(node);
    }
    else if(c == "IF")   //此时为条件语句
    {
        SyntaxTreeNode *node = new SyntaxTreeNode(StringConstant[14]);   //条件语句节点
        AddChildNode(fnode,node);         
        ifsentence(node);
    }
    else if(c == "WHILE") //此时为当型循环语句
    {
        SyntaxTreeNode *node = new SyntaxTreeNode(StringConstant[16]);   //当型循环语句
        AddChildNode(fnode,node);
        whilesentence(node);
    }
    else if(c == "CALL")  //此时为过程调用语句
    {
        SyntaxTreeNode *node = new SyntaxTreeNode(StringConstant[15]);   //call语句
        AddChildNode(fnode,node);
        callsentence(node);
    }
    else if(c == "READ") //此时为读语句
    {
        SyntaxTreeNode *node = new SyntaxTreeNode(StringConstant[17]);   //读语句节点
        AddChildNode(fnode,node);
        readsentence(node);
    }
    else if(c == "WRITE")  //此时为写语句
    {
        SyntaxTreeNode *node = new SyntaxTreeNode(StringConstant[18]);   //写语句节点
        AddChildNode(fnode,node);
        writesentence(node);
    }
    else if(c == "BEGIN") // 此时为复合语句
    {
        SyntaxTreeNode *node = new SyntaxTreeNode(StringConstant[9]);   //复合语句节点
        AddChildNode(fnode,node);
        combined(node);
    }
    else if(c == "")  //此时为空语句
    {
        SyntaxTreeNode *node = new SyntaxTreeNode(StringConstant[19]);   //空语句节点
        AddChildNode(fnode,node);
    }
    else
    {
        Recover();
    }
}

/*语句部分*/
void sentence(SyntaxTreeNode *fnode,string c)
{    
    SyntaxTreeNode *node = new SyntaxTreeNode(StringConstant[7]);   //sentence节点
    AddChildNode(fnode,node);    

    specific_sentence(node,c);  //哪种语句判断
}

/*分程序部分判断*/
void specific_subprog(SyntaxTreeNode *fnode)
{
    bool sentence_exist = false; //每个分程序中判断是否存在语句
    int tempTableId = tableId-1;//第一次到这个位置的时候，名字表的下标是0

    string c = Cut();
    if(c == "CONST")
    {
        SyntaxTreeNode *node = new SyntaxTreeNode(StringConstant[2]);   //常量说明部分节点
        AddChildNode(fnode,node);
        constantdeclare(node);//此处应进入常量说明部分
        c = Cut();
    }
    if(c == "VAR")
    {
        SyntaxTreeNode *node = new SyntaxTreeNode(StringConstant[4]);   //常量说明部分节点
        AddChildNode(fnode,node);
        variabledeclare(node);
        c = Cut();
    }    
    if(c == "PROCEDURE")
    {
        procedurdeclare_num++;
        if(procedurdeclare_num == 4)
        {
            PrintSyntaxError();
        }
        dx = 3;
        if(procedurdeclare_num > 1) isMainCall = false;
        proceduredeclare(fnode);       //过程说明部分
        procedurdeclare_num--; 
        if(procedurdeclare_num <= 1) isMainCall = true;
        c = Cut();  
    }
    if(tempTableId >= 0)
    {
        if (table[tempTableId]->kind == "proc") table[tempTableId]->addr = codeId;    
        if (table[tempTableId]->nulladdr.size() != 0)
        {
            for(int i = 0; i < table[tempTableId]->nulladdr.size() ;i++)
            {
                int tempId = table[tempTableId]->nulladdr[i];
                code[tempId].displacement = codeId;
            }
        }
    }
	//目标代码的生成，开辟地址空间
	int varNumber = 0;
	for (int i = 0; i < tableId; i++)
		if (table[i]->attribute == procedurdeclare_num && table[i]->kind == "var")
			varNumber++;
	codeAdd("int",0,varNumber+3);

    //标代码生成,直接跳转到主程序
	if (procedurdeclare_num == 0) code[0].displacement = codeId-1;
        
    //赋值语句 条件语句 当型循环语句 过程调用语句 读语句 写语句 复合语句 空语句
    if(c == "IDENTIFIER" || c == "IF" || c == "WHILE" || c == "CALL" || c == "READ" || c == "WRITE" || c == "BEGIN" || c == "")
    {
        sentence_exist = true;
        //此时应为语句的判断
        sentence(fnode,c);
    }
    else
    {
        Recover();
    }
    //本来应该有语句，如果没有语句，则说明应该有一条空语句
    if(sentence_exist == false)  
    {
        sentence(fnode,"");
    }
}

/*分程序部分*/
void subprog(SyntaxTreeNode *fnode)   
{   
    SyntaxTreeNode *node = new SyntaxTreeNode(StringConstant[1]);   //分程序节点，为root的孩子节点
    AddChildNode(fnode,node);
    specific_subprog(node);
    string c = Cut();   
    if(c == ".")   //分割符.
    {
        AddStringChild(fnode,c);  
        if(l < s.length())  //如果。后还有词语流
        {
            PrintSyntaxError();
        }
        codeAdd("opr", 0, 0);
    }
    else if (c == ";")
    {
        AddStringChild(fnode,c);    
        if(procedurdeclare_num == 0)
        {
            PrintSyntaxError();
        }
        nowProc = nowProcList[procedurdeclare_num];  //当前层proc恢复
        codeAdd("opr", 0, 0);
    }        
    else
    {
        PrintSyntaxError();
    }
}

/*程序部分*/
void program()
{
    codeAdd("jmp", 0, -1);
    syntaxTree.root = new SyntaxTreeNode(StringConstant[0]);   //根节点,为PROGRAM
    subprog(syntaxTree.root);
}

int main()
{
    code.clear();
    table.clear();

    string filename = "4";
    s = Lexical_answer(filename);    //词语流输入
    l = 0; //记录词语流扫描位置

    program();  

    vector<string> const1;
    vector<string> var1;
    vector<string> proc1;
    for(int i = 0; i < tableId; i++)
    {
        if(table[i]->kind == "const")
        {
            const1.push_back(table[i]->name);
        }
        else if(table[i]->kind == "var")
        {
            var1.push_back(table[i]->name);
        }
        else if(table[i]->kind == "proc")
        {
            proc1.push_back(table[i]->name);
        }
    }
    
    for(int i = 0 ; i < const1.size(); i++)
    {
        for(int j = 0; j < var1.size(); j++)
        {
            if(const1[i] == var1[j]) exit(-1);
        }
    }

    for(int i = 0 ; i < const1.size(); i++)
    {
        for(int j = 0; j < proc1.size(); j++)
        {
            if(const1[i] == proc1[j]) exit(-1);
        }
    }

    for(int i = 0 ; i < var1.size(); i++)
    {
        for(int j = 0; j < proc1.size(); j++)
        {
            if(var1[i] == proc1[j]) exit(-1);
        }
    }
//   freopen("spfaout", "w", stdout);
//   PrintSyntaxTree(syntaxTree.root);
//   cout << endl;
//   pretty(pretty_result);
//   PrintSymbolLable();
//   PrintTargerCode();
    PrintInCode();
    return 0;    
}
