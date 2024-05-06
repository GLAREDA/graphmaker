#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <limits>
#include "graph.h"
using namespace std;

const int INF = numeric_limits<int>::max();

int main() {
    string filename;
    int choice;
    cout << "Каким способом вы хотите задать граф?" << endl;
    cout << "1. Матрица инцидентности" << endl;
    cout << "2. Матрица смежности" << endl;
    cout << "Ваш выбор: ";
    cin >> choice;
    cout << "Введите имя файла: ";
    cin >> filename;
    int n;
    int** matrix;
    int edges = 0;
    if (choice == 1) {
        matrix = readIncidenceMatrix(filename, n);
    } else if (choice == 2) {
        matrix = readAdjacencyMatrix(filename, n);
    } else {
        cerr << "Ошибка: Неверный выбор." << endl;
        return 1;
    }
    if (matrix == nullptr) {
        cerr << "Ошибка: Не удалось считать матрицу из файла." << endl;
        return 1;
    }
    cout << "Число вершин: " << n << endl;
    int** GraphMatrix = new int*[n];
    for (int i = 0; i < n; ++i) {
        GraphMatrix[i] = new int[n];
        for (int j = 0; j < n; ++j) {
            GraphMatrix[i][j] = 0;
        }
    }
    if (choice == 1) {
        createFromIncMatrix(GraphMatrix, n, matrix, n, edges);
    } else {
        createFromAdjMatrix(GraphMatrix, n, matrix, n, edges);
    }
    cout << "Число ребер: " << edges << endl;
    cout << "Максимальная степень вершины: " << getMaxDegree(GraphMatrix, n) << endl;
    cout << "Минимальная степень вершины: " << getMinDegree(GraphMatrix, n) << endl;
    cout << "Средняя степень вершины: " << getAverageDegree(GraphMatrix, n) << endl;
    int** distanceMatrix = new int*[n];
    for (int i = 0; i < n; ++i) {
        distanceMatrix[i] = new int[n];
    }
    findDistanceMatrix(GraphMatrix, n, distanceMatrix);
    printDistanceMatrix(distanceMatrix, n);

    int* eccentricities = new int[n];
    findEccentricities(distanceMatrix, n, eccentricities);
    cout << "Эксцентриситеты вершин:" << endl;
    for (int i = 0; i < n; ++i) {
        cout << "Вершина " << i << ": " << eccentricities[i] << endl;
    }
    int radius = findRadius(eccentricities, n);
    int diameter = findDiameter(eccentricities, n);
    cout << "Радиус графа: " << radius << endl;
    cout << "Диаметр графа: " << diameter << endl;
    cout << "Центральные вершины:" << endl;
    for (int i = 0; i < n; ++i) {
        if (eccentricities[i] == radius) cout << i << endl;
    }
    cout << "Периферийные вершины:" << endl;
    for (int i = 0; i < n; ++i) {
        if (eccentricities[i] == diameter) {
            cout << i << endl;
        }
    }
    cout << "Передаточное число графа: " << findTransmissionNumber(eccentricities, n) << endl;
    cout << "Медиана графа: " << findMedian(eccentricities, n, distanceMatrix) << endl;
    visualizeGraph(GraphMatrix, n, "graph",eccentricities, findRadius(eccentricities, n),findDiameter(eccentricities, n));
    delete[] eccentricities;
    for (int i = 0; i < n; ++i) {
        delete[] matrix[i];
        delete[] GraphMatrix[i];
        delete[] distanceMatrix[i];
    }
    delete[] matrix;
    delete[] GraphMatrix;
    delete[] distanceMatrix;
    return 0;
}
