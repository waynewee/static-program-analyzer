
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

	for (int i = 0; i < max_depth + 1; i++) {
		NODE_LIST* node_list = new NODE_LIST;
		list_of_node_lists.push_back(node_list);
	}

	PopulateNodeList(root_node, list_of_node_lists, 0, max_depth);

	PrintNodeList(list_of_node_lists);

	QBrush blueBrush(Qt::blue);
	QPen outlinePen(Qt::black);
	outlinePen.setWidth(2);

	// set dimensions
	int diameter = 30;
	int unit_x = 50;
	int unit_y = 50;

	int start_x = max_depth * unit_x;

	int y = 0;
	for (NODE_LIST* node_list : list_of_node_lists) {
		int x = 0;
		for (AST_NODE ast_node : *node_list) {
			scene->addEllipse(start_x + x * unit_x, y * unit_y, diameter, diameter, outlinePen, blueBrush);
			x += 1;
		}
		start_x -= unit_x;
		y += 1;
	}

	//ellipse = scene->addEllipse(0, -100, 60, 60, outlinePen, blueBrush);

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

void MainWindow::PopulateNodeList(TNode* root_node, LIST_OF_NODE_LISTS list_of_node_lists, int curr_depth, int max_depth) {

	cout << "HELLO" << endl;

	if (curr_depth > max_depth) {
		return;
	}

	NODE_LIST* node_list = list_of_node_lists.at(curr_depth);
	//create element
	AST_NODE ast_node = make_pair(root_node->getData(), make_pair(4, 4));

	node_list->push_back(ast_node);

	for (TNode* node : root_node->GetChildrenVector()) {
		PopulateNodeList(node, list_of_node_lists, curr_depth + 1, max_depth);
	}


}

void MainWindow::PrintNodeList(LIST_OF_NODE_LISTS list_of_node_lists) {
	
	int level = 0;
	for (NODE_LIST* node_list : list_of_node_lists) {
		
		cout << "Level " << level << ": [";
		
		int idx = 0;
		for (AST_NODE ast_node : *node_list) {
			cout << ast_node.first;
			if (idx < node_list->size() - 1) {
				cout << ", ";
			}
		}
		cout << "]" << endl;
		level += 1;
	}
}