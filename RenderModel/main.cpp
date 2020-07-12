#include "RenderModel.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RenderModel w;
    w.show();
    return a.exec();
}
