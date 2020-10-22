#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qgraphicsscene.h>
#include "GUIWrapper.h"
#include <TNode.h>
#include <QGraphicsItem>
#include "GUINode.h"

typedef vector<GUINode*> NODE_LIST;
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
	void PopulateNodeList(TNode* root_node, TNode* parent_node, int curr_depth, int* leaf_x);
	void PrintNodeList();
	void InitialiseListOfNodeLists(int max_depth);
	void DrawAST();
	void DrawNode(GUINode* gui_node);
	void DrawNode(GUINode* gui_node, QColor color);
	
	LIST_OF_NODE_LISTS* list_of_node_lists_;

	const int diameter_ = 30;
	const int unit_x_ = 50;
	const int unit_y_ = 50;
	const int line_offset_x = diameter_ / 2;
	const int line_offset_y = diameter_;

};

#endif // MAINWINDOW_H
