# Considere uma expressão formada somente por numeros de 1 digito 
# e operadores =, -, /, * em notação polonesa. Como no ex: 45-3*47+/ 
# (Corresponde a [(4-5)*3]/(4+7)) Apresente o codigo usando pilha para calcular a expressão

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Definindo a pilha
#define MAX 100

typedef struct {
    float itens[MAX];
    int topo;
} Pilha;

// Função para inicializar a pilha
void inicializarPilha(Pilha* p) {
    p->topo = -1;
}

// Função para verificar se a pilha está vazia
int pilhaVazia(Pilha* p) {
    return p->topo == -1;
}

// Função para empilhar um item na pilha
void empilhar(Pilha* p, float valor) {
    if (p->topo == MAX - 1) {
        printf("Pilha cheia!\n");
        return;
    }
    p->itens[++(p->topo)] = valor;
}

// Função para desempilhar um item da pilha
float desempilhar(Pilha* p) {
    if (pilhaVazia(p)) {
        printf("Pilha vazia!\n");
        return -1; // Valor inválido
    }
    return p->itens[(p->topo)--];
}

// Função para realizar a operação
float calcular(float a, float b, char operador) {
    switch (operador) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        default: return 0; // Caso o operador seja inválido
    }
}

// Função para calcular a expressão em notação polonesa reversa
float calcularExpPolonesa(char* expressao) {
    Pilha p;
    inicializarPilha(&p);

    int i = 0;
    while (expressao[i] != '\0') {
        char ch = expressao[i];

        // Ignorar espaços
        if (ch == ' ' || ch == '\t') {
            i++;
            continue;
        }

        if (isdigit(ch)) {
            // Se for um número de 1 dígito, empilha
            empilhar(&p, ch - '0');  // Converte o caractere para inteiro e empilha
        } else {
            // Se for um operador, realiza a operação
            float b = desempilhar(&p);
            float a = desempilhar(&p);
            float resultado = calcular(a, b, ch);
            empilhar(&p, resultado);
        }
        i++;
    }

    // O resultado final será o único valor que restar na pilha
    return desempilhar(&p);
}

int main() {
    char expressao[MAX]; // Array para armazenar a expressão do usuário

    // Solicita ao usuário que insira uma expressão em notação polonesa reversa
    printf("Digite uma expressao em notacao polonesa reversa (1 digito por numero):\n");
    fgets(expressao, MAX, stdin); // Lê a expressão do usuário

    // Remove a nova linha do final da string, caso exista
    expressao[strcspn(expressao, "\n")] = '\0';

    // Chama a função para calcular o resultado
    float resultado = calcularExpPolonesa(expressao);

    // Exibe o resultado
    printf("Resultado da expressao '%s': %.2f\n", expressao, resultado);

    return 0;
}
