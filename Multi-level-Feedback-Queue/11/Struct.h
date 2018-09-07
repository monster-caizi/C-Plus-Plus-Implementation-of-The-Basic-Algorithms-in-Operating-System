#include<time.h>
#include <stdio.h> 
#include<memory.h>
#include<iostream>

#if !defined(BB)
#define BB
bool static pi[101];
 #endif  
 


class node
{
public:
	int  pid;//进程编号
	int  status;//是否正在运行标志
	int  priority;//优先级
	int  life;//生命周期
	node *next;//指针指向先一个节点
	
	//bool static pi[101];


	node()//初始化
	{
		srand( (unsigned)time(NULL));//srand()函数产生一个以当前时间开始的随机种子.应该放在for等循环语句前面 不然要很长时间等待
		while(1)
		{
			int p=rand()%101;//随机产生进程编号
			if(p!=0)
			if(!pi[p])
			{
				pid=p;
				pi[p]=true;
				break;
			}
		}
		status=0;//标志位不在运行
		priority=rand()%50;//随机产生优先级
		life=0;
		while(life==0)//随机产生生命周期
		{
			life=rand()%6;
		}
		next=0;
	}
};


/*void setPid()
{
	memset(node.pi,0,101);
}
*/


