
#include "func.h"
#include <stdlib.h>
/* Global Variable */
int nextToken;
int charClass; //문자의 타입
char expr[100]; //표현식

char nextChar; //
int lexLen; //구문의 길이 //

FILE* in_fp; //LISP 명령어 목록들을 읽어오기 위한 파일 포인터

/* Character classes */



/* Token codes */



/* Local Function declarations */
void addChar();
void getChar();
void getNonBlank();
int lex();
int lookup(char);
void expression(Node*);
void func(Node*, int);
void list(Node*);
void quote(Node*);

void eval(AST*);
void quote(Node* root);
Node* traverse(Node* root);
void freeTree(Node* root);
void freeStack(Stack* a);
//int str_chr(char* s, int c);
char lexeme[100];
char* plexeme;


char expr[100];
/*typedef struct Tokens {
    int tokenType;
    char* tokens;
}Tokens;


Tokens* tokens;
int tokenLen = 0;*/

/******************************************/
/* main driver                            */
/******************************************/
int main()
{
    /* Open the input data file and process its contents */
    if ((in_fp = fopen("code.in", "r")) == NULL) {
        printf("ERROR - cannot open front.in \n");
    } //파일 오픈 실패시 에러 메시지 출력

 
    else {
       
        AST* ast = (AST*)malloc(sizeof(AST)); //추상 구문 트리 생성
        varInit(); //변수들을 저장하기 위한 리스트를 초기화한다.
        plexeme = fgets(expr, sizeof(expr), in_fp); //파일로부터 한 줄을 읽어와서 char 포인터 plexeme에 저장한다.
        getChar(); //읽어온 한 줄에서 문자 하나를 가져와 nextChar에 저장한다.
         do {
            
             Node* newNode = (Node*)malloc(sizeof(Node)); //추상 구문 트리에 들어갈 노드 하나를 생성한다. 이 노드는 추상 구문 트리의 루트이다.
             newNode->isAllocated = TRUE; /*동적할당한 값을 해제하지 않으면 나중에 힙 영역을 벗어나서 에러가 발생한다.
                                         따라서 동적할당이 되었는지 여부를 알게 하기 위하여 Node 구조체의 멤버변수로 isAllocated를 넣었다. */
             if (newNode && ast) //동적할당한 노드와 동적할당한 추상구문트리가 모두 NULL이 아닐 경우
                 ast->root = newNode; //추상 구문 트리의 루트에 노드를 할당
            // initTree(ast->root);
             if (ast) { //추상구문 트리가 NULL이 아니면
                 lex();//lex 작업을 시작
                 expression(ast->root); //파일로부터 읽어온 한 줄(expression)에 대한 파싱 작업을 수행
                 eval(ast); //파싱한 결과 만들어진 트리로부터 결과값을 계산
                 plexeme = fgets(expr, sizeof(expr), in_fp); //파일로부터 새로운 문자열을 읽어옴
                 freeTree(ast->root); //추상 구문 트리와 그 자식 노드들에 대하여 해제 작업 진행
                 freeStack(&st); //추상 구문 트리를 읽어들여 저장한 스택을 해제
             }
             else {
                 error(); //만약 추상구문 트리가 NULL이면 에러 메시지 출력
                 exit(1);
             }
             

         } while (nextToken != EOF&&plexeme!=NULL); //파일로부터 읽어온 문자열이 NULL이 아니고 문자 하나를 읽었을 때 그 문자가 파일의 끝이 아닐 때
        
     
    }

    return 0;
}

int lookup(char ch) {
    switch (ch) {
    case '(':
        addChar();
        nextToken = LEFT_PAREN;
        break;
    case ')':
        //addChar();
        nextToken = RIGHT_PAREN;
        break;
   /* case '+':
        addChar();
        nextToken = ADD_OP;
        break;
    case '-':
        addChar();
        nextToken = SUB_OP;
        break;
    case '*':
        addChar();
        nextToken = MULT_OP;
        break;
    case '/':
        addChar();
        nextToken = DIV_OP;
        break;*/
    case 39:
        addChar();
        nextToken = QUOTE;
        break;
    case ' ':
        nextToken = SPACE_BAR;
        break;
    case '\n':
        nextToken = NEW_LINE;
        break;
    case '\0':
        nextToken = NIL;
    default:
        addChar();
        nextToken = EOF;
        break;
    }
    return nextToken;
}

/**************************************************/
/* addChar - a function to add nextChar to lexeme */
/**************************************************/
void addChar() { //lexeme 배열에 문자 추가
    if (lexLen <= 98) {  // max length of Lexime is 99
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0; // '\0'
    }
    else {
        printf("Error - lexeme is too long \n");
    }
}

/*****************************************************/
/* getChar - a function to get the next character
          of input and determine its character class */
          /*********************************
          ********************/
void getChar() {

    nextChar = *(plexeme)++;
    if (nextChar!= EOF) {
        if (isdigit(nextChar))
            charClass = DIGIT;
        else if (nextChar == '(' || nextChar == ')' || nextChar == ' ' || nextChar == '\n') //문자가 (나 )나 공백이나 개행문자일 경우
            charClass = UNKNOWN; //문자의 타입을 unknown으로 지정
        else if (nextChar == '\0') //널 문자일 경우
            charClass = UNKNOWN; //문자의 타입을 UNKNOWN으로 지정

        else
            charClass = LETTER; //십진수도 아니고 (도 아니고 )도 아니고 공백이나 개행문자도 아닌 경우 문자의 타입을 문자로 지정

    }
    else {
        charClass = EOF; //문자의 타입을 EOF로 지정
    }
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it
           returns a non-whitespace character        */
           /*****************************************************/
void getNonBlank() { //공백을 무시
    while (isspace(nextChar))
        getChar();
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic
         expressions                                 */
         /*****************************************************/
int lex() { //파일로부터 읽어온 문자열에 대하여 토큰화 작업 진행
    lexLen = 0;
    getNonBlank();

    switch (charClass) {
        /* Parse identifiers */
    case LETTER: //문자인 경우

        addChar();
        getChar();
        while (charClass == LETTER || charClass == DIGIT) { //변수명은 문자로 시작한 다음 a123과 같은 형식으로 사용될 수 있으므로 다음과 같이 lexeme배열에 문자를 받는다.
            addChar();
            getChar();
        }
        nextToken = IDENT; //토큰의 타입은 변수
        /*addChar();
        if (nextChar == '-' && !strcmp(lexeme, "-"))
        {
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            int isNum = TRUE;
            int i;
            for (i = 1; i < lexLen; i++)
            {
                if (!isdigit(lexeme[i]))
                {
                    isNum = FALSE;
                    break;
                }
            }

            if (isNum == FALSE)
            {
                if (str_chr(lexeme, '.') == 1) {
                    char* dotPos;
                    int dotIdx;
                    dotPos = strchr(lexeme, '.');
                    dotIdx = (int)(dotPos - lexeme);
                    if (dotIdx < lexLen)
                    {
                        int isNum = TRUE;
                        for (i = 1; i < dotIdx; i++)
                        {
                            if (!isdigit(lexeme[i]))
                            {
                                isNum = FALSE;
                                break;
                            }
                        }
                        for (i = dotIdx + 1; i < lexLen; i++)
                        {
                            if (!isdigit(lexeme[i]))
                            {
                                isNum = FALSE;
                                break;
                            }
                        }

                        if (isNum == TRUE)
                            nextToken = FLOAT_LIT;
                        else if (isNum == FALSE)
                            nextToken = IDENT;
                    }
                }
                else
                    nextToken = IDENT;

            }
            else if(isNum==FALSE)
                nextToken = INT_LIT;

        }
      
       
        else {
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = IDENT;
        }
        */
        break;
        /* Parse integer literals */
    case DIGIT: //십진수인 경우
       /* addChar();
        getChar();
        while (charClass == DIGIT||charClass==IDENT) {
            addChar();
            getChar();
        }

        int isNum = TRUE;
        int i;
        for (i = 0; i < lexLen; i++)
        {
            if (!isdigit(lexeme[i]))
            {
                isNum = FALSE;
                break;
            }
        }

        if (isNum == TRUE)
            nextToken = INT_LIT;
        else if (isNum == FALSE)
        {
            if (str_chr(lexeme, '.') == 1) {
                char* dotPos;
                int dotIdx;
                dotPos = strchr(lexeme, '.');
                dotIdx = (int)(dotPos - lexeme);
                if (dotIdx < lexLen)
                {
                    int isNum = TRUE;
                    for (i = 0; i < dotIdx; i++)
                    {
                        if (!isdigit(lexeme[i]))
                        {
                            isNum = FALSE;
                            break;
                        }
                    }
                    for (i = dotIdx + 1; i < lexLen; i++)
                    {
                        if (!isdigit(lexeme[i]))
                        {
                            isNum = FALSE;
                            break;
                        }
                    }

                    if (isNum == TRUE)
                        nextToken = FLOAT_LIT;
                    else if (isNum == FALSE)
                        nextToken = IDENT;
                }
            }
        }




        nextToken = INT_LIT;
        */
        addChar();
        getChar();
        while (charClass == DIGIT) {
            addChar();
            getChar();
        }
        nextToken = INT_LIT; //정수 리터럴
       
        break;
    
        /* Parentheses and operators */
    case SPACE_BAR: //공백 문자의 경우 아무 동작도 하지 않음 lexeme에다 추가도 하지 않는다.
        break;
    case UNKNOWN: //UNKNOWN 타입의 문자인 경우
        lookup(nextChar); //lookup 함수 내의 switch문에서 찾아보고 토큰을 반환
        getChar();
        break;
        /* EOF */
    case EOF:
        nextToken = EOF;
      //  lexeme[0] = 'E';
    //    lexeme[1] = 'O';
      //  lexeme[2] = 'F';
       // lexeme[3] = 0;
        break;
    } /* End of switch */
   // printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);
    return nextToken;
} /* End of function lex */



/*int str_chr(char* s, int c) {

    int i;

    int count = 0;

    for (i = 0; i < strlen(s); i++) {

        if (s[i] == c)

            count++;

    }
    return count;

}*/

void expression(Node * root) { //파일로부터 읽어온 문자열 하나에 대한 파싱 시작
    
    root->childnum = 0; //추상 구문 트리의 루트의 자식의 개수는 0
    if (nextToken == LEFT_PAREN) //만약 왼쪽 괄호가 나오면
    {
        root->type = ROOT;
        lex();
        func(root,0); //왼쪽 괄호 앞에 QUOTE(')가 붙어있지 않은 이상 LISP에서는 함수 호출을 의미하므로 함수에 대한 부분적 파싱을 수행하도록 한다.
    }

    else if (nextToken == QUOTE) //만약 토큰이 QUOTE(')이면 
    {
        root->type = QUOTE; //루트의 타입은 quote
        quote(root); //quote가 취하는 값에 대한 파싱 (quote는 리스트 혹은 atom을 값으로 취할 수 있음)
    }
    else if (nextToken == INT_LIT || nextToken == IDENT||nextToken==FLOAT_LIT) //토큰이 정수거나, identifier거나 실수인경우
    {

        if (nextToken == INT_LIT||nextToken==FLOAT_LIT) { //정수 혹은 실수면
            root->type = NUMBER; //루트가 저장하고 있는 값의 타입은 숫자
            root->number = atof(lexeme); //루트의 숫자를 lexeme의 문자열을 숫자로 변환하는 함수 atof로 계산하여 대입
        }
        else if (nextToken == IDENT) { //변수면
            root->type = SYMBOL;//루트이 타입은 심볼 (여기서 SYMBOL이나 IDENT에 부여된 값은 다르지만 의미는 비슷하다.)
            strcpy(root->str, lexeme);
        }
    
    }

  
    
    //return root;
}

void func(Node* root,int childnum) //함수 호출에 대한 부분적 파싱 사칙연산에 대해서만 구현되었고 리스트를 인자로 갖는 함수에 대해서는 처리하지 못했음
{
    root->childnum = childnum;
   //lex();
   
    while (nextToken != RIGHT_PAREN) //오른쪽 괄호가 나올 때까지 
    {
        if (nextToken == IDENT)//변수라면 그 변수에 대한 노드를 만들어서 저장,
        {
            Node* newNode = (Node*)malloc(sizeof(Node));
           // initTree(newNode);
            if (newNode) {
                newNode->isAllocated = TRUE;
                newNode->type = SYMBOL; //이는 노드가 취하는 값의 타입이 심볼임을 의미한다. (숫자가 아님)
                newNode->childnum = 0;
                strcpy(newNode->str, lexeme);
                newNode->parent = root;
                int num = root->childnum;
                root->child[num] = newNode;
                (root->childnum)++;
            }
            else
            {
                printf("Allocation error!!!");
                break;
            }
        }
        else if (nextToken == INT_LIT||nextToken==FLOAT_LIT)
        {
            Node* newNode = (Node*)malloc(sizeof(Node));
           // initTree(newNode);
            if (newNode) {
                newNode->isAllocated = TRUE;
                newNode->type = NUMBER;
                newNode->childnum = 0;
                newNode->number = atof(lexeme);
                int num = root->childnum;
                root->child[num] = newNode;
                (root->childnum)++;
            }
            else
            {
                printf("Allocation error!!!");
                break;
            }
        }
        else if (nextToken == QUOTE)
        {
            int num = root->childnum;
            quote(root->child[num]);
        }
        else if (nextToken == LEFT_PAREN)
        {
            lex();
            func(root,root->childnum);
                
        }

        lex();
    }
  
}

void quote(Node* root) //QUOTE에 대한 부분적 파싱 시작
{
    root->childnum = 0;
    lex();

    if (nextToken == LEFT_PAREN)
    {
        list(root->child[root->childnum]);

    }
    else if (nextToken == IDENT || nextToken == INT_LIT||nextToken==FLOAT_LIT)
    {
        Node* newNode = (Node*)malloc(sizeof(Node));
        //initTree(newNode);
        if (newNode) {
            if (nextToken == IDENT)
            {
                newNode->isAllocated = TRUE;
                newNode->type = SYMBOL;
                strcpy(newNode->str, lexeme);
                newNode->parent = root;
                root->child[root->childnum] = newNode;
                (newNode->childnum)++;
            }
            else if (nextToken == INT_LIT||nextToken==FLOAT_LIT)
            {
                newNode->isAllocated = TRUE;
                newNode->type = SYMBOL;
                strcpy(newNode->str, lexeme);
                newNode->parent = root;  
                root->child[root->childnum] = newNode;
                (newNode->childnum)++;
            }
        }
        else
            fprintf(stderr, "Allocation error!!");
    }
 
}

void list(Node* root) //QUOTE 다음에 왼쪽 괄호가 왔을 때는 리스트를 의미함. 이 함수는 list에 대한 부분적 파싱을 한다. (코드는 작성 되었으나, 제대로 된 결과 수행 불가)
{
    root->childnum = 0;
    root->type = LIST;
    lex();
    List* l1 = (List*)malloc(sizeof(List));

 
    while (nextToken != RIGHT_PAREN) {

        if (nextToken == LEFT_PAREN)
        {
            list(root->child[root->childnum]);
        }
        else if (nextToken == IDENT)
        {
            Node* newNode = (Node*)malloc(sizeof(Node));
           // initTree(newNode);
            if (newNode) {
                newNode->isAllocated = TRUE;
                newNode->type = SYMBOL;
                strcpy(newNode->str, lexeme);
                newNode->parent = root;
                root->child[root->childnum] = newNode;
                (root->childnum)++;
            }
            else {
                error();
                break;
            }
        }
        else if (nextToken == INT_LIT||nextToken==FLOAT_LIT) {

            Node* newNode = (Node*)malloc(sizeof(Node));
          //  initTree(newNode);
            if (newNode)
            {
                newNode->isAllocated = TRUE;
                newNode->type = NUMBER;
                newNode->number = atof(lexeme);
                newNode->parent = root;
                root->child[root->childnum] = newNode;
                (root->childnum)++;
            }
            else
            {
                error();
                break;
            }
        }
       /* else if (nextToken == QUOTE)
        {

        }*/
        lex();
    }

    int i;
    Node* traverse = root;
   
    //ListNode* tail = l1->head;

    for (i = 0; i < root->childnum; i++)
    {
        Node* child = traverse->child[i];
        ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
        if (child->type == SYMBOL)
        {
            strcpy(newNode->str, lexeme);
            newNode->valtype = SYMBOL;
        }
        else if (child->type == NUMBER)
        {
            newNode->number = child->number;
            newNode->valtype = NUMBER;
        }
        else if (child->type == LIST)
        {
            root->list = child->list;
        }
    }
}

void eval(AST* tree) //파싱 함수를 통해 생성된 추상 구문 트리를 바탕으로 결과 계산
{

    int i;
    initStack(&st); //추상 구문 트리에 있던 값들을 저장하기 위한 스택을 초기화
    Node* root = tree->root;
    traverse(root); //추상 구문 트리에서 맨 오른쪽 leaf node부터 왼쪽 leaf node로 순회한 다음 그 위의 레벨로 탐색하면서 각 추상 구문 트리가 갖는 값을 스택에 push함

    initStack(&params);
    int resultType = NUMBER;

    if (root->type == ROOT){ //root의 타입이 ROOT인 경우 -> 이는 root node가 빈 껍데기임을 의미함 (노드는 존재하지만 값은 갖지 않음)
        while (StackEmpty(&st) == FALSE) //스택이 비어있지 않은 경우
        {
            StackNode* s = pop(&st); //추상 구문 트리의 값을 저장한 스택에서 하나를 팝한다.
            if (s->type == SYMBOL)
            {
                if (!strcmp(s->str, "+"))//만약 그 값이 +라면
                {
                    StackNode* a = add(&params); /*덧셈 함수를 수행한다. -> 예를 들어 stack 에 + 2 3 과 같은 형식으로 저장되었을 경우 add가 수행 되면 2와 3 그리고 +가 pop되고 
                                                 결과값인 5가 스택에 push된다*/
                    
                    push(&st, a);//그 다음 덧셈 함수를 수행한 결과를 stack에 push한다.
                    initStack(&params);
                    if (st.top == 0) {
                        resultType = NUMBER;
                        break;
                    }
                }
                else if (!strcmp(s->str, "-")) {/*뺄셈함수를 수행한다. 뺄셈 함수도 위의 덧셈의 예와 마찬가지로 수행된다.*/
                    StackNode* a = sub(&params);
                    push(&st, a);
                    initStack(&params);
                    if (st.top == 0) {
                        resultType = NUMBER;
                        break;
                    }
                }
                else if (!strcmp(s->str, "*")) {
                    StackNode* a = mult(&params);
                    push(&st, a);
                    initStack(&params); //params는 함수의 인자를 저장하기 위한 스택이다.
                    if (st.top == 0) {
                        resultType = NUMBER;
                        break;
                    }
                }
                else if (!strcmp(s->str, "/")) {
                    StackNode* a = division(&params);
                    push(&st, a);
                    initStack(&params);
                    if (st.top == 0) {
                        resultType = NUMBER;
                        break;
                    }
                }

                else if (!strcmp(s->str, "SETQ")) //SETQ함수 수행
                {
                    setq(&params);
                    resultType = NOTHING;
                }
                else {
                    push(&params, s);
                   
                }

            }
            else if (s->type == NUMBER)
            {
                push(&params, s);
            }

           
        }

    
        switch (resultType)
        {
        case NUMBER:
            if (resultType == NUMBER) {
                StackNode* result = pop(&st);
                if (result->type == NUMBER)
                {
                    printf("%.2lf\n", result->number);
                }
            }
            break;
        case NOTHING:
            break;

        }
     

    }
    else if (root->type == SYMBOL)
    {
        int i;
        if (vaList.varlength == 0)
            error();
        for (i = 0; i < vaList.varlength; i++)
        {
            if (!strcmp(vaList.vars[i]->str, root->str))
            {
                if (vaList.vars[i]->type == NUMBER)
                    printf("%.2lf\n", vaList.vars[i]->number);
                else if (vaList.vars[i]->type == LIST)
                {

                }
            }
        }
    }
    
}


Node* traverse(Node* root) { /*추상 구문 트리의 노드를 맨 아래 레벨의 가장 오른쪽 leaf 노드부터 왼쪽 leaf 노드까지 순회한 
                             다음 다음 레벨로 올라가서 오른쪽 노드 부터 왼쪽 노드 순으로 계속 순회하면서 각 트리의 노드가 갖고 잇는 값을 stack에 push*/

    int i;
    if (root->type == ROOT) {
        if (root->childnum != 0) {
            for (i = 0; i<root->childnum; i++){
                Node* child = root->child[i];
                if (child->childnum != 0)
                    traverse(child);
                else {
                    StackNode* node = (StackNode*)malloc(sizeof(StackNode));
                    node->isAllocated = TRUE;
                    switch (child->type)
                    {
                        case SYMBOL:
                            node->type = SYMBOL;
                            strcpy(node->str, child->str);
                            push(&st,node);
                            break;
                        case NUMBER:
                            node->type = NUMBER;
                            node->number = child->number;
                            push(&st,node);
                            break;
                    }
                }

            }
        }
    }
    else if (root->type == NUMBER)
    {
        StackNode* node = (StackNode*)malloc(sizeof(StackNode));
        node->isAllocated = TRUE;
        node->type = NUMBER;
        node->number = root->number;
        push(&st, node);
        
    }
    else if (root->type == SYMBOL)
    {

    }
}


void freeTree(Node* root) { //트리에 대한 해제 수행
    int i;
    if (root->childnum != 0) {
        for (i = 0; i < root->childnum; i++) {
            Node* child = root->child[i];
            if (child->childnum != 0)
                freeTree(child);
            else {
                if (child != NULL&&child->isAllocated==TRUE) {
                        free(child);
                }
            }

        }
    }
    free(root);
}








