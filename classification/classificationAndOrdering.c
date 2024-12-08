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

// Funções de comparação para ordenação
int comparePoints(const void *a, const void *b) {
    Point *p1 = (Point *)a;
    Point *p2 = (Point *)b;
    return (p1->distance < p2->distance) ? -1 : (p1->distance > p2->distance);
}

int compareStrings(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int compareInts(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int compareFloats(const void *a, const void *b) {
    double diff = (*(double *)a - *(double *)b);
    return (diff < 0) ? -1 : (diff > 0);
}

// Função para calcular a distância euclidiana de um ponto até a origem
void calculateDistances(Point points[], int numPoints) {
    for (int i = 0; i < numPoints; i++) {
        points[i].distance = sqrt(points[i].x * points[i].x + points[i].y * points[i].y);
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

        // Ordenação
        qsort(points, numPoints, sizeof(Point), comparePoints);
        qsort(strings, numStrings, sizeof(char *), compareStrings);
        qsort(ints, numInts, sizeof(int), compareInts);
        qsort(floats, numFloats, sizeof(double), compareFloats);

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