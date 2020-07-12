#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_RenderModel.h"

class RenderModel : public QMainWindow
{
    Q_OBJECT

public:
    RenderModel(QWidget *parent = Q_NULLPTR);

private:
    Ui::RenderModelClass ui;
};
