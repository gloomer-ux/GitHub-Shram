#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QFile>
#include <QTextStream>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QtMath>

// Функция для загрузки графа из файла
void loadGraph(const QString& fileName, QVector<QPair<int, int>>& edges, int& vertexCount) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Не удалось открыть файл");
        return;
    }
    QTextStream in(&file);
    vertexCount = in.readLine().toInt(); // Читаем количество вершин

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(" ");
        if (parts.size() == 2) {
            int start = parts[0].toInt();
            int end = parts[1].toInt();
            edges.append(qMakePair(start, end)); // Добавляем ребро
        }
    }
    file.close();
}

// Основная функция для отрисовки графа
void drawGraph(QGraphicsScene* scene, const QVector<QPair<int, int>>& edges, int vertexCount) {
    const int radius = 200; // Радиус окружности для расположения вершин
    const int vertexRadius = 15; // Радиус вершины (круга)
    QVector<QPointF> positions(vertexCount);

    // Расчет положения вершин по окружности
    for (int i = 0; i < vertexCount; ++i) {
        double angle = (2 * M_PI / vertexCount) * i;
        double x = radius * qCos(angle);
        double y = radius * qSin(angle);
        positions[i] = QPointF(x, y);

        // Отрисовка вершины
        QGraphicsEllipseItem * vertex = scene->addEllipse(x - vertexRadius, y - vertexRadius,
                                                         vertexRadius * 2, vertexRadius * 2,
                                                         QPen(Qt::black), QBrush(Qt::yellow));
        // Добавление текста с номером вершины
        QGraphicsTextItem* text = scene->addText(QString::number(i + 1));
        text->setPos(x - 5, y - 5); // Небольшой сдвиг для центрирования текста
    }

    // Отрисовка ребер
    for (const auto& edge : edges) {
        int start = edge.first - 1;
        int end = edge.second - 1;
        if (start >= 0 && start < vertexCount && end >= 0 && end < vertexCount) {
            scene->addLine(QLineF(positions[start], positions[end]), QPen(Qt::black));
        }
    }
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Создаем графическую сцену
    QGraphicsScene scene;
    scene.setSceneRect(-250, -250, 500, 500); // Устанавливаем размеры сцены

    // Читаем граф из файла
    QVector<QPair<int, int>> edges;
    int vertexCount = 0;
    loadGraph("C:/Users/Sibyl/Documents/untitled2/graph.txt", edges, vertexCount);

    // Рисуем граф
    drawGraph(&scene, edges, vertexCount);

    // Создаем виджет для отображения
    QGraphicsView view(&scene);
    view.setWindowTitle("Граф");
    view.resize(600, 600);
    view.show();

    return app.exec();
}
