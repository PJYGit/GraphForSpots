#ifndef GFORSPOTS_H
#define GFORSPOTS_H

#include <QMainWindow>
#include <QColor>
#include <QPixmap>
#include <QPainter>

namespace Ui {
class GForSpots;
}

class GForSpots : public QMainWindow
{
    Q_OBJECT

public:
    explicit GForSpots(QWidget *parent = nullptr);
    ~GForSpots();

    void paintEvent(QPaintEvent *event);
    void my_paintLine(int x1, int y1, int x2, int y2, bool color);

    void ReadFiles();
    void InitButtons();
    void InitLines();
    void DrawLines(int fnode, int snode, bool mode);
    QRect GetPos(int num);

    typedef struct ArcNode
        {
            int   adjvex;
            struct ArcNode  *nextarc;
            int weight;
        } ArcNode;

        typedef struct VNode
        {
            int num;
            QString spot;
            QString  ticketPrice;
            struct ArcNode  *firstarc = nullptr;
        } VNode, AdjList[100];

        struct ALGraph
        {
            AdjList  vertices;
            int n;
            int e;
        }G;

        struct{
            int  adjvex;
            int  lowcost;
        }   clo_edge[100];

        void DFS(ALGraph G, int first, bool* visited, int* origin);
        void Dijkstra(ALGraph G, int v0, int path[], int dist[]);
        int Prim(ALGraph G, int u);

private slots:
        void on_Find_Roads_clicked();

        void on_Find_Tree_clicked();

        void on_Find_Short_clicked();

        void infor_Check();


private:
    Ui::GForSpots *ui;
    QPixmap Pix;
};

#endif // GFORSPOTS_H
