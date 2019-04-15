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
    void my_paintLine(int x1, int y1, int x2, int y2);

    void ReadFiles();
    void InitButtonsAndLines();
    QRect GetPos(int num);

    typedef struct ArcNode
        {
            int   adjvex;   // ending vertex of the arc
            struct ArcNode  *nextarc;//指向下一表节点
            struct ArcNode  *priou;
            int weight; // relevant weight of arc
        } ArcNode;//表节点

        typedef struct VNode
        {
            QString spot;
            QString  ticketPrice;   // 顶点信息
            struct ArcNode  *firstarc = nullptr;//指向下一个表节点
        } VNode, AdjList[100];//表头结点

        struct ALGraph
        {
            AdjList  vertices;//顶点信息
            int n;//顶点个数
            int e;//边个数
        }G;//图

        struct{
            int  adjvex;  // a vertex in U
            int  lowcost; // weight of the edge
        }   closedge[100];

private:
    Ui::GForSpots *ui;
    QPixmap Pix;
};

#endif // GFORSPOTS_H
