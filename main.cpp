#include "gforspots.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GForSpots w;
    w.show();

    return a.exec();
}
