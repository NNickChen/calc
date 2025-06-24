#include "calc.hpp"

char *s;

double build(int l,int r);
double cal(double a,double b,char c);

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

double build(int l,int r)
{
	int t=find(l,r);
	if(t!=-1) return cal(build(l,t-1),build(t+1,r),s[t]);
	else if(s[l]=='(') return build(l+1,r-1);
	else return number(l,r);
	return -1;
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
