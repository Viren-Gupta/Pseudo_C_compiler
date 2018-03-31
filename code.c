#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <stdbool.h>
#define SIZE 100000


#define DELIM 0
#define VAR 1
#define NUM 2
#define DOT 3
#define EQ 4
#define PLUS 5
#define MINUS 6
#define MULT 7
#define DIV 8
#define SEMI 9
#define LESS 10
#define EQEQ 11
#define WHILE 12
#define LRBRA 13
#define RRBRA 14
#define LCBRA 15
#define RCBRA 16
#define ARR 17
#define LSBRA 18
#define RSBRA 19

typedef struct Node{
	char* root;
	struct Node** children;
	int numberOfChildren;
}Node;

typedef struct{
	int tokenType;
	char* tokenValue;
}Token;

bool isSpace(char ch){
	if(ch==' '||ch=='\t'||ch=='\n')
		return true;
	else 
		return false;
	
}

bool isalphabet(char ch){
	if(ch>='a' && ch<='z')
		return true;
	else
		return false;
}

bool isNumeric(char ch){
	if(ch>='0' && ch<='9')
		return true;
	else
		return false;
}

int tokType;

void expression(Token*,int,int,Node*);
void statement(Token*,int,int,Node*);

int B;

char* getToken(char* input,char* ans){

	

	int index=0;
	char ch;
	char* word=malloc(sizeof(char)*SIZE);
	while(*input!='\0'){

		if(isSpace(*input)){
			//printf("sss %c",*input);

			input++;
			continue;
		}

		if(isalphabet(*input)){
			while(isalphabet(*input)){
				//printf("alpha\n");
				word[index++]=(*input);
				input++;
			}
			if (strcmp(word,"while")==0){
				tokType=WHILE;
			} 
			// else if (*input == '[') {
			// 	word[index++]=(*input);
			// 	input++;
			// 	while(*input != ']'){
			// 		//printf("num\n");
			// 		word[index++]=(*input);
			// 		input++;
			// 	}
			// 	word[index++]=(*input);
			// 	input++;

			// 	tokType=ARR;

			// }
			else{
				tokType=VAR;
			}
			strcpy(ans,word);

			
			return input;
		}
		
		if(isNumeric(*input) ){
			int counter=0;
			int flag=0;
			while(isNumeric(*input)|| *input == '.' ){
				//printf("num\n");
				
				if(flag==1){
					counter++;
				}

				if(*input=='.'){
					flag=1;
				}
				
				if(counter<=B)
					word[index++]=(*input);

				input++;
			}
			strcpy(ans,word);
			tokType=NUM;
			return input;
		}
		if(*input=='(' || *input==')' || *input=='[' || *input==']' || *input=='{' || *input=='}'){
			word[index++]=(*input);
			strcpy(ans,word);
			if(*input=='(')
				tokType= LRBRA;
			if(*input==')')
				tokType= RRBRA;
			if(*input=='[')
				tokType= LSBRA;
			if(*input==']')
				tokType= RSBRA;
			if(*input=='{')
				tokType= LCBRA;
			if(*input=='}')
				tokType= RCBRA;
			
			input++;
			return input;
		}


		if(*input=='=' || *input=='+' || *input=='-' || *input=='*' || *input=='/' || *input=='<'){
			word[index++]=(*input);
			if(*input=='=') {
				if (*(input+1) == '=') {
					tokType = EQEQ;
					input++;
					word[index++]=(*input);
					
				} else {
					tokType= EQ;
				}
			}
			
			strcpy(ans,word);
			
			if(*input=='+')
				tokType= PLUS;
			if(*input=='-')
				tokType= MINUS;
			if(*input=='*')
				tokType= MULT;
			if(*input=='/')
				tokType= DIV;
			if(*input=='<')
				tokType= LESS;

			input++;
			return input;
		}

		if(*input==';'){
			word[index++]=(*input);
			strcpy(ans,word);
			tokType=SEMI;
			input++;
			return input; 
		}

	}

}

Node* Mainroot;
int upto;

bool find(Token* array,int start,int end){
	int i;
	int sum=0;
	int flag=0;
	int chk=start;
	if (array[start].tokenType==WHILE){
		for(i=start;i<=end;i++){
			if (flag==1 && sum==0){

				upto=i;
				if(i==end){
					return false;
				}
				else
					return true;
			}
			if(array[i].tokenType==LCBRA){
				flag=1;
				sum++;
			}
			else if(array[i].tokenType==RCBRA){
				sum--;
			}
		}
	}
	else{
		for(i=start;i<=end;i++){
			if(array[i].tokenType==SEMI){
				upto=i;
				if(i==end)
					return false;
				return true;
			}
		}
	}
}

void variable(Token* array,int start,int end,Node* parent){
		//printf("In variable\n");
	//if(start!=end)
	//{
		//printf("Error in variable\n");
		//printf("%d %d\n",start,end );
	//}
	if(start != end && array[start+1].tokenType==LSBRA){

//		printf("%d %d In variable\n",start,end);

		parent->children=(Node**)malloc(sizeof(Node*)*SIZE);
		parent->numberOfChildren=0;

		Node* child1=(Node*)malloc(sizeof(Node)*1);
		Node* child2=(Node*)malloc(sizeof(Node)*1); 
		Node* child3=(Node*)malloc(sizeof(Node)*1);
		Node* child4=(Node*)malloc(sizeof(Node)*1);


		child1->root="variable";
		child2->root="[";
		child3->root="expression";
		child4->root="]";


		parent->children[0]=child1;
		parent->children[1]=child2;
		parent->children[2]=child3;
		parent->children[3]=child4;
		parent->numberOfChildren+=4;

		variable(array , start ,start , child1);
		expression(array , start+2 ,end-1 , child3);

	}else{
		parent->children=(Node**)malloc(sizeof(Node*)*SIZE);
		parent->numberOfChildren=0;
		Node* child1=(Node*)malloc(sizeof(Node)*1);
		child1->root=array[start].tokenValue;
		parent->children[0]=child1;
		parent->numberOfChildren+=1;
	}
}

int ind;

int checkForOperator(Token* array,int start,int end,int tok1,int tok2){
	int j, balance = 0;
	for(j=end;j>=start;j--){
		if (array[j].tokenType == LRBRA || array[j].tokenType == LSBRA) {
			balance++;
		} else if (array[j].tokenType == RRBRA || array[j].tokenType == RSBRA) {
			balance--;
		}
		if(array[j].tokenType==tok1 || array[j].tokenType==tok2)
			{
				if (balance != 0) {
					continue;
				}	
				ind=j;
				return array[j].tokenType;
			}
	}

	return -1;
}

void factor(Token* array,int start,int end,Node* parent){
	parent->children=(Node**)malloc(sizeof(Node*)*SIZE);
	parent->numberOfChildren=0;

	int ptr=array[start].tokenType;
	if(ptr==VAR){
			Node* child1=(Node*)malloc(sizeof(Node)*1);
			child1->root="variable";
			parent->children[0]=child1;
			parent->numberOfChildren+=1;
			variable(array,start,end,child1);
		
	}
	else if(ptr==NUM){
		/*Node* child1=(Node*)malloc(sizeof(Node)*1);
		child1->root=;
		parent->children[0]=array[start].tokenValue;
		parent->children[0]=child1;
		parent->numberOfChildren+=1;
		//number(array,start,end,child1);*/

		Node* child1=(Node*)malloc(sizeof(Node)*1);
		child1->root=array[start].tokenValue;
		parent->children[0]=child1;
		parent->numberOfChildren+=1;
	}
	else{

		Node* child1=(Node*)malloc(sizeof(Node)*1);
		Node* child2=(Node*)malloc(sizeof(Node)*1); 
		Node* child3=(Node*)malloc(sizeof(Node)*1);


		child1->root="(";
		child2->root="expression";
		child3->root=")";


		parent->children[0]=child1;
		parent->children[1]=child2;
		parent->children[2]=child3;
		parent->numberOfChildren+=3;

		expression(array,start+1,end-1,child2);
	}
}


void mulDiv(Token* array,int start,int end,Node* parent){
	parent->children=(Node**)malloc(sizeof(Node*)*SIZE);
	parent->numberOfChildren=0;

	int ans=checkForOperator(array,start,end,MULT,DIV);
	int localInd=ind;
	//*******************************************************************************
	if(ans!=-1 ){
		Node* child1=(Node*)malloc(sizeof(Node)*1);
		Node* child2=(Node*)malloc(sizeof(Node)*1); 
		Node* child3=(Node*)malloc(sizeof(Node)*1);
		
		child1->root="mulDiv";
		if(ans==MULT)
			child2->root="MULT";
		else
			child2->root="DIV";

		child3->root="factor";	
		parent->children[0]=child1;
		parent->children[1]=child2;
		parent->children[2]=child3;
		parent->numberOfChildren+=3;

		mulDiv(array,start,localInd-1,child1);
		factor(array,localInd+1,end,child3);
	}
	else{
		Node* child1=(Node*)malloc(sizeof(Node)*1);
		child1->root="factor";
		parent->children[0]=child1;
		parent->numberOfChildren+=1;
		factor(array,start,end,child1);
	}

}


void term(Token* array,int start,int end,Node* parent){
	parent->children=(Node**)malloc(sizeof(Node*)*SIZE);
	parent->numberOfChildren=0;

	int ans=checkForOperator(array,start,end,PLUS,MINUS);
	int localInd=ind;
	if(ans!=-1 ){
		Node* child1=(Node*)malloc(sizeof(Node)*1);
		Node* child2=(Node*)malloc(sizeof(Node)*1); 
		Node* child3=(Node*)malloc(sizeof(Node)*1);
		
		child1->root="term";
		if(ans==PLUS)
			child2->root="PLUS";
		else
			child2->root="MINUS";

		child3->root="mulDiv";	
		parent->children[0]=child1;
		parent->children[1]=child2;
		parent->children[2]=child3;
		parent->numberOfChildren+=3;

		term(array,start,localInd-1,child1);
		mulDiv(array,localInd+1,end,child3);
	}
	else{
		Node* child1=(Node*)malloc(sizeof(Node)*1);
		child1->root="mulDiv";
		parent->children[0]=child1;
		parent->numberOfChildren+=1;
		mulDiv(array,start,end,child1);
	}

}

void expression(Token* array,int start,int end,Node* parent){
	//printf("In expression\n");
	parent->children=(Node**)malloc(sizeof(Node*)*SIZE);
	parent->numberOfChildren=0;

	int ans=checkForOperator(array,start,end,LESS,EQEQ);
	int localInd=ind;
	if(ans!=-1 ){
		Node* child1=(Node*)malloc(sizeof(Node)*1);
		Node* child2=(Node*)malloc(sizeof(Node)*1); 
		Node* child3=(Node*)malloc(sizeof(Node)*1);
		
		child1->root="expression";
		if(ans==LESS)
			child2->root="LESS";
		else
			child2->root="EQEQ";
		child3->root="term";

		parent->children[0]=child1;
		parent->children[1]=child2;
		parent->children[2]=child3;
		parent->numberOfChildren+=3;

		expression(array,start,localInd-1,child1);
		term(array,localInd+1,end,child3);
	}
	else{
		Node* child1=(Node*)malloc(sizeof(Node)*1);
		child1->root="term";
		parent->children[0]=child1;
		parent->numberOfChildren+=1;
		term(array,start,end,child1);
	}

}


void cond(Token* array,int start,int end,Node* parent){
	//printf("in cond\n");
	parent->children=(Node**)malloc(sizeof(Node*)*SIZE);
	parent->numberOfChildren=0;
	//printf("******** %d %d\n",start,end );
	if (array[start].tokenType==WHILE){
		Node* child1=(Node*)malloc(sizeof(Node)*1);
		Node* child2=(Node*)malloc(sizeof(Node)*1); 
		Node* child3=(Node*)malloc(sizeof(Node)*1);
		Node* child4=(Node*)malloc(sizeof(Node)*1);
		Node* child5=(Node*)malloc(sizeof(Node)*1);
		Node* child6=(Node*)malloc(sizeof(Node)*1); 
		Node* child7=(Node*)malloc(sizeof(Node)*1);
		child1->root = "while";
		child2->root = "(";
		child3->root = "expression";
		child4->root = ")";
		child5->root = "{";
		child6->root = "statement";
		child7->root = "}";

		parent->children[0]=child1;
		parent->children[1]=child2;
		parent->children[2]=child3;
		parent->children[3]=child4;
		parent->children[4]=child5;
		parent->children[5]=child6;
		parent->children[6]=child7;
		parent->numberOfChildren+=7;

		//printf("%d--------------------%d\n",start, end);
		int j ;
			for(j=start;j<=end;j++){
		//		printf("array : %d\n", array[j].tokenType);
				if(array[j].tokenType== LCBRA)
					break;
			}
		//	printf("Start : %d\n",j );
	//		printf("111111111 %d %d\n",start+2,j-2 );
		 expression(array, start+2, j-2  , child3);
	//	 printf("22222222222 %d %d\n",j+1,end-1 );
	//	 printf("Called 1 \n");
		 statement(array, j+1 , end-1, child6);

	}	
	else{
		Node* child1=(Node*)malloc(sizeof(Node)*1);
		Node* child2=(Node*)malloc(sizeof(Node)*1); 
		Node* child3=(Node*)malloc(sizeof(Node)*1);


		child1->root="variable";
		child2->root="EQ";
		child3->root="expression";


		parent->children[0]=child1;
		parent->children[1]=child2;
		parent->children[2]=child3;
		parent->numberOfChildren+=3;


	//printf("%d\n", parent.numberOfChildren);

		int j;
			for(j=start;j<=end;j++){
				if(array[j].tokenType==EQ)
					break;
			}

		variable(array,start,j-1,child1);
		expression(array,j+1,end,child3);	
	}
}

void statement(Token* array,int start,int end,Node* parent){

	int localUpto=0;
	
	parent->children=(Node**)malloc(sizeof(Node*)*SIZE);
	parent->numberOfChildren=0;
	bool val=find(array,start,end);
	localUpto=upto;
	//printf("ss %d\n",localUpto );
	int rule=0;
	if(val)
		rule=1;
	else
		rule=2;
	//printf("rule %d %d\n",rule ,localUpto);
	if(rule==1){
		Node* child1=(Node*)malloc(sizeof(Node)*1);
		Node* child2=(Node*)malloc(sizeof(Node)*1);
		Node* child3=(Node*)malloc(sizeof(Node)*1);
		child1->root="cond";
		child2->root=";";
		child3->root="statement";

		parent->children[0]=child1;
		parent->children[1]=child2;
		parent->children[2]=child3;

		parent->numberOfChildren+=3;
		//printf("%d\n", parent.numberOfChildren);

		//printf("mmmmmmmy end : %d\n",end );



		cond(array,start,localUpto-1,child1);
	//	printf("upto %d\n",localUpto );
	//	printf("Called 2\n");
		statement(array,localUpto+1,end,child3);

	}
	else{
	//	printf(" In statement else \n");
		//printf("upto %d\n",upto );
		//printf("%d\n", parent.numberOfChildren);
		Node* child1=(Node*)malloc(sizeof(Node)*1);
		Node* child2=(Node*)malloc(sizeof(Node)*1);
		child1->root="cond";
		child2->root=";";
		
		parent->children[0]=child1;
		parent->children[1]=child2;
		parent->numberOfChildren+=2;
		//printf("my end : %d\n",end );
		cond(array,start,end-1,child1);
		
	}
}

void postOrder(Node* p){
	if(p==NULL)
		return;
	int j;
	//if(p->numberOfChildren==0)
	printf("%s\n",p->root );
	for(j=0;j< p->numberOfChildren;j++){
		postOrder(p->children[j]);
	}
	
}

FILE* fpt;

void displayList(Node* p){
	if(p==NULL)
		return;
	int j;

	if(p->numberOfChildren==0){
		fprintf(fpt," %s ",p->root );
		return;
	}
	

	fprintf(fpt," %s #",p->root );
	for(j=0;j< p->numberOfChildren;j++){
		displayList(p->children[j]);
	}

	fprintf(fpt," @ ");
}


void parseFunc(Token* array,int size){
	Node* parent=(Node*)malloc(sizeof(Node)*1);
	parent->root="statement";
	Mainroot=parent;
	//printf("Called 3\n");
	statement(array,0,size-1,parent);
	Node* temp=Mainroot;
	//printf("%s\n",temp->root );
	//printf("%d\n",temp->numberOfChildren );
	int j;/*
	for(j=0;j<temp->numberOfChildren;j++){
		printf("%s ",temp->children[j]->root);
		//printf("child %d\n",temp->children[j]->numberOfChildren);
	}*/

	//printf("PreOrder\n\n");
	//postOrder(Mainroot);

	displayList(Mainroot);
}


void calc(char* input){
	Token* array=malloc(sizeof(Token)*SIZE);
	int i=0,v;
	while(*input!='\0'){
		if(isSpace(*input)){
			input++;
			continue;
		}
		char* ans=malloc(sizeof(char)*SIZE);
		char* p;
		p=getToken(input,ans);
		input=p;
		
		Token t;
		t.tokenType=tokType;
		t.tokenValue=ans;
		array[i++]=t;
		tokType=0;
	}

	/*int j;
	for(j=0;j<i;j++)
		printf("%s     %d\n",array[j].tokenValue,array[j].tokenType);
*/
	parseFunc(array,i);

	
}



int main(int argc,char** argv){

	if(argc < 2){
		printf(" Please enter bit precision as command line argument\n");
		return;
	}


	B=atoi(argv[1]);

	fpt=fopen("output.txt","w");

	FILE *fp;
	char *buff=(char*)malloc(sizeof(char)*SIZE);
	char ch;
	int j=0;
	fp = fopen("input.txt", "r");
	while((ch=fgetc(fp))!=EOF){
		
		buff[j++] =ch;
	}

	fclose(fp); 

	//printf("%s\n",buff );
	// char* ptr="hh=9;j=132+3*2;ar[]";
	calc(buff);

	fclose(fpt);
	return 0;
}
