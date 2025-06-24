#include"bits/stdc++.h"
using namespace std;

char *s;

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

double cal(double a,double b,char c)
{
	// cout<<"a="<<a<<" b="<<b<<endl;
	double ans=-1;
	if(c=='+') ans=a+b;
	else if(c=='-') ans=a-b;
	else if(c=='*') ans=a*b;
	else if(c=='^') ans=pow(a,b);
	else ans=a/b;
	return ans;
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

double build(int l,int r)
{
	int t=find(l,r);
	if(t!=-1) return cal(build(l,t-1),build(t+1,r),s[t]);
	else if(s[l]=='(') return build(l+1,r-1);
	else return number(l,r);
	return -1;
}

int main(int argc, char const *argv[])
{
	if(argc<=1)
	{
		s=(char*)malloc(1000);
		while(scanf("%s",s)!=EOF)
		{
			double num=build(0,strlen(s)-1);
			printf("%.2lf\n",num);
			memset(s,'\0',1000);
		}
	}
	else
	for(int i=1;i<argc;i++)
	{
		s=(char*)argv[i];
		double num=build(0,strlen(s)-1);
		printf("%.2lf\n",num);
	}
	return 0;
}
