#include<bits/stdc++.h>
using namespace std;

//目标指令的结构体：
struct TargetCode
{
	string funcCode;
	int levelDiff;
	int displacement;
};
vector<TargetCode> code;
int codeId = 0;
void getTargetCode()
{
    ifstream codein;
	codein.open("program.code");
	string s; int l, a;
	TargetCode t;
	while (codein >> s >> l >> a)
	{
		t.funcCode = s;
		t.levelDiff = l;
		t.displacement = a;
		code.push_back(t);
        codeId++;
	}
}

stack <int> stack1;
int moveId = 0;
int backId = 0;
int staticId = 0;
int actId = 0;
int cid = 0;  //代码段下标
vector<int> action1;

int base(TargetCode ir)  //递归查询动态链
{
    int l = ir.levelDiff;
    int newId = moveId;
    while (l > 0) {
        newId = action1[newId];
        l--;
    }
    return newId;
}

void opr(int f)
{
    switch(f)
    {
        case 0:  //返回退出
        {
            cid = action1[backId];
            moveId = action1[staticId];
            backId = moveId + 1;
            staticId = moveId + 2;
            cid--;
            break;
        }
        case 1:
        {
            int temp = stack1.top();
            stack1.pop();
            temp = temp + stack1.top();
            stack1.pop();
            stack1.push(temp);
            break;
        }
        case 2:
        {
            int temp = stack1.top();
            stack1.pop();
            temp = stack1.top() - temp;
            stack1.pop();
            stack1.push(temp);
            break;
        }
        case 3:
        {
            int temp = stack1.top();
            stack1.pop();
            temp = temp * stack1.top();
            stack1.pop();
            stack1.push(temp);
            break;
        }
        case 4:
        {
            int temp = stack1.top();
            stack1.pop();
            temp = stack1.top() / temp;
            stack1.pop();
            stack1.push(temp);
            break;
        }
        case 5:
        {
            int temp;
            if (stack1.top() % 2 == 1)
                temp = 1;
            else
                temp = 0;
            stack1.pop();
            stack1.push(temp);
            break;
        }
        case 6: //等于
        {
            int temp = stack1.top();
            stack1.pop();
            if (temp == stack1.top())
                temp = 1;
            else
                temp = 0;
            stack1.pop();
            stack1.push(temp);
            break;
        }
        case 7: //#号运算
        {
            int temp = stack1.top();
            stack1.pop();
            if (temp == stack1.top())
                temp = 0;
            else
                temp = 1;
            stack1.pop();
            stack1.push(temp);
            break;
        }
        case 8: //小于
        {
            int temp = stack1.top();
            stack1.pop();
            if (stack1.top() < temp)
                temp = 1;
            else
                temp = 0;
            stack1.pop();
            stack1.push(temp);
            break;
        }
        case 9: //小于等于
        {
            int temp = stack1.top();
            stack1.pop();
            if (stack1.top() <= temp)
                temp = 1;
            else
                temp = 0;
            stack1.pop();
            stack1.push(temp);
            break;
        }
        case 10: //大于
        {
            int temp = stack1.top();
            stack1.pop();
            if (stack1.top() > temp)
                temp = 1;
            else
                temp = 0;
            stack1.pop();
            stack1.push(temp);
            break;
        }
        case 11: //大于等于
        {
            int temp = stack1.top();
            stack1.pop();
            if (stack1.top() >= temp)
                temp = 1;
            else
                temp = 0;
            stack1.pop();
            stack1.push(temp);
            break;
        }
        case 12: //读
        {
            int temp;
            //scanf("%d",&temp);
            cin >> temp;
            stack1.push(temp);
            break;
        }
        case 13: //写
        {
            cout << stack1.top() << endl;
            break;
        }
        case 14:
        {
            int temp = stack1.top();
            stack1.pop();
            temp = 0 - temp;
            stack1.push(temp);
            break;
        }
    }
}

void Run()
{
    while (cid != codeId - 1)
    {
        if(code[cid].funcCode == "jmp")
        {
            if(cid == 0)
            {
                action1.push_back(0);
                moveId = 0;
                backId = 1;
                staticId = 0;
                actId++;
                action1.push_back(0);
                actId++;
                action1.push_back(0);
                actId++;
            }
            cid = code[cid].displacement;
        }
        else if(code[cid].funcCode == "opr")
        {
            opr(code[cid].displacement);
            cid++;
        }
        else if(code[cid].funcCode == "lit") //常量声明
        {
            stack1.push(code[cid].displacement);
            cid++;
        }
        else if(code[cid].funcCode == "lod")
        {
			stack1.push(action1[base(code[cid]) + code[cid].displacement]);
			cid++;
        }
        else if(code[cid].funcCode == "sto")
        {
			action1[base(code[cid]) + code[cid].displacement] = stack1.top();
			cid++;
        }
        else if(code[cid].funcCode == "cal")  //过程调用语句
        {
            action1.push_back(base(code[cid]));
            int tempMoveId = moveId;
            moveId = actId;   //动态链
            actId++;
            action1.push_back(cid+1);
            backId = actId;
            actId++;
			action1.push_back(tempMoveId);
			staticId = actId;
			cid = code[cid].displacement;
            actId++;
        }
        else if(code[cid].funcCode == "jpc")
        {
		    if (stack1.top() == 0)
            {
                cid = code[cid].displacement;
            }
		    else
            {
                cid++;
            }           
        }
        else if(code[cid].funcCode == "int")
        {
            for(int i = 0; i < code[cid].displacement - 3; i++)
            {
                action1.push_back(0);
                actId++;
            }
            cid++;
        }
    }
}

int main()
{
    
    code.clear();
    action1.clear();
    getTargetCode();
    Run();
    getchar();
    return 0;
}
