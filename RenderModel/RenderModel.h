#ifndef RenderModel_h__
#define RenderModel_h__

#include <QtWidgets/QMainWindow>
#include "ui_RenderModel.h"

class RenderModel : public QMainWindow
{
    Q_OBJECT

public:
    RenderModel(QWidget *parent = Q_NULLPTR);
	inline RenderWidget * getRenderWindow() { return ui.centralWidget; }
private:
    Ui::RenderModelClass ui;
};
#endif // RenderModel_h__
