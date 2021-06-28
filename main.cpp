#include "travelplan.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TravelPlan w;
    w.show();
    return a.exec();
}
