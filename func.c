#include "func.h"




void push(Stack* s, StackNode* data) { //스택 push
	if (s->top >= STACK_SIZE - 1)
	{
		fprintf(stderr, "Stack FUll!");
	}
	else
		s->stk[++s->top] = data;
}



StackNode* pop(Stack* s) {//스택 pop
	if (s->top == -1)
	{
		fprintf(stderr, "Stack empty!");
		
	}
	else {
		return s->stk[s->top--];
	}
}


void initStack(Stack* s) { //스택 초기화
	s->top = -1;
	int i;
	/*for (i = 0; i < 100; i++)
	{
		//s->stk[i]->list = NULL;
		s->stk[i] = NULL;
		
	}*/
}

int StackEmpty(Stack* s) { //스택이 비었는지 확인
	if (s->top == -1)
		return TRUE;
	else
		return FALSE;
}

void error() {
	fprintf(stderr, "Error occured! Check your code again!");
}


int calcsym(Stack* s,StackNode* node) {//인자로 받은 스택의 노드의 값을 IDENTIFIER에서 숫자 혹은 리스트로 바꾸기 위한 함수
	int exist = isVarExist(node->str); //node가 값으로 취하는 변수의 이름과 똑같은 이름을 가진 변수가 저장되어 있는지 체크
	if (exist == NOT_EXIST) { //존재하지 않으면 에러 메시지 출력
		error();
		return FALSE;
	}
	else {
		if (vaList.vars[exist]->type == NUMBER)
		{
			StackNode* a = (StackNode*)malloc(sizeof(StackNode));
			a->isAllocated = TRUE;
			a->type = NUMBER;
			a->number = vaList.vars[exist]->number;
			push(s, a);
			return TRUE;
		}
		else if (vaList.vars[exist]->type == LIST)
		{
			return TRUE;
		}
	}
}
StackNode* add(Stack* s) { //덧셈
	StackNode* ret = (StackNode*)malloc(sizeof(Node));
	ret->isAllocated = TRUE;
	ret->number = 0;
	int calcResult;

	StackNode* firstnode = pop(s);
	switch (firstnode->type)
	{
	case SYMBOL:
		calcResult = calcsym(s, firstnode);
		if (calcResult == TRUE)
		{
			ret->number = pop(s)->number;
		}
		else
			error();
		break;
	case NUMBER:
		ret->number = firstnode->number;
		break;
	}
	while (StackEmpty(s)==FALSE)
	{
		StackNode* node = pop(s);
		if (node->type == NUMBER)
		{
			ret->number += node->number;
			
		}
		else if (node->type == SYMBOL)
		{
			calcResult = calcsym(s, node);
			if (calcResult == FALSE)
				break;
			else {
				ret->number += pop(s)->number;
			}
		}
		else {
			ret->type = ERROR_TYPE;
			break;
		}
	}
	ret->type = NUMBER;
	return ret;
}

StackNode* sub(Stack* s) {//뺄셈
	StackNode* ret = (StackNode*)malloc(sizeof(Node));
	ret->isAllocated = TRUE;
	StackNode* firstnode = pop(s);
	int calcResult;
	switch (firstnode->type)
	{
		case SYMBOL:
			calcResult = calcsym(s, firstnode);
			if (calcResult == TRUE)
			{
				ret->number = pop(s)->number;
			}
			else
				error();
			break;
		case NUMBER:
			ret->number = firstnode->number;
			break;
	}
	while (StackEmpty(s)==FALSE)
	{
		StackNode* node = pop(s);
		if (node->type == NUMBER)
		{
			ret->number -= node->number;

		}
		else if (node->type == SYMBOL)
		{
			calcResult = calcsym(s, node);
			if (calcResult == FALSE)
				break;
			else {
				ret->number -= pop(s)->number;
			}
		}
		else {
			ret->type = ERROR_TYPE;
			break;
		}
	}

	ret->type = NUMBER;
	return ret;
}


void varInit() {
	vaList.varlength = 0;
}


StackNode* mult(Stack* s) {//곱셈
	StackNode* ret = (StackNode*)malloc(sizeof(Node));
	ret->isAllocated = TRUE;
	ret->number = 1;
	int calcResult;
	StackNode* firstnode = pop(s);
	switch (firstnode->type)
	{
	case SYMBOL:
		calcResult = calcsym(s, firstnode);
		if (calcResult == TRUE)
		{
			ret->number = pop(s)->number;
		}
		else
			error();
		break;
	case NUMBER:
		ret->number = firstnode->number;
		break;
	}
	while (StackEmpty(s)==FALSE)
	{
		StackNode* node = pop(s);
		if (node->type == NUMBER)
		{
			ret->number *= node->number;

		}
		else if (node->type == SYMBOL)
		{
			calcResult = calcsym(s, node);
			if (calcResult == FALSE)
				break;
			else {
				ret->number *= pop(s)->number;
			}
		}
		else {
			ret->type = ERROR_TYPE;
			break;
		}
	}
	ret->type = NUMBER;
	return ret;
}


StackNode* division(Stack* s) {//나눗셈
	StackNode* ret = (StackNode*)malloc(sizeof(Node));
	ret->isAllocated = TRUE;
	StackNode* firstnode = pop(s);
	int calcResult;
	switch (firstnode->type)
	{
	case SYMBOL:
		calcResult = calcsym(s, firstnode);
		if (calcResult == TRUE)
		{
			ret->number = pop(s)->number;
		}
		else
			error();
		break;
	case NUMBER:
		ret->number = firstnode->number;
		break;
	}
	while (StackEmpty(s)==FALSE)
	{
		StackNode* node = pop(s);
		if (node->type == NUMBER)
		{
			ret->number *= node->number;

		}
		else if (node->type == SYMBOL)
		{
			calcResult = calcsym(s, node);
			if (calcResult == FALSE)
				break;
			else {
				ret->number /= pop(s)->number;
			}
		}
		else {
			ret->type = ERROR_TYPE;
			break;
		}
	}
	ret->type = NUMBER;
	return ret;
}

void setq(Stack * s){ //sETQ

	if (s->top != 1) //SETQ는 인자 두개를 취하므로 인자 두개가 아니면 에러를 발생시킴
		error();
	else {
		StackNode* var = pop(s); //variable symbol
		StackNode* val = pop(s); //variable's value(INT_LIT, FLOAT_LIT OR LIST)
		Var* v = (Var*)malloc(sizeof(Var)); //기존에 저장되지 않았던 변수가 setq의 인자로 들어왔을 때 그 변수를 저장하기 위한 노드
		int exist;
		exist = isVarExist(var->str);
		if (exist != NOT_EXIST)
		{
			switch (val->type)
			{
			case NUMBER:
				vaList.vars[exist]->number = val->number;
				vaList.vars[exist]->type = val->type;
				break;
			case LIST:
				break;
			default:
				error();
			}
		}
		else {
			switch (val->type) {
			case NUMBER:
				v->number = val->number;
				v->type = val->type;
				strcpy(v->str, var->str);
				int valNum = vaList.varlength;
				vaList.vars[valNum] = v;
				(vaList.varlength)++;
				break;
			case LIST:
				break;
			default:
				error();

			}
		}

	}
}

int isVarExist(char str []) { //인자로 받은 변수의 이름을 가진 변수가 존재하는지 체크
	int i;
	int Exist = NOT_EXIST;
	if (vaList.varlength == 0)
		Exist = NOT_EXIST;
	else {
		for (i = 0; i < vaList.varlength; i++)
		{
			if (!strcmp(vaList.vars[i]->str, str))
			{
				/*if (vaList.vars[i]->type == NUMBER)
				{
					StackNode* a = (StackNode*)malloc(sizeof(StackNode));
					a->isAllocated = TRUE;
					a->type = NUMBER;
					a->number = vaList.vars[i]->number;
					push(&st, a);
				}
				else if (vaList.vars[i]->type == LIST)
				{

				}*/
				Exist = i;
				break;
			}

		}
	}
	return Exist;
}

void freeStack(Stack* a) { //스택을 해제하는 함수

	int i;
	for (i = 99; i >= 0; i--)
	{
		if (st.stk[i] != NULL&&st.stk[i]->isAllocated==TRUE) {
			free(st.stk[i]);
			st.stk[i] = NULL;
		}
	}
	a->top = -1;
}

