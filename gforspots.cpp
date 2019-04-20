#include "gforspots.h"
#include "ui_gforspots.h"
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QMessageBox>
#define IMPOSSIBLE 100000

int count;
int sign[100];
int k_prim = 7;
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
    int k;
    int m;
    int length;
    while (fscanf(f, "%d\t%d\t%d",&k,&m,&length) != EOF){
        ArcNode* newNode;
        newNode = (ArcNode*)malloc(sizeof(ArcNode));
        newNode->adjvex = m;
        newNode->weight = length;
        newNode->nextarc = NULL;
        ArcNode* p = G.vertices[k].firstarc;
        if(p == NULL){//表头结点为空
            G.vertices[k].firstarc = (ArcNode*)malloc(sizeof(ArcNode));//给表头结点的firstarc分配内存
            G.vertices[k].firstarc = newNode;
        }
        else{
            while (p->nextarc != NULL){
                p = p->nextarc;
            }
            p->nextarc = newNode;
        }

        //由于是无向图，要分别对两个顶点建立联系
        newNode = (ArcNode*)malloc(sizeof(ArcNode));
        newNode->adjvex = k;
        newNode->weight = length;
        newNode->nextarc = NULL;
        p = G.vertices[m].firstarc;
        if (p == NULL){//表头结点为空
            G.vertices[m].firstarc = (ArcNode*)malloc(sizeof(ArcNode));
            G.vertices[m].firstarc = newNode;
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
        return QRect(180,175,50,50);
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
    InitLines();
    this->repaint();
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
    InitLines();
    this->repaint();
    QString temp = ui->Start->toPlainText();
    int start = temp.toInt();

    bool visited[100];
    int v;
    for (v = 0; v<G.n; ++v)
        visited[v] = false; // set flag for being visited
    int origin[100];
    for (v = 0; v < G.n; ++v){
        origin[v] = -1;
    }
    count = 0;
    DFS(G, start, visited, origin);
}

void GForSpots::DFS(ALGraph G, int first, bool* visited, int* origin){
    bool temp_v[100];
    int temp_o[100];
    int k;
    for (k = 0; k < 100; k++){
        temp_v[k] = visited[k];
        temp_o[k] = origin[k];
    }

    temp_v[first] = true;
    int i;
    for (i = 0; i < G.n; ++i){
        if (temp_o[i] == -1){
            break;
        }
    }
    if (i == G.n-1){
        origin[i] = first;
        tempS = ui->Roads->toPlainText() + "Path";
        tempS += QString::number(++count);
        tempS += ":";
        ui->Roads->setText(tempS);

        int j;
        for (j = 0; j < G.n-1; j++){
            tempS = ui->Roads->toPlainText();
            tempS += G.vertices[origin[j]].spot;
            tempS += "->";
            ui->Roads->setText(tempS);
        }
        tempS = ui->Roads->toPlainText();
        tempS += G.vertices[origin[j]].spot;
        tempS += "\n";
        ui->Roads->setText(tempS);
        return;
    }

    else{
        temp_o[i] = first;
    }
    struct ArcNode* tempN;
    tempN = G.vertices[first].firstarc;

    if (visited[tempN->adjvex] == false){
        DFS(G, tempN->adjvex, temp_v,temp_o);
    }
    tempN = tempN->nextarc;

    while (tempN != nullptr){
        if (visited[tempN->adjvex] == false){
            DFS(G, tempN->adjvex,temp_v,temp_o);
        }
        tempN = tempN->nextarc;
    }
}

void GForSpots::on_Find_Short_clicked()
{    
    int from = ui->ShortS->toPlainText().toInt();
    int to = ui->ShortE->toPlainText().toInt();

    int path[100];
    int dist[100];
    Dijkstra(G, from, path, dist);
    int i = 1;
    InitLines();
    this->repaint();

    DrawLines(to, path[to], false);
    this->repaint();
    while (path[to] != from){
        to = path[to];
        DrawLines(to, path[to], false);
        this->repaint();
        i++;
    }

}

void GForSpots::Dijkstra(ALGraph G, int v0, int path[], int dist[]){
    int v;
    for(v = 0; v<G.n; ++v)
    {
        sign[v] = 0;
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
    dist[v0] = 0;  sign[v0] = 1;//S集中开始时只有v0
    int i;

    for(i = 1; i<G.n; ++i){
        int min;
        min = IMPOSSIBLE;
        int j;
        for (j = 0; j < G.n; ++j){
            if (sign[j] == 0){
                if (dist[j]<min) {
                    v = j;
                    min = dist[j];
                }
            }
        }
        sign[v] = 1;//将v加入S

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
        for (j = 0; j < G.n; ++j){//调整其余在V-S的点

            if (sign[j] == 0 && (dist[v] + q[j]<dist[j]))
            {
                dist[j] = dist[v] + q[j];
                path[j] = v;
            }
        }


    }
}

void GForSpots::on_Find_Tree_clicked()
{
    InitLines();
    this->repaint();
    ui->Lenth->setText("");
    int u = (k_prim++)%G.n;
    int total_length = Prim(G, u);
    int i;

    for (i = 0; i < G.n; i++){

        if (i!=u){
            DrawLines(i, clo_edge[i].adjvex,false);
            this->repaint();
        }

    }

    ui->Lenth->setText(QString::number(total_length) + "m");
}

int GForSpots::Prim(ALGraph G, int u){

    int total_length = 0;
    int j;
    for (j = 0; j < G.n; ++j){
            clo_edge[j].lowcost =100000;
    }

    ArcNode* p = G.vertices[u].firstarc;
    while (p != nullptr){
        clo_edge[p->adjvex].lowcost = p->weight;
        clo_edge[p->adjvex].adjvex = u;
        p = p->nextarc;
    }
    clo_edge[u].lowcost = 0;      // U＝{u}
    int i;
    for (i = 0; i < G.n-1; ++i){
        int minimum = 100000;
        int min = 0;
        int k;
        for (k = 0; k < G.n; k++){
            if (clo_edge[k].lowcost < minimum&&clo_edge[k].lowcost != 0){
                minimum = clo_edge[k].lowcost;
                min = k;
            }
        }
        total_length += minimum;
        clo_edge[min].lowcost = 0;    // add vertex k into U


        ArcNode* p = G.vertices[min].firstarc;
        while (p != nullptr){

            if (p->weight< clo_edge[p->adjvex].lowcost
                && clo_edge[p->adjvex].lowcost>0){
                clo_edge[p->adjvex].lowcost = p->weight;
                clo_edge[p->adjvex].adjvex = min;
            }

            p = p->nextarc;
        }
    }


    return total_length;
}












