#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#define STACK_SIZE 20
typedef enum{lparen=0,rparen=1,plus=2,minus=3,times=4,divide=5,eos=6,operand=7} precedence;
char map[6]={'(', ')', '+', '-', '*', '/'};
int isp[6]={0, 19, 12, 12, 13, 13};
int icp[6]={20, 19, 12, 12, 13, 13};
typedef struct {
	bool is_char;
	int data;
} D;
precedence get_token(int *num, int *u, char *exp)
{
	char s=exp[*u];
	(*u)++;
	*num=(int)s-48;
	if (*num>-1 && *num<10) {
		while (exp[*u]>47 && exp[*u]<58)
			*num=*num*10+exp[(*u)++]-48;
		return operand;
	}
	switch(s){
		case '(':
			return lparen;
		case ')':
			return rparen;
		case '+':
			return plus;
		case '-':
			return minus;
		case '/':
			return divide;
		case '*':
			return times;
		case '\0':
			return eos;
	}
}
void print_D(bool is_char, int data, D *d, int *offset) {
	d[*offset].is_char=is_char;
	d[*offset].data=data;
	(*offset)++;
	if (is_char)
		printf("%c", map[data]);
	else
		printf("%d", data);
	return;
}
void postfix(char *exp, D *d, int *offset)
{
	int op;
	precedence token;
	precedence stack[STACK_SIZE];
	int n = 0;
	int top = 0;
	for (token = get_token(&op, &n, exp); token != eos; token = get_token(&op, &n, exp)){
		if (token == operand)
			print_D(false, op, d, offset);
		else if (token == rparen){
			while(top>0 && stack[top-1] != lparen)
				print_D(true, stack[--top], d, offset);
			top--;
		}
		else{
			while(top>0 && isp[stack[top-1]]>=icp[token])
				print_D(true, stack[--top], d, offset);
			stack[top++]=token;;
		}
	}
	while (true) {
		if (top<=0)
			break;
		token=stack[--top];
		print_D(true, token, d, offset);
	}
	printf("\n");
}
int eval(D *d, int size){
	int stack[STACK_SIZE];
	int op1,op2, offset;
	int n = 0;
	int top = 0;
	for (offset=0;offset<size;++offset) {
		D token=d[offset];
		if(!token.is_char)
			stack[top++]=token.data;
		else {
			op2 = stack[--top];
			op1 = stack[--top];
			switch((precedence)token.data){
				case plus:stack[top++]=op1+op2;
						  break;
				case minus:stack[top++]=op1-op2;
						  break;
				case times:stack[top++]=op1*op2;
						 	break;
				case divide:stack[top++]=op1/op2;
							break;
				}
		}
	}
	return stack[--top];
}
int main(void){
	char input[100];
	D d[STACK_SIZE];
	int offset=0;
	scanf("%s",input);
	postfix(input, d, &offset);
	int result=eval(d, offset);
	printf("%d\n", result);
	return 0;
}	
