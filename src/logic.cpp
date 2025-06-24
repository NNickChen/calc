#include "calc.hpp"

double number(int l,int r)
{
	double n=0;
	bool f=false/*,f1=false*/;
	int i;
	// if(s[l]=='-') //负数的处理
	// {
	// 	f1=true;
	// 	l++;
	// }
	for(i=l;i<=r;i++)
	{
		if(s[i]=='.')
		{
			f=true;
			break;
		}
		n=n*10+s[i]-'0';
	}
	double j=10;
	if(f)
	{
		for(i=i+1;i<=r;i++)
		{
			n+=(s[i]-'0')/j;
			j*=10;
		}
	}
	// if(f1==true) n=-n; //负数
	return n;
}

double find(int l,int r)
{
	int t=0;
	int pos=-1,pos1=-1,pos2=-1;
	for(int i=l;i<=r;i++)
	{
		if(s[i]=='(') t++;
		else if(s[i]==')') t--;
		if(t!=0) continue;
		if(s[i]=='+'||s[i]=='-') pos=i;
		else if(s[i]=='*'||s[i]=='/')
		{
			if(pos!=-1) 
			{
				pos1=-1;
				pos2=-1;
			}
			else pos1=i;
		} else if(s[i]=='^')
		{
			if(pos!=-1||pos1!=-1)
			{
				pos2=-1;
			} else pos2=i;
		}
	}
	if(pos!=-1) return pos;
	else if(pos1!=-1) return pos1;
	else return pos2;
}

