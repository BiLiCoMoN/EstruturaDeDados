#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função para comparar dois inteiros (usada em qsort)
int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// Função para calcular a soma de um array
int calculateSum(int *array, int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += array[i];
    }
    return sum;
}

// Estrutura para armazenar uma lista e sua soma
typedef struct {
    int *values;
    int size;
    int sum;
} List;

// Função para comparar duas listas com base na soma (usada em qsort)
int compareLists(const void *a, const void *b) {
    List *listA = (List *)a;
    List *listB = (List *)b;
    return listA->sum - listB->sum;
}

// Função para processar o arquivo de entrada
void processFile(const char *inputFilename, const char *outputFilename) {
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

    char line[1024];
    while (fgets(line, sizeof(line), inputFile) != NULL) {
        // Vetor para armazenar as listas
        List lists[100];
        int listCount = 0;

        // Tokenizar a linha
        char *token = strtok(line, " ");
        while (token != NULL) {
            if (strcmp(token, "start") == 0) {
                // Nova lista
                lists[listCount].values = (int *)malloc(100 * sizeof(int));
                lists[listCount].size = 0;

                // Ler os números da lista
                token = strtok(NULL, " ");
                while (token != NULL && strcmp(token, "start") != 0) {
                    int value = atoi(token);
                    lists[listCount].values[lists[listCount].size++] = value;
                    token = strtok(NULL, " ");
                }

                // Ordenar a lista e calcular a soma
                qsort(lists[listCount].values, lists[listCount].size, sizeof(int), compare);
                lists[listCount].sum = calculateSum(lists[listCount].values, lists[listCount].size);

                listCount++;
            } else {
                token = strtok(NULL, " ");
            }
        }

        // Ordenar as listas com base na soma
        qsort(lists, listCount, sizeof(List), compareLists);

        // Escrever as listas ordenadas na linha correspondente do arquivo de saída
        for (int i = 0; i < listCount; i++) {
            if (i > 0) fprintf(outputFile, " "); // Adiciona um espaço apenas entre as listas
            fprintf(outputFile, "start");
            for (int j = 0; j < lists[i].size; j++) {
                fprintf(outputFile, " %d", lists[i].values[j]);
            }
            free(lists[i].values); // Liberar a memória alocada para a lista
        }
        fprintf(outputFile, "\n"); // Quebra de linha ao final da linha de entrada
    }

    fclose(inputFile);
    fclose(outputFile);
}

int main() {
    const char *inputFilename = "L1Q1.in";
    const char *outputFilename = "L1Q1.out";

    processFile(inputFilename, outputFilename);

    return 0;
}
