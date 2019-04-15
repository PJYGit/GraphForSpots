#include "gforspots.h"
#include "ui_gforspots.h"
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QMessageBox>


GForSpots::GForSpots(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GForSpots)
{
    ui->setupUi(this);

    Pix = QPixmap(width(), height());
    Pix.fill(Qt::gray);

    ReadFiles();
    InitButtonsAndLines();

    /*
    int u = 0;
    QString t = QString::number(u);
    QPushButton *aux = ui->centralWidget->findChild<QPushButton*>(t);

    int f = aux->x();

    int k = aux->y();
    QString temp = QString::number(f);
    ui->centralWidget->findChild<QPushButton*>(t)->setText(temp);


    my_paintLine(f, k, 30, 30);
    this->update();
    this->repaint();
    */
}

GForSpots::~GForSpots()
{
    delete ui;
}

void GForSpots::paintEvent(QPaintEvent *event) {
    QPainter Painter(this);
    Painter.drawPixmap(0, 0, width(), height(), Pix);
}

void GForSpots::my_paintLine(int x1, int y1, int x2, int y2, bool color){

    QPainter Painter(&Pix);
    if(color){
        Painter.setPen(QPen(Qt::black, 2));
    }
    else {
        Painter.setPen(QPen(Qt::red, 2));
    }

    Painter.drawLine(x1, y1, x2, y2);
}

void GForSpots::ReadFiles(){
    FILE *f = fopen("F:/Qt codes/GForSpots/Vex.txt", "r");
        fscanf(f, "%d", &G.n);
        int i;
        for (i = 0; i < G.n; i++){
            int j;
            fscanf(f, "%d", &j);

            char Temp[100];
            fscanf(f, "%s", Temp);
            G.vertices[j].spot = Temp;
            fscanf(f, "%s", Temp);
            G.vertices[j].ticketPrice = Temp;
        }
        fclose(f);

        //读入边信息
        f = fopen("F:/Qt codes/GForSpots/Edge.txt", "r");
        int a;
        int b;
        int length;
        while (fscanf(f, "%d\t%d\t%d",&a,&b,&length) != EOF){
            ArcNode* newNode;
            newNode = (ArcNode*)malloc(sizeof(ArcNode));
            newNode->adjvex = b;
            newNode->weight = length;
            newNode->nextarc = NULL;
            ArcNode* p = G.vertices[a].firstarc;
            if(p == NULL){//表头结点为空
                G.vertices[a].firstarc = (ArcNode*)malloc(sizeof(ArcNode));//给表头结点的firstarc分配内存
                G.vertices[a].firstarc = newNode;
            }
            else{
                while (p->nextarc != NULL){
                    p = p->nextarc;
                }
                p->nextarc = newNode;
            }

            //由于是无向图，要分别对两个顶点建立联系
            newNode = (ArcNode*)malloc(sizeof(ArcNode));
            newNode->adjvex = a;
            newNode->weight = length;
            newNode->nextarc = NULL;
            p = G.vertices[b].firstarc;
            if (p == NULL){//表头结点为空
                G.vertices[b].firstarc = (ArcNode*)malloc(sizeof(ArcNode));
                G.vertices[b].firstarc = newNode;
            }
            else{
                while (p->nextarc != NULL){
                    p = p->nextarc;
                }
                p->nextarc = newNode;
            }
        }
        fclose(f);
}


void GForSpots::InitButtonsAndLines(){

    for(int i=0;i<G.n;i++){
        QPushButton *but = new QPushButton(ui->centralWidget);
        but->setMaximumSize(50,50);
        but->setMinimumSize(50,50);
        but->setObjectName(QString::number(i));
        but->setText(QString::number(i));

        but->setGeometry(GetPos(i));
    }
}

QRect GForSpots::GetPos(int num){
    if (num == 0){
        return QRect(140,25,50,50);
    }
    else if (num == 1) {
        return QRect(60,215,50,50);
    }
    else if (num == 2) {
        return QRect(460,25,50,50);
    }
    else if (num == 3) {
        return QRect(550,215,50,50);
    }
    else if (num == 4) {
        return QRect(140,385,50,50);
    }
    else if (num == 5) {
        return QRect(460,385,50,50);
    }
    else if (num == 6) {
        return QRect(300,315,50,50);
    }
    else if (num == 7) {
        return QRect(180,160,50,50);
    }
    else if (num == 8) {
        return QRect(420,160,50,50);
    }
    else {
        QMessageBox::warning(this, "Warning!", "Capacity Explosion1");
        exit(0);
        return QRect(0,0,0,0);
    }
}












