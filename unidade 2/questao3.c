#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Estrutura para a lista duplamente ligada
typedef struct Node {
    int key;
    struct Node *next;
    struct Node *prev;
    struct CNode *circleList; // Lista simplesmente ligada circular associada
} Node;

// Estrutura para a lista simplesmente ligada circular
typedef struct CNode {
    double value;
    struct CNode *next;
} CNode;

// Inicializa uma lista circular vazia
CNode *initializeCircularList() {
    return NULL;
}

// Insere um valor na lista circular de forma ordenada, respeitando a diferença máxima de 0,99
void insertSortedCircular(CNode **head, double value) {
    if (*head == NULL) {
        CNode *newNode = (CNode *)malloc(sizeof(CNode));
        if (newNode == NULL) {
            fprintf(stderr, "Falha na alocação de memória para um novo CNode.\n");
            exit(1);
        }
        newNode->value = value;
        newNode->next = newNode;
        *head = newNode;
        return;
    }

    CNode *current = *head;
    CNode *prev = NULL;
    do {
        if (fabs(current->value - value) > 0.99) {
            fprintf(stderr, "Erro: Valor %.2f excede a diferença máxima permitida de 0,99.\n", value);
            return;
        }
        prev = current;
        current = current->next;
    } while (current != *head);

    CNode *newNode = (CNode *)malloc(sizeof(CNode));
    if (newNode == NULL) {
        fprintf(stderr, "Falha na alocação de memória para um novo CNode.\n");
        exit(1);
    }
    newNode->value = value;

    // Inserção ordenada
    current = *head;
    prev = NULL;
    do {
        if (current->value >= value) break;
        prev = current;
        current = current->next;
    } while (current != *head);

    if (prev == NULL) {
        // Inserir no início
        CNode *tail = *head;
        while (tail->next != *head) tail = tail->next;
        newNode->next = *head;
        tail->next = newNode;
        *head = newNode;
    } else {
        prev->next = newNode;
        newNode->next = current;
    }
}

// Calcula a soma dos elementos da lista circular
double sumCircularList(CNode *head) {
    double sum = 0.0;
    if (head == NULL) return sum;

    CNode *current = head;
    do {
        sum += current->value;
        current = current->next;
    } while (current != head);

    return sum;
}

// Insere um Node na lista duplamente ligada com a lista circular associada
void insertIntoLinkedList(Node **head, int key, CNode *circleList) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Falha na alocação de memória para um novo Node.\n");
        exit(1);
    }
    newNode->key = key;
    newNode->circleList = circleList;
    newNode->next = NULL;
    newNode->prev = NULL;

    if (*head == NULL) {
        *head = newNode;
        return;
    }

    // Inserção ordenada baseada na soma das listas circulares
    Node *current = *head;
    Node *prev = NULL;
    double newSum = sumCircularList(circleList);
    while (current != NULL && sumCircularList(current->circleList) < newSum) {
        prev = current;
        current = current->next;
    }

    if (prev == NULL) {
        // Inserir no início
        newNode->next = *head;
        (*head)->prev = newNode;
        *head = newNode;
    } else {
        newNode->next = current;
        newNode->prev = prev;
        if (current != NULL) {
            current->prev = newNode;
        }
        prev->next = newNode;
    }
}

// Libera a memória da lista circular
void freeCircularList(CNode *head) {
    if (head == NULL) return;
    CNode *current = head->next;
    CNode *temp;
    while (current != head) {
        temp = current;
        current = current->next;
        free(temp);
    }
    free(head);
}

// Libera a memória da lista duplamente ligada
void freeLinkedList(Node *head) {
    while (head != NULL) {
        Node *temp = head;
        head = head->next;
        freeCircularList(temp->circleList);
        free(temp);
    }
}

// Processa o arquivo de entrada e gera o arquivo de saída
void processLines(const char *inputFilename, const char *outputFilename) {
    Node *listHead = NULL;

    FILE *inputFile = fopen(inputFilename, "r");
    if (inputFile == NULL) {
        perror("Erro ao abrir o arquivo de entrada");
        return;
    }

    FILE *outputFile = fopen(outputFilename, "w");
    if (outputFile == NULL) {
        perror("Erro ao abrir o arquivo de saída");
        fclose(inputFile);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), inputFile) != NULL) {
        char *token = strtok(line, " ");
        while (token != NULL) {
            if (strcmp(token, "LE") == 0) {
                token = strtok(NULL, " ");
                int key = atoi(token);

                double values[100];
                int numValues = 0;
                token = strtok(NULL, " ");
                while (token != NULL && strcmp(token, "LI") != 0) {
                    values[numValues++] = atof(token);
                    token = strtok(NULL, " ");
                }

                CNode *circleList = initializeCircularList();
                for (int i = 0; i < numValues; i++) {
                    insertSortedCircular(&circleList, values[i]);
                }

                insertIntoLinkedList(&listHead, key, circleList);
            } else {
                token = strtok(NULL, " ");
            }
        }
    }

    Node *current = listHead;
    while (current != NULL) {
        fprintf(outputFile, "[%d(", current->key);
        CNode *circleCurrent = current->circleList;
        if (circleCurrent != NULL) {
            do {
                fprintf(outputFile, "%.2f", circleCurrent->value);
                circleCurrent = circleCurrent->next;
                if (circleCurrent != current->circleList) fprintf(outputFile, "->");
            } while (circleCurrent != current->circleList);
        }
        fprintf(outputFile, ")]");
        current = current->next;
        if (current != NULL) fprintf(outputFile, "->");
    }
    fprintf(outputFile, "\n");

    freeLinkedList(listHead);

    fclose(inputFile);
    fclose(outputFile);
}

int main() {
    const char *inputFilename = "L1Q3.in";
    const char *outputFilename = "L1Q3.out";

    processLines(inputFilename, outputFilename);

    return 0;
}
