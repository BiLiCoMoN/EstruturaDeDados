#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para a pilha
typedef struct {
    char **elements;
    int top;
    int capacity;
} Stack;

// Fun��es para manipular a pilha
Stack *initializeStack(int capacity);
void resizeStack(Stack *stack);
void push(Stack *stack, const char *value);
char *pop(Stack *stack);
void freeStack(Stack *stack);
void processInput(FILE *inputFile, FILE *outputFile);

// Fun��o para inicializar a pilha
Stack *initializeStack(int capacity) {
    Stack *stack = malloc(sizeof(Stack));
    stack->elements = malloc(capacity * sizeof(char *));
    stack->top = -1;
    stack->capacity = capacity;
    return stack;
}

// Fun��o para redimensionar a pilha
void resizeStack(Stack *stack) {
    stack->capacity *= 2;
    stack->elements = realloc(stack->elements, stack->capacity * sizeof(char *));
}

// Fun��o para adicionar um elemento � pilha
void push(Stack *stack, const char *value) {
    if (stack->top == stack->capacity - 1) {
        resizeStack(stack);
    }
    stack->elements[++stack->top] = strdup(value);
}

// Fun��o para remover um elemento da pilha
char *pop(Stack *stack) {
    if (stack->top == -1) {
        return NULL;
    }
    char *value = stack->elements[stack->top];
    stack->elements[stack->top--] = NULL;
    return value;
}

// Fun��o para liberar a mem�ria da pilha
void freeStack(Stack *stack) {
    for (int i = 0; i <= stack->top; i++) {
        free(stack->elements[i]);
    }
    free(stack->elements);
    free(stack);
}

// Fun��o para processar a entrada e gerar a sa�da
void processInput(FILE *inputFile, FILE *outputFile) {
    Stack *stack = initializeStack(10); // Inicializa a pilha com capacidade inicial
    char value[100];
    int firstOperation = 1; // Flag para o primeiro "push" da linha

    // L� o arquivo de entrada linha por linha
    while (fscanf(inputFile, "%s", value) != EOF) {
        // Insere o valor na pilha e faz os pops necess�rios para manter a ordem
        if (firstOperation) {
            fprintf(outputFile, "push-%s", value);
            firstOperation = 0;
        } else {
            fprintf(outputFile, " push-%s", value);
        }
        push(stack, value);

        // Realiza o "pop" apenas se necess�rio (ordem alfab�tica)
        int popCount = 0;
        while (stack->top >= 0 && strcmp(stack->elements[stack->top], value) > 0) {
            pop(stack);
            popCount++;
        }

        if (popCount > 0) {
            fprintf(outputFile, " %dx-pop", popCount);
        }
    }

    freeStack(stack);
}

int main() {
    FILE *inputFile = fopen("L1Q2.in", "r");  // Nome do arquivo de entrada
    FILE *outputFile = fopen("L1Q2.out", "w"); // Nome do arquivo de sa�da

    if (inputFile == NULL || outputFile == NULL) {
        printf("Erro ao abrir os arquivos\n");
        return 1;
    }

    processInput(inputFile, outputFile);

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
