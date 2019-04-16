#include "gforspots.h"
#include "ui_gforspots.h"
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QMessageBox>
#define IMPOSSIBLE 100000

int count;
int s[100];
int k = 7;
QString tempS = "";
int nowSpot = 0;

GForSpots::GForSpots(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GForSpots)
{
    ui->setupUi(this);

    Pix = QPixmap(width(), height());
    Pix.fill(Qt::gray);

    ReadFiles();
    InitButtons();
    InitLines();

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
        G.vertices[j].num = j;
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


void GForSpots::InitButtons(){

    for(int i=0;i<G.n;i++){
        QPushButton *but = new QPushButton(ui->centralWidget);
        but->setMaximumSize(50,50);
        but->setMinimumSize(50,50);
        but->setObjectName(QString::number(i));
        but->setText(QString::number(i));

        but->setGeometry(GetPos(i));
        connect(but,SIGNAL(clicked()),this,SLOT(infor_Check()));
    }
}

void GForSpots::InitLines(){
    for(int i=0;i<G.n;i++){
        ArcNode *temp = G.vertices[i].firstarc;
        int numTemp = G.vertices[temp->adjvex].num;
        DrawLines(i, numTemp,true);

        while (temp->nextarc != nullptr){
            temp = temp->nextarc;
            DrawLines(i,G.vertices[temp->adjvex].num,true);
        }
    }
}

void GForSpots::DrawLines(int fnode, int snode, bool mode){
    QString temp1 = QString::number(fnode);
    QString temp2 = QString::number(snode);
    QPushButton *tempBut1 = ui->centralWidget->findChild<QPushButton*>(temp1);
    QPushButton *tempBut2 = ui->centralWidget->findChild<QPushButton*>(temp2);
    int a = tempBut1->x();
    int b = tempBut1->y()+20;
    int c = tempBut2->x();
    int d = tempBut2->y()+20;
    my_paintLine(a,b,c,d,mode);
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
        return QRect(300,290,50,50);
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

void GForSpots::infor_Check(){
    ui->Information->setText("");
    QString temp = sender()->objectName();
    nowSpot = temp.toInt();
    ui->Information->setText("Spot:"+G.vertices[nowSpot].spot+"\n"
                             +G.vertices[nowSpot].ticketPrice);
    ui->Information->append("There are several spots around this one:");
    ArcNode* p = G.vertices[nowSpot].firstarc;

    ui->Information->append(G.vertices[nowSpot].spot+"->"
                            +G.vertices[p->adjvex].spot+"   "
                            +QString::number(p->weight)+"m");

    while (p->nextarc != nullptr){
        p = p->nextarc;
        ui->Information->append(G.vertices[nowSpot].spot+"->"
                                +G.vertices[p->adjvex].spot+"   "
                                +QString::number(p->weight)+"m");

    }
}

void GForSpots::on_Find_Roads_clicked()
{
    ui->Roads->setText("");
    QString temp = ui->Start->toPlainText();
    int start = temp.toInt();

    bool visited[100];
        int v;
        for (v = 0; v<G.n; ++v)
            visited[v] = false; // set flag for being visited
        int a[100];
        for (v = 0; v < G.n; ++v){
            a[v] = -1;
        }
        count = 0;
        DFS(G, start, visited, a);
}

void GForSpots::DFS(ALGraph G, int v, bool* visited, int* a){
    bool x[100];
    int b[100];
    int y;
    for (y = 0; y < 100; y++){
        x[y] = visited[y];
        b[y] = a[y];
    }

    x[v] = true;
    int i;
    for (i = 0; i < G.n; ++i){
        if (b[i] == -1){
            break;
        }
    }
    if (i == G.n-1){//走到头了
        a[i] = v;
        tempS = ui->Roads->toPlainText() + "Path";
        tempS += QString::number(++count);
        tempS += ":";
        ui->Roads->setText(tempS);

        int j;
        for (j = 0; j < G.n-1; j++){
            tempS = ui->Roads->toPlainText();
            tempS += G.vertices[a[j]].spot;
            tempS += "->";
            ui->Roads->setText(tempS);
        }
        tempS = ui->Roads->toPlainText();
        tempS += G.vertices[a[j]].spot;
        tempS += "\n";
        ui->Roads->setText(tempS);
        return;
    }

    else{
        b[i] = v;
    }
    struct ArcNode* w;
    w = G.vertices[v].firstarc;

    if (visited[w->adjvex] == false){
        DFS(G, w->adjvex, x,b);
    }
    w = w->nextarc;

    while (w != nullptr){
        if (visited[w->adjvex] == false){
            DFS(G, w->adjvex,x,b);
        }
        w = w->nextarc;
    }
}

void GForSpots::on_Find_Short_clicked()
{
    int from = ui->ShortS->toPlainText().toInt();
    int to = ui->ShortE->toPlainText().toInt();

    int path[100];
    int dist[100];
    Dijkstra(G, from, path, dist);
    int y = to;
    int i=0;
    int re[10];
    re[i] = y;
    i++;
    InitLines();

    DrawLines(y, path[y], false);
    this->repaint();
    while (path[y] != from){
        y = path[y];
        DrawLines(y, path[y], false);
        this->repaint();
        re[i] =y;
        i++;
    }

}

void GForSpots::Dijkstra(ALGraph G, int v0, int path[], int dist[]){
    int v;
    for(v = 0; v<G.n; ++v)
    {
        s[v] = 0;
        dist[v] = IMPOSSIBLE;
        path[v] = -1;
    }
    ArcNode* p;
    p = G.vertices[v0].firstarc;
    while (p != nullptr){
        dist[p->adjvex] = p->weight;
        path[p->adjvex] = v0;
        p = p->nextarc;
    }
    dist[v0] = 0;  s[v0] = 1;//S集中开始时只有v0
    int i;

    for(i = 1; i<G.n; ++i){
        int min;
        min = IMPOSSIBLE;
        int w;
        for (w = 0; w < G.n; ++w){
            if (s[w] == 0){
                if (dist[w]<min) {
                    v = w;
                    min = dist[w];
                }
            }
        }
        s[v] = 1;//将v加入S

        p = G.vertices[v].firstarc;
        int d;
        int q[100];
        for (d = 0; d < G.n; d++){
            q[d] = IMPOSSIBLE;
        }
        while (p != nullptr){
            q[p->adjvex] = p->weight;
            p = p->nextarc;
        }
        for (w = 0; w < G.n; ++w){//调整其余在V-S的点

            if (s[w] == 0 && (dist[v] + q[w]<dist[w]))
            {
                dist[w] = dist[v] + q[w];
                path[w] = v;
            }
        }


    }
}

void GForSpots::on_Find_Tree_clicked()
{
    //TODO: Find the minimum spanning tree.
}












