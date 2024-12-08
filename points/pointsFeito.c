#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct {
    int x;
    int y;
    double distance; // Distância Euclidiana até a origem
} Point;

double calculateDistance(const Point *p1, const Point *p2) {
    return sqrt((p2->x - p1->x) * (p2->x - p1->x) + (p2->y - p1->y) * (p2->y - p1->y));
}

int comparePoints(const Point *pa, const Point *pb) {
    return (pa->distance < pb->distance) ? -1 : (pa->distance > pb->distance) ? 1 : 0;
}

void insertionSort(Point points[], int numPoints) {
    for (int i = 1; i < numPoints; i++) {
        Point key = points[i];
        int j = i - 1;
        while (j >= 0 && comparePoints(&points[j], &key) > 0) {
            points[j + 1] = points[j];
            j--;
        }
        points[j + 1] = key;
    }
}

int readPointsFromLine(const char *line, Point points[], int maxPoints) {
    int numPoints = 0;
    char *p = (char *)line; // Usa a linha passada por referência

    // Pula a palavra "points"
    while (*p != '(') {
        p++;
    }
    p++; // Pula o parêntese de abertura

    // Continua lendo pontos até atingir o máximo ou não haver mais pontos na linha
    while (numPoints < maxPoints) {
        int x, y;
        if (sscanf(p, "%d,%d", &x, &y) == 2) {
            points[numPoints].x = x;
            points[numPoints].y = y;
            points[numPoints].distance = calculateDistance(&points[numPoints], &(Point){0, 0});
            numPoints++;

            // Avança para o próximo ponto
            p = strchr(p, '('); // Encontra o próximo parêntese de abertura
            if (p != NULL) {
                p++; // Pula o parêntese de abertura
            } else {
                // Não há mais pontos na linha
                break;
            }
        } else {
            // Falha ao ler as coordenadas, sai do loop
            break;
        }
    }

    return numPoints;
}

int main() {
    const char *inputFilename = "L0Q1.in";
    const char *outputFilename = "L0Q1.out";
    const int maxPoints = 1000; // Ajuste conforme necessário
    Point points[maxPoints];
    int numPoints;

    FILE *inputFile = fopen(inputFilename, "r");
    if (inputFile == NULL) {
        perror("Erro ao abrir o arquivo de entrada");
        return 1;
    }

    FILE *outputFile = fopen(outputFilename, "w");
    if (outputFile == NULL) {
        perror("Erro ao abrir o arquivo de saída");
        fclose(inputFile);
        return 1;
    }

    char line[256]; // Ajuste o tamanho conforme necessário

    while (fgets(line, sizeof(line), inputFile) != NULL) {
        numPoints = readPointsFromLine(line, points, maxPoints);

        // Calcular a distância de atalho entre o primeiro e o último ponto na ordem original
        double shortcutDistance = 0.0;
        if (numPoints > 1) {
            shortcutDistance = calculateDistance(&points[0], &points[numPoints - 1]);
        }

        // Calcular a distância total entre os pontos na ordem original
        double totalDistance = 0.0;
        for (int i = 0; i < numPoints - 1; i++) {
            totalDistance += calculateDistance(&points[i], &points[i + 1]);
        }

        // Ordenar os pontos baseados na distância até a origem
        insertionSort(points, numPoints);

        // Formatar a saída no formato especificado, mantendo a ordem original
        fprintf(outputFile, "points (");
        for (int i = 0; i < numPoints; i++) {
            fprintf(outputFile, "%d,%d", points[i].x, points[i].y);
            if (i < numPoints - 1) {
                fprintf(outputFile, ") (");
            }
        }
        fprintf(outputFile, ") distance %.2f shortcut %.2f\n", totalDistance, shortcutDistance);
    }

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
