
#include "func.h"
#include <stdlib.h>
/* Global Variable */
int nextToken;
int charClass; //������ Ÿ��
char expr[100]; //ǥ����

char nextChar; //
int lexLen; //������ ���� //

FILE* in_fp; //LISP ��ɾ� ��ϵ��� �о���� ���� ���� ������

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
    } //���� ���� ���н� ���� �޽��� ���

 
    else {
       
        AST* ast = (AST*)malloc(sizeof(AST)); //�߻� ���� Ʈ�� ����
        varInit(); //�������� �����ϱ� ���� ����Ʈ�� �ʱ�ȭ�Ѵ�.
        plexeme = fgets(expr, sizeof(expr), in_fp); //���Ϸκ��� �� ���� �о�ͼ� char ������ plexeme�� �����Ѵ�.
        getChar(); //�о�� �� �ٿ��� ���� �ϳ��� ������ nextChar�� �����Ѵ�.
         do {
            
             Node* newNode = (Node*)malloc(sizeof(Node)); //�߻� ���� Ʈ���� �� ��� �ϳ��� �����Ѵ�. �� ���� �߻� ���� Ʈ���� ��Ʈ�̴�.
             newNode->isAllocated = TRUE; /*�����Ҵ��� ���� �������� ������ ���߿� �� ������ ����� ������ �߻��Ѵ�.
                                         ���� �����Ҵ��� �Ǿ����� ���θ� �˰� �ϱ� ���Ͽ� Node ����ü�� ��������� isAllocated�� �־���. */
             if (newNode && ast) //�����Ҵ��� ���� �����Ҵ��� �߻󱸹�Ʈ���� ��� NULL�� �ƴ� ���
                 ast->root = newNode; //�߻� ���� Ʈ���� ��Ʈ�� ��带 �Ҵ�
            // initTree(ast->root);
             if (ast) { //�߻󱸹� Ʈ���� NULL�� �ƴϸ�
                 lex();//lex �۾��� ����
                 expression(ast->root); //���Ϸκ��� �о�� �� ��(expression)�� ���� �Ľ� �۾��� ����
                 eval(ast); //�Ľ��� ��� ������� Ʈ���κ��� ������� ���
                 plexeme = fgets(expr, sizeof(expr), in_fp); //���Ϸκ��� ���ο� ���ڿ��� �о��
                 freeTree(ast->root); //�߻� ���� Ʈ���� �� �ڽ� ���鿡 ���Ͽ� ���� �۾� ����
                 freeStack(&st); //�߻� ���� Ʈ���� �о�鿩 ������ ������ ����
             }
             else {
                 error(); //���� �߻󱸹� Ʈ���� NULL�̸� ���� �޽��� ���
                 exit(1);
             }
             

         } while (nextToken != EOF&&plexeme!=NULL); //���Ϸκ��� �о�� ���ڿ��� NULL�� �ƴϰ� ���� �ϳ��� �о��� �� �� ���ڰ� ������ ���� �ƴ� ��
        
     
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
void addChar() { //lexeme �迭�� ���� �߰�
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
        else if (nextChar == '(' || nextChar == ')' || nextChar == ' ' || nextChar == '\n') //���ڰ� (�� )�� �����̳� ���๮���� ���
            charClass = UNKNOWN; //������ Ÿ���� unknown���� ����
        else if (nextChar == '\0') //�� ������ ���
            charClass = UNKNOWN; //������ Ÿ���� UNKNOWN���� ����

        else
            charClass = LETTER; //�������� �ƴϰ� (�� �ƴϰ� )�� �ƴϰ� �����̳� ���๮�ڵ� �ƴ� ��� ������ Ÿ���� ���ڷ� ����

    }
    else {
        charClass = EOF; //������ Ÿ���� EOF�� ����
    }
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it
           returns a non-whitespace character        */
           /*****************************************************/
void getNonBlank() { //������ ����
    while (isspace(nextChar))
        getChar();
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic
         expressions                                 */
         /*****************************************************/
int lex() { //���Ϸκ��� �о�� ���ڿ��� ���Ͽ� ��ūȭ �۾� ����
    lexLen = 0;
    getNonBlank();

    switch (charClass) {
        /* Parse identifiers */
    case LETTER: //������ ���

        addChar();
        getChar();
        while (charClass == LETTER || charClass == DIGIT) { //�������� ���ڷ� ������ ���� a123�� ���� �������� ���� �� �����Ƿ� ������ ���� lexeme�迭�� ���ڸ� �޴´�.
            addChar();
            getChar();
        }
        nextToken = IDENT; //��ū�� Ÿ���� ����
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
    case DIGIT: //�������� ���
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
        nextToken = INT_LIT; //���� ���ͷ�
       
        break;
    
        /* Parentheses and operators */
    case SPACE_BAR: //���� ������ ��� �ƹ� ���۵� ���� ���� lexeme���� �߰��� ���� �ʴ´�.
        break;
    case UNKNOWN: //UNKNOWN Ÿ���� ������ ���
        lookup(nextChar); //lookup �Լ� ���� switch������ ã�ƺ��� ��ū�� ��ȯ
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

void expression(Node * root) { //���Ϸκ��� �о�� ���ڿ� �ϳ��� ���� �Ľ� ����
    
    root->childnum = 0; //�߻� ���� Ʈ���� ��Ʈ�� �ڽ��� ������ 0
    if (nextToken == LEFT_PAREN) //���� ���� ��ȣ�� ������
    {
        root->type = ROOT;
        lex();
        func(root,0); //���� ��ȣ �տ� QUOTE(')�� �پ����� ���� �̻� LISP������ �Լ� ȣ���� �ǹ��ϹǷ� �Լ��� ���� �κ��� �Ľ��� �����ϵ��� �Ѵ�.
    }

    else if (nextToken == QUOTE) //���� ��ū�� QUOTE(')�̸� 
    {
        root->type = QUOTE; //��Ʈ�� Ÿ���� quote
        quote(root); //quote�� ���ϴ� ���� ���� �Ľ� (quote�� ����Ʈ Ȥ�� atom�� ������ ���� �� ����)
    }
    else if (nextToken == INT_LIT || nextToken == IDENT||nextToken==FLOAT_LIT) //��ū�� �����ų�, identifier�ų� �Ǽ��ΰ��
    {

        if (nextToken == INT_LIT||nextToken==FLOAT_LIT) { //���� Ȥ�� �Ǽ���
            root->type = NUMBER; //��Ʈ�� �����ϰ� �ִ� ���� Ÿ���� ����
            root->number = atof(lexeme); //��Ʈ�� ���ڸ� lexeme�� ���ڿ��� ���ڷ� ��ȯ�ϴ� �Լ� atof�� ����Ͽ� ����
        }
        else if (nextToken == IDENT) { //������
            root->type = SYMBOL;//��Ʈ�� Ÿ���� �ɺ� (���⼭ SYMBOL�̳� IDENT�� �ο��� ���� �ٸ����� �ǹ̴� ����ϴ�.)
            strcpy(root->str, lexeme);
        }
    
    }

  
    
    //return root;
}

void func(Node* root,int childnum) //�Լ� ȣ�⿡ ���� �κ��� �Ľ� ��Ģ���꿡 ���ؼ��� �����Ǿ��� ����Ʈ�� ���ڷ� ���� �Լ��� ���ؼ��� ó������ ������
{
    root->childnum = childnum;
   //lex();
   
    while (nextToken != RIGHT_PAREN) //������ ��ȣ�� ���� ������ 
    {
        if (nextToken == IDENT)//������� �� ������ ���� ��带 ���� ����,
        {
            Node* newNode = (Node*)malloc(sizeof(Node));
           // initTree(newNode);
            if (newNode) {
                newNode->isAllocated = TRUE;
                newNode->type = SYMBOL; //�̴� ��尡 ���ϴ� ���� Ÿ���� �ɺ����� �ǹ��Ѵ�. (���ڰ� �ƴ�)
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

void quote(Node* root) //QUOTE�� ���� �κ��� �Ľ� ����
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

void list(Node* root) //QUOTE ������ ���� ��ȣ�� ���� ���� ����Ʈ�� �ǹ���. �� �Լ��� list�� ���� �κ��� �Ľ��� �Ѵ�. (�ڵ�� �ۼ� �Ǿ�����, ����� �� ��� ���� �Ұ�)
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

void eval(AST* tree) //�Ľ� �Լ��� ���� ������ �߻� ���� Ʈ���� �������� ��� ���
{

    int i;
    initStack(&st); //�߻� ���� Ʈ���� �ִ� ������ �����ϱ� ���� ������ �ʱ�ȭ
    Node* root = tree->root;
    traverse(root); //�߻� ���� Ʈ������ �� ������ leaf node���� ���� leaf node�� ��ȸ�� ���� �� ���� ������ Ž���ϸ鼭 �� �߻� ���� Ʈ���� ���� ���� ���ÿ� push��

    initStack(&params);
    int resultType = NUMBER;

    if (root->type == ROOT){ //root�� Ÿ���� ROOT�� ��� -> �̴� root node�� �� ���������� �ǹ��� (���� ���������� ���� ���� ����)
        while (StackEmpty(&st) == FALSE) //������ ������� ���� ���
        {
            StackNode* s = pop(&st); //�߻� ���� Ʈ���� ���� ������ ���ÿ��� �ϳ��� ���Ѵ�.
            if (s->type == SYMBOL)
            {
                if (!strcmp(s->str, "+"))//���� �� ���� +���
                {
                    StackNode* a = add(&params); /*���� �Լ��� �����Ѵ�. -> ���� ��� stack �� + 2 3 �� ���� �������� ����Ǿ��� ��� add�� ���� �Ǹ� 2�� 3 �׸��� +�� pop�ǰ� 
                                                 ������� 5�� ���ÿ� push�ȴ�*/
                    
                    push(&st, a);//�� ���� ���� �Լ��� ������ ����� stack�� push�Ѵ�.
                    initStack(&params);
                    if (st.top == 0) {
                        resultType = NUMBER;
                        break;
                    }
                }
                else if (!strcmp(s->str, "-")) {/*�����Լ��� �����Ѵ�. ���� �Լ��� ���� ������ ���� ���������� ����ȴ�.*/
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
                    initStack(&params); //params�� �Լ��� ���ڸ� �����ϱ� ���� �����̴�.
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

                else if (!strcmp(s->str, "SETQ")) //SETQ�Լ� ����
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


Node* traverse(Node* root) { /*�߻� ���� Ʈ���� ��带 �� �Ʒ� ������ ���� ������ leaf ������ ���� leaf ������ ��ȸ�� 
                             ���� ���� ������ �ö󰡼� ������ ��� ���� ���� ��� ������ ��� ��ȸ�ϸ鼭 �� Ʈ���� ��尡 ���� �մ� ���� stack�� push*/

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


void freeTree(Node* root) { //Ʈ���� ���� ���� ����
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








