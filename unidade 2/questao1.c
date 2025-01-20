#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função para comparar dois inteiros (usada em qsort)
int compare(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
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
    int lastSum = INT_MIN; // Inicializa a última soma válida com um valor mínimo

    while (fgets(line, sizeof(line), inputFile) != NULL) {
        List lists[100];
        int listCount = 0;

        char *token = strtok(line, " ");
        while (token != NULL) {
            if (strcmp(token, "start") == 0) {
                lists[listCount].values = (int *)malloc(100 * sizeof(int));
                lists[listCount].size = 0;

                token = strtok(NULL, " ");
                while (token != NULL && strcmp(token, "start") != 0) {
                    int value = atoi(token);
                    lists[listCount].values[lists[listCount].size++] = value;
                    token = strtok(NULL, " ");
                }

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
        int first = 1; // Flag para adicionar espaço entre as listas, mas não no início
        for (int i = 0; i < listCount; i++) {
            if (lastSum != lists[i].sum || first) { // Verifica se a soma é única
                if (!first) fprintf(outputFile, " "); // Adiciona espaço se não for a primeira lista
                fprintf(outputFile, "start");
                for (int j = 0; j < lists[i].size; j++) {
                    fprintf(outputFile, " %d", lists[i].values[j]);
                }
                lastSum = lists[i].sum; // Atualiza a última soma válida
                first = 0; // Após escrever a primeira lista, não queremos mais adicionar espaço inicial
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
