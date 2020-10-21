
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "GUIWrapper.h"
#include <iostream>
#include <QFileDialog>
#include <QtGui>
#include <QTextStream>
#include <FrontendWrapper.h>
#include <TNode.h>
#include <vector>
#include <list>
#include "GUINode.h"


using namespace std;
MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	scene = new QGraphicsScene(this);

	ui->graphicsView->setScene(scene);

	wrapper = new GUIWrapper();
}

MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::btnLoad_clicked() {

	QString src_name = QFileDialog::getOpenFileName(this,
		tr("Open Source file"), ".", tr("txt Files (*.txt)"));

	QFile file;
	file.setFileName(src_name);
	if (!file.open(QIODevice::ReadOnly))
		return;
	file_contents_ = file.readAll();
	ui->txtCodeEditor->document()->setPlainText(file_contents_);	
	wrapper->parse(src_name.toStdString());
}

void MainWindow::btnRun_clicked() {
	std::list<std::string> results;
	wrapper->evaluate(ui->txtQuery->toPlainText().toStdString(),results);
	QString result_str;

	for (auto const& result : results) {

		result_str += QString(result.c_str());
	}
	ui->txtResult->document()->setPlainText(result_str);
}

// Reference this https://www.bogotobogo.com/Qt/Qt5_QGraphicsView_QGraphicsScene.php
void MainWindow::on_astButton_clicked() {

	string file_contents = file_contents_.toUtf8().constData();
	FrontendWrapper wrapper(file_contents);
	TNode* root_node = wrapper.GetAST();
	root_node->Print(root_node);

	
	int max_depth = CountMaxDepth(root_node, 0);

	LIST_OF_NODE_LISTS list_of_node_lists;

	InitialiseListOfNodeLists(&list_of_node_lists, max_depth);

	PopulateNodeList(root_node, NULL, list_of_node_lists, 0);

	PrintNodeList(list_of_node_lists);

	DrawAST(list_of_node_lists);

}

void MainWindow::on_cfgButton_clicked() {
	cout << "Hello World" << endl;
}

int MainWindow::CountMaxDepth(TNode* root_node, int depth) {
	//base case
	if (root_node->GetChildrenVector().size() == 0) {
		return depth;
	}
	else {

		vector<int> depths;

		for (TNode* node : root_node->GetChildrenVector()) {
			depths.push_back(CountMaxDepth(node, depth + 1));
		}

		int max = depths.at(0);

		for (int depth : depths) {
			if (depth > max) {
				max = depth;
			}
		}

		return max;
	}
}

void MainWindow::PopulateNodeList(TNode* root_node, TNode* parent_node, LIST_OF_NODE_LISTS list_of_node_lists, int curr_depth) {

	if (curr_depth > list_of_node_lists.size()) {
		return;
	}

	int unit_y = 50;
	int unit_x = 100;

	NODE_LIST* node_list = list_of_node_lists.at(curr_depth);

	int y = curr_depth * unit_y;
	int pos = node_list->size();
	int x = pos * unit_x;

	if (root_node->GetChildrenVector().size() == 0) {
		GUINode* gui_node = new GUINode(root_node, parent_node, x, y);
		node_list->push_back(gui_node);
	}
	else {

		for (TNode* child : root_node->GetChildrenVector()) {
			PopulateNodeList(child, root_node, list_of_node_lists, curr_depth + 1);
		}

		NODE_LIST* child_node_list = list_of_node_lists.at(curr_depth + 1);
		NODE_LIST* filtered_child_node_list = new NODE_LIST();

		for (GUINode* gui_node : *child_node_list) {
			if (gui_node->parent_ == root_node) {
				filtered_child_node_list->push_back(gui_node);
			}
		}

		int start_x = filtered_child_node_list->at(0)->x_;
		int end_x = filtered_child_node_list->at(filtered_child_node_list->size() - 1)->x_;

		int parent_x = x + (end_x + start_x) / 2;

		GUINode* gui_node = new GUINode(root_node, parent_node, parent_x, y);
		node_list->push_back(gui_node);
		
	}

}

void MainWindow::PrintNodeList(LIST_OF_NODE_LISTS list_of_node_lists) {
	
	int level = 0;
	for (NODE_LIST* node_list : list_of_node_lists) {
		
		cout << "Level " << level << ": [";
		
		int idx = 0;
		for (GUINode* ast_node : *node_list) {
			cout << ast_node->node_->getData();
			if (idx < node_list->size() - 1) {
				cout << ", ";
			}
		}
		cout << "]" << endl;
		level += 1;
	}
}

void MainWindow::InitialiseListOfNodeLists(LIST_OF_NODE_LISTS* list_of_node_lists, int max_depth) {
	for (int i = 0; i < max_depth + 1; i++) {
		NODE_LIST* node_list = new NODE_LIST;
		list_of_node_lists->push_back(node_list);
	}
}

void MainWindow::DrawAST(LIST_OF_NODE_LISTS list_of_node_lists) {

	scene->clear();

	QBrush blueBrush(Qt::blue);
	QPen outlinePen(Qt::black);
	outlinePen.setWidth(2);

	// set dimensions
	int diameter = 30;

	for (NODE_LIST* node_list : list_of_node_lists) {
		for (GUINode* ast_node : *node_list) {
			scene->addEllipse(ast_node->x_, ast_node->y_, diameter, diameter, outlinePen, blueBrush);
			QGraphicsTextItem* text = scene->addText(QString::fromStdString(ast_node->node_->getData()));

			text->setPos(ast_node->x_ - 20, ast_node-> y_ + 30);
		}
	}

	//ellipse = scene->addEllipse(0, -100, 60, 60, outlinePen, blueBrush);
}