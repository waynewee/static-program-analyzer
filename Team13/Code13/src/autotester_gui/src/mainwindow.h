#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qgraphicsscene.h>
#include "GUIWrapper.h"
#include <TNode.h>

typedef pair<int, int> COORDINATES;
typedef pair<string, COORDINATES> AST_NODE;
typedef vector<AST_NODE> NODE_LIST;
typedef vector<NODE_LIST*> LIST_OF_NODE_LISTS;

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
	QString file_contents_;
	int CountMaxDepth(TNode* root_node, int depth);
	void PopulateNodeList(TNode* root_node, LIST_OF_NODE_LISTS list_of_node_lists, int max_depth, int curr_depth);
	void PrintNodeList(LIST_OF_NODE_LISTS list_of_node_lists);
};

#endif // MAINWINDOW_H
