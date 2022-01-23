#include "QtWidgetForMarkingImage.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtWidgetForMarkingImage w;
    w.show();
    return a.exec();
}
