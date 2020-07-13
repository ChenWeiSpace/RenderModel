#include "RenderModel.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RenderModel w;
    w.show();
	a.processEvents();
	w.getRenderWindow()->buildContent();
    return a.exec();
}
