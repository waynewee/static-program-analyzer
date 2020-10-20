#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qgraphicsscene.h>
#include "GUIWrapper.h"
namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();


public slots:
	void btnLoad_clicked();
	void btnRun_clicked();
	void on_astButton_clicked();
	void on_cfgButton_clicked();

private:
	Ui::MainWindow *ui;
	GUIWrapper *wrapper;
	QGraphicsScene* scene;
	QGraphicsEllipseItem* ellipse;
	QGraphicsRectItem* rectangle;
	QGraphicsTextItem* text;
};

#endif // MAINWINDOW_H
