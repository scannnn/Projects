#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
typedef struct{
	int top;
	char st[20];
}STACK;
void initialize(STACK *);
int isEmpty(STACK *);
int isFull(STACK *);
void push(STACK *,char);
char pop(STACK *);
void makePostfix();
void printStack(int,int,STACK);
void printfStack2(STACK);
void solvePostfix();
char infix[20][20],postfix[20][20];
int values[10];
/*Up there my functions and some global variables as the name implies. And values array is an array that we're keeping our a,b(vaiables). For example
for a values[variable_name-'a'] when we do this substract we will get the index number that we will keep in this array.*/
void main(){
	int N=0,i,j;
	FILE *fi;
	char buffer[100],*token;
	fi = fopen("input.txt","r");
	while(!feof(fi)){
		i = 0;
		fgets(buffer,100,fi);
		token = strtok(buffer," ");
		while(token != NULL){
			infix[N][i] = token[0];
			token = strtok(NULL," ");
			i++;
		}
		N++;
	}
	fclose(fi);
	/*Up there we're reading infix notations from text file.*/
	makePostfix();			// i will explain this function at inside it.
	printf("\nPostfix :");
	i=0;
	while(postfix[i][0]!='\0'){
		j=0;
		printf("\n");
		printf("	");
		while(postfix[i][j]!='\0'){
			printf("%c",postfix[i][j]);
			j++;
		}
		i++;
	}
	printf("\n");
	/* Up there we're printing our postfix notations that we found in makePostfix() function.*/
	solvePostfix();			// i will explain this function at inside it.
	i=0;
	printf("\n\nValues : \n");
	while(values[i] != NULL){
		printf("%c<-%d ",('a'+i),values[i]);
		i++;
	}
	printf("\n The page starting from the top. You need to scroll up it.\n(Sayfa yetmedi. Yukaridan basliyor ciktilar yukariya kaydirmaniz gerek.)");
	/* Up there we're printing our values.*/
	getch();
}
void initialize(STACK *stack){
	stack->top = 0;
}
int isEmpty(STACK *stack){
	if(stack->top == 0) return 1;
	return 0;
}
int isFull(STACK *stack){
	if(stack->top == 20) return 1;
	return 0;
}
void push(STACK *stack,char x){
	if(isFull(stack)) {
		printf("Stack is full\n");
		return;
	}
	stack->st[stack->top] = x;
	stack->top++;
}
char pop(STACK *stack){
	if(isEmpty(stack)){
		printf("Stack is empty\n");
		return;
	}
	stack->top--;
	return stack->st[stack->top];
}
/*Up there there are some functions that we need for our stack. These are general stack functions that necessary(pop,push...)*/
void makePostfix(){
	int i,j,k;
	/* i counter for lines. j for infix's column and k is for postfix's column.*/
	i=0;
	while(infix[i][0]!='\0'){
		int l;
		STACK s;
		initialize(&s);
		l=0;
		while(infix[i][l] != '\0'){
			printf("%c",infix[i][l]);
			l++;
		}
		printf("\n\n");
		j=2;
		k=2;
		postfix[i][0] = infix[i][0];
		printStack(i,0,s);
		postfix[i][1] = infix[i][1];
		printStack(i,1,s);
		/*Up there we're saving our first 2 variables to postfix array which are variable and = For example every infix notation starting with a= or
		b= so we don't need to search ( + - * .. for them.*/
		while(infix[i][j]!=';'){
			if((infix[i][j]=='(')||(infix[i][j]=='+')||(infix[i][j]=='-')||(infix[i][j]=='*')||(infix[i][j]=='/')||(infix[i][j]==')')){
				if(!isEmpty(&s)){
					char tmp;
					tmp = pop(&s);
					/* Firstly we are poping an item from stack because we need to compare symbol's priority.*/
					if(infix[i][j]=='('){
						push(&s,tmp);
						push(&s,infix[i][j]);
					}
					/* If we find a ( firstly pushing that we popped and pushing (.*/
					if(tmp=='('){
						push(&s,tmp);
						push(&s,infix[i][j]);
					}
					/* Like up there if we popped ( then we're pushing again.*/
					if(infix[i][j]==')'){
						postfix[i][k] = tmp;
						k++;
						tmp = pop(&s);
						while((tmp!='(')){
							postfix[i][k] = pop(&s);
							k++;
						}
					}
					/* Up there if we take ) from array now we need to pop all symbols from stact till we see the (.*/
					if(((infix[i][j]=='+')||(infix[i][j]=='-'))&&((tmp=='+')||(tmp=='-'))){
						push(&s,tmp);
						push(&s,infix[i][j]);
					}
					if(((infix[i][j]=='*')||(infix[i][j]=='/'))&&((tmp=='*')||(tmp=='/'))){
						push(&s,tmp);
						push(&s,infix[i][j]);
					}
					if(((infix[i][j]=='*')||(infix[i][j]=='/'))&&((tmp=='+')||(tmp=='-'))){
						push(&s,tmp);
						push(&s,infix[i][j]);
					}
					/* Up there as you can see from if statements we're looking for priorities.*/
					if(((infix[i][j]=='+')||(infix[i][j]=='-'))&&((tmp=='*')||(tmp=='/'))){
						while((tmp=='*')||(tmp=='/')){
							postfix[i][k] = tmp;
							k++;
							if(!isEmpty(&s)) tmp = pop(&s);
						}
						if(!isEmpty(&s)){
							push(&s,tmp);
							push(&s,infix[i][k]);
						}
						else push(&s,infix[i][k]);
					}
					/* Up there if infix is low priority according to tmp we need to pop all high priority symbols from stack and add them to postfix 
					notation.*/
				}
				else{
					push(&s,infix[i][j]);
				}
				/* We need this else statement for empty stack. If stack is empty we can't pop symbol for tmp variable.*/
			}
			else{
				postfix[i][k] = infix[i][j];
				k++;
			}
			/* We need this else statement for not symbol infix's. For example if our next item is 5 (constant) or a,b we won't push them stack.*/
			j++;
			printStack(i,k,s);		// i will explain this function in inside the function.
		}
		while(!isEmpty(&s)){
			postfix[i][k] = pop(&s);
			k++;
		}
		/* Up there finally when our infix array is finished we need to pop all items from stack that we're left behind. And we well complete our first
		line from infix to postfix.*/
		printf("\nIntermediate output : ");
		l=0;
		while(postfix[i][l]!='\0'){
			printf("%c",postfix[i][l]);
			l++;
		}
		printf("\n\n");
		i++;
	}
}
void printStack(int i,int k,STACK s){
	int j,l;
	char temp[20];
	printf("Postfix : ");
	for(j=0;j<k;j++){
		printf("%c",postfix[i][j]);
	}
		if(isEmpty(&s)) printf("    Stack : Empty");
		else{
			printf("    Stack : ");
			l=0;
			while(!isEmpty(&s)){
				temp[l] = pop(&s);
				l++;
			}
			l--;
			while(l>=0){
				printf("%c",temp[l]);
				l--;
			}
		}
		printf("\n");
}
/* Up there we're printing our stack's current contents and postfix notation.*/
void solvePostfix(){
	int i=0,j,k;
	while(postfix[i][0] != '\0'){
		STACK s;
		initialize(&s);
		j=2;
		k=0;
		printf("\n");
		while(postfix[i][k] != '\0'){
			printf("%c",postfix[i][k]);
			k++;
		}
		printf("   ");
		k=0;
		while(values[k] != NULL){
			printf("%c<-%d ",('a'+k),values[k]);
			k++;
		}
		printf("\n\n");
		/* Up there we're printing our current postfix notation and values that we're known.*/
		while(postfix[i][j] != '\0'){
			if((postfix[i][j]!='-')||(postfix[i][j]!='+')||(postfix[i][j]!='*')||(postfix[i][j]!='/')){
				if(isdigit(postfix[i][j])){
					push(&s,(postfix[i][j]-'0'));
				}
				else if(islower(postfix[i][j])){
					push(&s,values[postfix[i][j]-'a']);
					}
					/* Up there we're looking for being digit or a,b variables statement. If incoming char is digit or variable we're pushing them
					to stack.*/
					else if(postfix[i][j]=='*'){
					int temp1,temp2;
					temp1=pop(&s);
					temp2=pop(&s);
					push(&s,(temp2*temp1));
					}
					else if(postfix[i][j]=='-'){
					int temp1,temp2;
					temp1=pop(&s);
					temp2=pop(&s);
					push(&s,(temp2-temp1));
					}
					else if(postfix[i][j]=='+'){
					int temp1,temp2;
					temp1=pop(&s);
					temp2=pop(&s);
					push(&s,(temp2+temp1));
					}
					else if(postfix[i][j]=='/'){
					int temp1,temp2;
					temp1=pop(&s);
					temp2=pop(&s);
					push(&s,(temp2/temp1));
					}
					/* Up there as you can see we're doing mathematical operations with stack's top 2 item.*/
			}
			 printfStack2(s);
			 printf("\n");
			 j++;
		}
		values[postfix[i][0]-'a'] = pop(&s);
		i++;
	}
	/* Up there we're saving our variable that we found at the end of postfix notation.*/
}
void printfStack2(STACK s){
	int i,temp[20];
	printf("Stack : ");
	i=0;
	while(!isEmpty(&s)){
		temp[i] = pop(&s);
		i++;
	}
	i--;
	while(i>=0){
		printf("%d ",temp[i]);
		i--;
	}
}
/* Up there we're printing our stack's contents.*/
