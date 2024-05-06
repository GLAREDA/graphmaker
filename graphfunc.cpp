#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <limits>

using namespace std;

const int INF = numeric_limits<int>::max();

void findDistanceMatrix(int** Matrix, int vertices, int** distanceMatrix) {
    for (int i = 0; i < vertices; ++i) {
        for (int j = 0; j < vertices; ++j) {
            if (i == j) {
                distanceMatrix[i][j] = 0;
            } else if (Matrix[i][j] == 1) {
                distanceMatrix[i][j] = 1;
            } else {
                distanceMatrix[i][j] = INF; 
            }
        }
    }

    for (int k = 0; k < vertices; ++k) {
        for (int i = 0; i < vertices; ++i) {
            for (int j = 0; j < vertices; ++j) {
                if (distanceMatrix[i][k] != INF && distanceMatrix[k][j] != INF &&
                    distanceMatrix[i][k] + distanceMatrix[k][j] < distanceMatrix[i][j]) {
                    distanceMatrix[i][j] = distanceMatrix[i][k] + distanceMatrix[k][j];
                }
            }
        }
    }
}

void findEccentricities(int** distanceMatrix, int vertices, int* eccentricities) {
    for (int i = 0; i < vertices; ++i) {
        int maxDistance = 0;
        for (int j = 0; j < vertices; ++j) {
            if (distanceMatrix[i][j] > maxDistance && distanceMatrix[i][j] != INF) {
                maxDistance = distanceMatrix[i][j];
            }
        }
        eccentricities[i] = maxDistance;
    }
}

int getDegree(int** Matrix, int vertices, int vertex) {
    if (vertex >= 0 && vertex < vertices) {
        int degree = 0;
        for (int i = 0; i < vertices; ++i) {
            if (Matrix[vertex][i] == 1) {
                degree++;
            }
        }
        return degree;
    } else {
        cout << "Неверный индекс вершины." << endl;
        return -1;
    }
}

int getMaxDegree(int** Matrix, int vertices) {
    int maxDegree = 0;
    for (int i = 0; i < vertices; ++i) {
        int degree = getDegree(Matrix, vertices, i);
        if (degree > maxDegree) {
            maxDegree = degree;
        }
    }
    return maxDegree;
}

int getMinDegree(int** Matrix, int vertices) {
    int minDegree = getDegree(Matrix, vertices, 0);
    for (int i = 1; i < vertices; ++i) {
        int degree = getDegree(Matrix, vertices, i);
        if (degree < minDegree) {
            minDegree = degree;
        }
    }
    return minDegree;
}

double getAverageDegree(int** Matrix, int vertices) {
    double totalDegree = 0;
    for (int i = 0; i < vertices; ++i) {
        totalDegree += getDegree(Matrix, vertices, i);
    }
    return totalDegree / vertices;
}

void addEdge(int** Matrix, int vertices, int& edges, int u, int v) {
    if (u >= 0 && u < vertices && v >= 0 && v < vertices) {
        Matrix[u][v] = 1;
        Matrix[v][u] = 1;
        edges++;
    } else {
        cout << "Неверные индексы вершин." << endl;
    }
}

void createFromIncMatrix(int** Matrix, int vertices, int** incidenceMatrix, int n, int& edges) {
    for (int j = 0; j < n; ++j) {
        int startVertex = -1, endVertex = -1;
        for (int i = 0; i < vertices; ++i) {
            if (incidenceMatrix[i][j] == 1) {
                if (startVertex == -1)
                    startVertex = i;
                else
                    endVertex = i;
            }
        }
        if (startVertex != -1 && endVertex != -1 && Matrix[startVertex][endVertex] != 1) {
            addEdge(Matrix, vertices, edges, startVertex, endVertex);
        }
    }
}

void createFromAdjMatrix(int** Matrix, int vertices, int** adjacencyMatrix, int n, int& edges) {
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (adjacencyMatrix[i][j] == 1 && Matrix[i][j] != 1) {
                addEdge(Matrix, vertices, edges, i, j);
            }
        }
    }
}

void printDistanceMatrix(int** distanceMatrix, int vertices) {
    cout << "Матрица расстояний:" << endl;
    for (int i = 0; i < vertices; ++i) {
        for (int j = 0; j < vertices; ++j) {
            if (distanceMatrix[i][j] == INF) {
                cout << "INF ";
            } else {
                cout << distanceMatrix[i][j] << " ";
            }
        }
        cout << endl;
    }
}

void visualizeGraph(int** Matrix, int vertices, const string& filename, int* eccentricities, int radius, int diameter) {
    if (filename.empty()) {
        cerr << "Error: Empty filename." << endl;
        return;
    }

    ofstream dotFile(filename + ".dot"); // Создание DOT файла
    if (!dotFile.is_open()) {
        cerr << "Error: Unable to create dot file " << filename << endl;
        return;
    }

    // Запись заголовка DOT файла
    dotFile << "graph G {" << endl;
    dotFile << "splines=curved;" << endl;
    dotFile << "  layout=dot;" << endl; // Использование алгоритма Dot
    dotFile<<"rankdir=TB"<<endl;

    // Запись узлов с указанием цвета в зависимости от эксцентриситета
    for (int i = 0; i < vertices; ++i) {
        if (eccentricities[i] == radius) {
            dotFile << "  " << i << " [color=blue];" << endl; // Центральные вершины
        } else if (eccentricities[i] == diameter) {
            dotFile << "  " << i << " [color=red];" << endl; // Периферийные вершины
        } else {
            dotFile << "  " << i << ";" << endl; // Обычные вершины
        }
    }

    // Запись рёбер графа
    for (int i = 0; i < vertices; ++i) {
        for (int j = i + 1; j < vertices; ++j) {
            if (Matrix[i][j] == 1) {
                dotFile << "  " << i << " -- " << j << ";" << endl;
            }
        }
    }

    dotFile << "}" << endl;

    dotFile.close(); // Закрытие файла

    // Запуск Graphviz для создания изображения
    string command = "dot -Tpng " + filename + ".dot -o " + filename + ".png";
    system(command.c_str());
}



int** readIncidenceMatrix(const string& filename, int& n) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return nullptr;
    }

    file >> n;

    int** incidenceMatrix = new int*[n];
    for (int i = 0; i < n; ++i) {
        incidenceMatrix[i] = new int[n];
        for (int j = 0; j < n; ++j) {
            file >> incidenceMatrix[i][j];
        }
    }

    file.close(); // Close the file after reading

    return incidenceMatrix;
}

int** readAdjacencyMatrix(const string& filename, int& n) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return nullptr;
    }

    file >> n;

    int** adjacencyMatrix = new int*[n];
    for (int i = 0; i < n; ++i) {
        adjacencyMatrix[i] = new int[n];
        for (int j = 0; j < n; ++j) {
            file >> adjacencyMatrix[i][j];
        }
    }

    file.close(); // Close the file after reading

    return adjacencyMatrix;
}

int findRadius(int* eccentricities, int vertices) {
    int minEccentricity = eccentricities[0];
    for (int i = 1; i < vertices; ++i) {
        if (eccentricities[i] < minEccentricity) {
            minEccentricity = eccentricities[i];
        }
    }
    return minEccentricity;
}

int findDiameter(int* eccentricities, int vertices) {
    int maxEccentricity = eccentricities[0];
    for (int i = 1; i < vertices; ++i) {
        if (eccentricities[i] > maxEccentricity) {
            maxEccentricity = eccentricities[i];
        }
    }
    return maxEccentricity;
}

int findMedian(int* eccentricities, int vertices, int** distanceMatrix) {
    int minEccentricitiesSum = INF;
    int median = -1;
    for (int i = 0; i < vertices; ++i) {
        int sum = 0;
        for (int j = 0; j < vertices; ++j) {
            sum += distanceMatrix[i][j];
        }
        if (sum < minEccentricitiesSum) {
            minEccentricitiesSum = sum;
            median = i;
        }
    }
    return median;
}

int findTransmissionNumber(int* eccentricities, int vertices) {
    int radius = findRadius(eccentricities, vertices);
    int diameter = findDiameter(eccentricities, vertices);
    return diameter - radius;
}
