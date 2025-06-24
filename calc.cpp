#include "calc.hpp"

char *s;

double build(int l,int r);
double cal(double a,double b,char c);


int main(int argc, char const *argv[])
{
	if(argc>1)
	{
		for(int i=1;i<argc;i++)
		{
			s=(char*)argv[i];
			double num=build(0,strlen(s)-1);
			printf("%.2lf\n",num);
		}
	}

	s=(char*)malloc(buf_size+1);
	if(!s)
	{
		printf("malloc failed");
		return 1;
	}
	while(true)
	{
		cout<<">";
		memset(s,'\0',1000);
		if(scanf("%s",s)==EOF||!strcmp(s,"exit")) break;
		double num=build(0,strlen(s)-1);
		printf("%.2lf\n",num);
	}
	free(s);
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
