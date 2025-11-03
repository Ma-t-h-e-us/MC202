#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct Pair
{
    double valor;
    double derivada; 
} Pair;

typedef struct No {
    Pair data;
    struct No* next;
} No;

typedef struct {
    No* top;
} Stack;

Stack* createStack(){
    Stack* s;
    size_t bytes = sizeof(Stack); //tamanho da esturura em bytes
    void* ptr = malloc(bytes);
    s = (Stack*) ptr; //convertendo o ponteiro genérico para Stack
    s->top = NULL;
    return s;
}

int isEmpty(Stack* s) {
    if (s->top == NULL) return 1; else return 0;
}

void push(Stack* s, Pair p){
    No* novoNo;
    size_t bytes = sizeof(No);
    void* ptr = malloc(bytes);
    novoNo = (No*) ptr;
    novoNo->data = p;
    novoNo->next = s->top;
    s->top = novoNo;
}

Pair pop(Stack* s){
    if (isEmpty(s) == 1)
    {
        printf("Erro: pilha vazia.\n");
    }
    No* temp = s->top;
    Pair p = temp->data;
    s->top = temp->next;
    free(temp);
    return p;
}

void freeStack(Stack* s){
    while (isEmpty(s) == 0)
    {
        pop(s);
    }
    free(s);
}

int main()
{
    //lendo e armazenando os valorores de entrada
    char function[256];
    double x0;

    fgets(function, sizeof(function), stdin);
    scanf("%lf", &x0);

    Stack* stack = createStack();
    char* token = strtok(function, " \n");

    while (token != NULL) {
        Pair a, b, res;

        switch (token[0]) {
            case '+':
                b = pop(stack);
                a = pop(stack);
                res.valor = a.valor + b.valor;
                res.derivada = a.derivada + b.derivada;
                push(stack, res);
                break;

            case '-':
                b = pop(stack);
                a = pop(stack);
                res.valor = a.valor - b.valor;
                res.derivada = a.derivada - b.derivada;
                push(stack, res);
                break;

            case '*':
                b = pop(stack);
                a = pop(stack);
                res.valor = a.valor * b.valor;
                res.derivada = a.derivada * b.valor + a.valor * b.derivada;
                push(stack, res);
                break;

            case '/':
                b = pop(stack);
                a = pop(stack);
                res.valor = a.valor / b.valor;
                res.derivada = (a.derivada * b.valor - a.valor * b.derivada) / (b.valor * b.valor);
                push(stack, res);
                break;

            case '^':
                b = pop(stack);
                a = pop(stack);
                res.valor = pow(a.valor, b.valor);
                res.derivada = b.valor * pow(a.valor, b.valor - 1) * a.derivada;
                push(stack, res);
                break;

            default:
                if (token[0] == 'x') {
                    res.valor = x0;
                    res.derivada = 1.0;
                } else {
                    res.valor = atof(token);
                    res.derivada = 0.0;
                }
                push(stack, res);
                break;
        }

        token = strtok(NULL, " \n");
    }

    if (isEmpty(stack)) {
        printf("Erro: expressão inválida.\n");
    }

    Pair result = pop(stack);
    printf("%.5lf\n", result.valor);
    printf("%.5lf\n", result.derivada);

    freeStack(stack);
    
    return 0;
}
