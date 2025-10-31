#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    double val;   // valor da expressão parcial
    double der;   // derivada parcial em relação a x
} Pair;

typedef struct Node {
    Pair data;
    struct Node* next;
} Node;

typedef struct {
    Node* top;
} Stack;

// Funções da pilha
Stack* createStack() {
    Stack* s = (Stack*) malloc(sizeof(Stack));
    s->top = NULL;
    return s;
}

int isEmpty(Stack* s) {
    return s->top == NULL;
}

void push(Stack* s, Pair p) {
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->data = p;
    newNode->next = s->top;
    s->top = newNode;
}

Pair pop(Stack* s) {
    if (isEmpty(s)) {
        fprintf(stderr, "Erro: pilha vazia.\n");
        exit(EXIT_FAILURE);
    }
    Node* temp = s->top;
    Pair p = temp->data;
    s->top = temp->next;
    free(temp);
    return p;
}

void freeStack(Stack* s) {
    while (!isEmpty(s)) {
        pop(s);
    }
    free(s);
}

// Função principal de avaliação e diferenciação
int main() {
    char expr[256];
    double x0;

    // Lê a expressão e o valor de x0
    fgets(expr, sizeof(expr), stdin);
    scanf("%lf", &x0);

    Stack* stack = createStack();
    char* token = strtok(expr, " \n");

    while (token != NULL) {
        Pair a, b, res;

        if (strcmp(token, "x") == 0) {
            res.val = x0;
            res.der = 1.0;
            push(stack, res);
        } 
        else if (strcmp(token, "+") == 0) {
            b = pop(stack);
            a = pop(stack);
            res.val = a.val + b.val;
            res.der = a.der + b.der;
            push(stack, res);
        } 
        else if (strcmp(token, "-") == 0) {
            b = pop(stack);
            a = pop(stack);
            res.val = a.val - b.val;
            res.der = a.der - b.der;
            push(stack, res);
        } 
        else if (strcmp(token, "*") == 0) {
            b = pop(stack);
            a = pop(stack);
            res.val = a.val * b.val;
            res.der = a.der * b.val + a.val * b.der;
            push(stack, res);
        } 
        else if (strcmp(token, "/") == 0) {
            b = pop(stack);
            a = pop(stack);
            res.val = a.val / b.val;
            res.der = (a.der * b.val - a.val * b.der) / (b.val * b.val);
            push(stack, res);
        } 
        else if (strcmp(token, "^") == 0) {
            b = pop(stack); // expoente
            a = pop(stack); // base
            res.val = pow(a.val, b.val);
            res.der = b.val * pow(a.val, b.val - 1) * a.der;
            push(stack, res);
        } 
        else {
            // token é uma constante numérica
            double c = atof(token);
            res.val = c;
            res.der = 0.0;
            push(stack, res);
        }

        token = strtok(NULL, " \n");
    }

    if (isEmpty(stack)) {
        fprintf(stderr, "Erro: expressão inválida.\n");
        exit(EXIT_FAILURE);
    }

    Pair result = pop(stack);
    printf("%.5lf\n", result.val);
    printf("%.5lf\n", result.der);

    freeStack(stack);
    return 0;
}
