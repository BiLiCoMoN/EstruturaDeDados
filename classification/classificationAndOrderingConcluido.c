#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Estrutura para representar um ponto no espaço bidimensional
typedef struct {
    double x;
    double y;
    double distance; // Distância Euclidiana até a origem
} Point;

// Função para calcular a distância euclidiana de um ponto até a origem
void calculateDistances(Point points[], int numPoints) {
    for (int i = 0; i < numPoints; i++) {
        points[i].distance = sqrt(points[i].x * points[i].x + points[i].y * points[i].y);
    }
}

// Implementação de Insertion Sort para pontos
void insertionSortPoints(Point points[], int numPoints) {
    for (int i = 1; i < numPoints; i++) {
        Point key = points[i];
        int j = i - 1;
        while (j >= 0 && points[j].distance > key.distance) {
            points[j + 1] = points[j];
            j--;
        }
        points[j + 1] = key;
    }
}

// Implementação de Insertion Sort para strings
void insertionSortStrings(char *strings[], int numStrings) {
    for (int i = 1; i < numStrings; i++) {
        char *key = strings[i];
        int j = i - 1;
        while (j >= 0 && strcmp(strings[j], key) > 0) {
            strings[j + 1] = strings[j];
            j--;
        }
        strings[j + 1] = key;
    }
}

// Implementação de Insertion Sort para inteiros
void insertionSortInts(int ints[], int numInts) {
    for (int i = 1; i < numInts; i++) {
        int key = ints[i];
        int j = i - 1;
        while (j >= 0 && ints[j] > key) {
            ints[j + 1] = ints[j];
            j--;
        }
        ints[j + 1] = key;
    }
}

// Implementação de Insertion Sort para floats
void insertionSortFloats(double floats[], int numFloats) {
    for (int i = 1; i < numFloats; i++) {
        double key = floats[i];
        int j = i - 1;
        while (j >= 0 && floats[j] > key) {
            floats[j + 1] = floats[j];
            j--;
        }
        floats[j + 1] = key;
    }
}

// Função para formatar números com até 2 casas decimais somente quando necessário
void formatNumber(char *buffer, double value) {
    if (fabs(value - (int)value) < 1e-9) {
        sprintf(buffer, "%d", (int)value);
    } else if (fabs(value * 10 - (int)(value * 10)) < 1e-9) {
        sprintf(buffer, "%.1f", value);
    } else {
        sprintf(buffer, "%.2f", value);
    }
}

// Função principal para processar o arquivo
void processLines(const char *inputFilename, const char *outputFilename) {
    const int maxPoints = 1000;
    const int maxStrings = 1000;
    const int maxInts = 1000;
    const int maxFloats = 1000;

    Point points[maxPoints];
    char *strings[maxStrings];
    int ints[maxInts];
    double floats[maxFloats];

    for (int i = 0; i < maxStrings; i++) {
        strings[i] = (char *)malloc(sizeof(char) * 256);
    }

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

    int numPoints = 0, numStrings = 0, numInts = 0, numFloats = 0;

    while (fgets(line, sizeof(line), inputFile) != NULL) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        numPoints = 0;
        numStrings = 0;
        numInts = 0;
        numFloats = 0;

        // Parsing da linha
        char *saveptr;
        char *token = strtok_r(line, " ", &saveptr);
        while (token != NULL) {
            if (token[0] == '(' && strchr(token, ',')) {
                sscanf(token, "(%lf,%lf)", &points[numPoints].x, &points[numPoints].y);
                numPoints++;
            } else if (strchr("0123456789-.", token[0])) {
                if (strchr(token, '.')) {
                    floats[numFloats++] = atof(token);
                } else {
                    ints[numInts++] = atoi(token);
                }
            } else {
                strcpy(strings[numStrings++], token);
            }
            token = strtok_r(NULL, " ", &saveptr);
        }

        // Calcular distâncias dos pontos
        calculateDistances(points, numPoints);

        // Ordenação usando Insertion Sort
        insertionSortPoints(points, numPoints);
        insertionSortStrings(strings, numStrings);
        insertionSortInts(ints, numInts);
        insertionSortFloats(floats, numFloats);

        // Escrevendo a saída
        fprintf(outputFile, "str:");
        for (int i = 0; i < numStrings; i++) {
            fprintf(outputFile, "%s%s", (i > 0 ? " " : ""), strings[i]);
        }
        fprintf(outputFile, " int:");
        for (int i = 0; i < numInts; i++) {
            fprintf(outputFile, "%s%d", (i > 0 ? " " : ""), ints[i]);
        }
        fprintf(outputFile, " float:");
        for (int i = 0; i < numFloats; i++) {
            char buffer[32];
            formatNumber(buffer, floats[i]);
            fprintf(outputFile, "%s%s", (i > 0 ? " " : ""), buffer);
        }
        fprintf(outputFile, " p:");
        for (int i = 0; i < numPoints; i++) {
            char bufferX[32], bufferY[32];
            formatNumber(bufferX, points[i].x);
            formatNumber(bufferY, points[i].y);
            fprintf(outputFile, "%s(%s,%s)", (i > 0 ? " " : ""), bufferX, bufferY);
        }
        fprintf(outputFile, "\n");
    }

    // Liberar memória alocada para as strings
    for (int i = 0; i < maxStrings; i++) {
        free(strings[i]);
    }

    fclose(inputFile);
    fclose(outputFile);
}

int main() {
    const char *inputFilename = "L0Q2.in";
    const char *outputFilename = "L0Q2.out";

    processLines(inputFilename, outputFilename);

    return 0;
}
