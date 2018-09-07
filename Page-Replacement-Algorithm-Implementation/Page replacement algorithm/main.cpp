#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h> 
//#include <memory.h>
#include <iomanip>
using namespace std;


int aa[320];
int bb[320];
int cc[320]={7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1};
int dd[32][2];
double opt,fifo,lru;
double  interrupt1,interrupt2,interrupt3;
void setInstruction()
{
	int n;
	srand(time(NULL));
	//产生320条指令
	for(int i=0;i<320;i++)
	{
		n=320;
		aa[i]=rand()%n+1;//产生随机指令
		if(aa[i]==319)//顺序执行一条指令
			aa[i+1]=aa[i];
		else
			aa[i+1]=aa[i]+1;
		//往前跳产生指令
		n=aa[i+1];
		i=i+2;
		aa[i]=rand()%n+1;
		if(aa[i]==319)//顺序执行一条指令
			aa[i+1]=aa[i];
		else
			aa[i+1]=aa[i]+1;
		i=i+2;
		//往后跳产生一条指令
		while(1)
		{
			aa[i]=rand()%320+1;
			if(aa[i]>aa[i-2]+2)
				break;
		}

	}
	return ;
}
void setPage(int n)//通过输入值确定其所在页面值
{
	for(int i=0;i<320;i++)
		bb[i]=aa[i]/(n*10)+1;
	return ;
}
int reduced()
{
	int i,n=1,c;
	//合并相邻相同值
	cc[0]=c=bb[0];
	for(i=0;i<320;i++)
	{
		if(c!=bb[i])
		{
			cc[n]=c=bb[i];
			n++;
		}
	}
	return n;
}
void OPT(int num,int Bnum)
{
	opt=0;
	int i=0,j,k=0,l,max,m;
	bool flag;
	memset(dd,0,sizeof(dd));
	while(i<Bnum)//通过Bnum将总的内存块填满
	{
		flag=false;
		for(j=0;j<i;j++)
		{
			if(dd[j][0]==cc[k])//判断是否已存在于内存块中
			{
				flag=true;
				break;
			}
		}
		if(!flag)//不存在则写入
		{
			dd[i][0]=cc[k];
			opt++;//记录缺页中断
			for(l=k+1;l<num;l++)//找出该页面最近的位置
			{
				if(cc[l]==dd[i][0])
				{
					dd[i][1]=l;
					break;
				}
				if(cc[l]!=dd[i][0]&&l==num-1)
				{
					dd[i][1]=500;
				}
			}
			i++;
		}
		else
		{
			for(l=k+1;l<num;l++)//存在则重新修改，找出该页面新的最近的位置
			{
				if(cc[l]==dd[i][0])
				{
					dd[i][1]=l;
					break;
				}
				if(cc[l]!=dd[i][0]&&l==num-1)
				{
					dd[i][1]=500;
				}
			}
		}
		k++;
	}
	//内存块已经满了以后的操作
	for(;k<num;k++)
	{
		//判断是否已存在于内存块中
		max=-1;
		flag=false;
		for(j=0;j<Bnum;j++)
		{
			if(dd[j][0]==cc[k])
			{
				flag=true;
				//存在则重新修改，找出该页面新的最近的位置
				for(l=k+1;l<num;l++)
				{
					if(cc[l]==dd[j][0])
					{
						dd[j][1]=l;
						break;
					}
					if(cc[l]!=dd[j][0]&&l==num-1)
					{
						dd[j][1]=500;
					}
				}
				break;
			}
		}
		if(!flag)//不存在则置换页面
		{
			opt++;//记录缺页中断
			for(i=0;i<Bnum;i++)//找出最远不会被调用的页面
			{
				if(dd[i][1]>max)
				{
					max=dd[i][1];
					m=i;
				}
			}
			dd[m][0]=cc[k];//将其置换
			for(l=k+1;l<num;l++)//找出该页面新的最近的位置
			{
				if(cc[l]==dd[m][0])
				{
					dd[m][1]=l;
					break;
				}
				if(cc[l]!=dd[m][0]&&l==num-1)
				{
					dd[m][1]=500;
				}
			}

		}
	}
	interrupt1=opt/num*100;//计算其缺页中断率
	return ;

}
void FIFO(int num,int Bnum)
{
	fifo=0;
	int i=0,j,k=0,m;
	bool flag;
	memset(dd,0,sizeof(dd));
	while(i<Bnum)//通过Bnum将总的内存块填满
	{
		flag=false;
		for(j=0;j<i;j++)
		{
			if(dd[j][0]==cc[k])//判断是否已存在于内存块中
			{
				flag=true;
				break;
			}
		}
		if(!flag)//不存在则写入
		{
			dd[i][0]=cc[k];
			fifo++;//记录缺页中断
			i++;
		}
		k++;
	}
	m=0;//记录当前最早开始的内存块
	for(;k<num;k++)
	{
		flag=false;
		for(j=0;j<Bnum;j++)
		{
			if(dd[j][0]==cc[k])//判断是否已存在于内存块中
			{
				flag=true;
				break;
			}
		}
		if(!flag)//不存在则置换
		{
			fifo++;//记录缺页中断
			dd[m][0]=cc[k];//将其置换
			m++;//确定新的最早开始的内存块
			m=m%Bnum;
		}
	}
	interrupt2=fifo/num*100;//计算其缺页中断率
	return ;
}
void LRU(int num,int Bnum)
{
	int i=0,j,k=0,min,m;
	bool flag;
	lru=0;
	memset(dd,0,sizeof(dd));
	while(i<Bnum)//通过Bnum将总的内存块填满
	{
		flag=false;
		for(j=0;j<i;j++)
		{
			if(dd[j][0]==cc[k])//判断是否已存在于内存块中
			{
				dd[j][1]=k;//修改当前执行位置
				flag=true;
				break;
			}
		}
		if(!flag)//不存在则写入
		{
			dd[i][0]=cc[k];
			dd[i][1]=k;//记录当前执行位置
			lru++;//记录缺页中断
			i++;
		}
		k++;
	}
	//内存块已经满了以后的操作
	for(;k<num;k++)
	{
		min=500;
		flag=false;
		for(j=0;j<Bnum;j++)//判断是否已存在于内存块中
		{
			if(dd[j][0]==cc[k])//存在则重新修改，记录其当前执行位置
			{
				dd[j][1]=k;
				flag=true;
				break;
			}
		}
		if(!flag)//不存在则置换页面
		{
			lru++;//记录缺页中断
			for(i=0;i<Bnum;i++)
			{
				if(dd[i][1]<min)//找出最久未被执行的块内存
				{
					min=dd[i][1];
					m=i;
				}
			}
			dd[m][0]=cc[k];//将其置换出来
			dd[m][1]=k;//记录当前执行位置
		}
	}
	interrupt3=lru/num*100;//计算缺页中断率
	return ;
}
int main()
{
	int  i,n,num,Bnum;
	
	num=20;
	Bnum=3;
	n=0;
	OPT(num,Bnum);
	FIFO(num,Bnum);
	LRU(num,Bnum);
	cout<<"Verify the correctness of the test data:\n";
	cout<<"\nThe total number of page calls is:"<<num<<", the number of memory blocks allocated to the program is:"<<Bnum<<"\n";
	cout<<"The number of missing pages of the algorithm OPT is"<<(int)opt<<", the page fault interrupt rate is:"<<setprecision(2)<<interrupt1<<"%。\n";
	cout<<"The number of pages missing in algorithm FIFO is"<<(int)fifo<<", the page fault interrupt rate is:"<<setprecision(2)<<interrupt2<<"%。\n";
	cout<<"The number of missing pages of the algorithm LRU is"<<(int)lru<<", the page fault interrupt rate is:"<<setprecision(2)<<interrupt3<<"%。\n\n\n\n";
	//随机产生指令
	memset(aa,0,sizeof(aa));
	setInstruction();
	cout<<"Display instruction address:\n";
	for(i=0;i<320;i++)
	{
		if(i%5==0&&i!=0)
			if(i%10==0)
				cout<<endl;
			else
				cout<<"      ";
		cout<<setw(4)<<aa[i]<<" ";
		
	}
	//根据输入页面大小确定指令所在页面
	cout<<"\nPlease enter page size:";
	cin>>n;
	setPage(n);
	cout<<"\nThe page number of each instruction is:\n";
	for(i=0;i<320;i++)
	{
		if(i%5==0&&i!=0)
			if(i%10==0)
				cout<<endl;
			else
				cout<<"      ";
		cout<<setw(4)<<bb[i]<<" ";
		
	}
	//合并相邻相同页面
	cout<<endl;
	num=reduced();
	cout<<"\nCombined results:\n";
	for(i=0;i<num;i++)
	{
		if(i%5==0&&i!=0)
			if(i%10==0)
				cout<<endl;
			else
				cout<<"      ";
		cout<<setw(4)<<cc[i]<<" ";
	}
	cout<<endl;
	//输入内存块大小，计算缺页中断率
	cout<<"\nPlease enter the size of the block:";
	cin>>Bnum;
	OPT(num,Bnum);
	FIFO(num,Bnum);
	LRU(num,Bnum);
	//输出测试结果
	cout<<"\nPage size:"<<n<<"K. The total number of pages is"<<num<<", the number of memory blocks allocated to the program is:"<<Bnum<<"\n";
	cout<<"The number of missing pages of the algorithm OPT is"<<(int)opt<<", the page fault interrupt rate is:"<<setprecision(2)<<interrupt1<<"%。\n";
	cout<<"The number of pages missing in algorithm FIFO is"<<(int)fifo<<", the page fault interrupt rate is:"<<setprecision(2)<<interrupt2<<"%。\n";
	cout<<"The number of pages missing in algorithm LRU is"<<(int)lru<<", the page fault interrupt rate is:"<<setprecision(2)<<interrupt3<<"%。\n";

	return 0;
}