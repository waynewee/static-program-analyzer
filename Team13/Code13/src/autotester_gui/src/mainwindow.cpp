
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

	main_brush_ = QBrush(Qt::blue);
	outline_pen_ = QPen(Qt::black);
	outline_pen_.setWidth(1);
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
	wrapper->Evaluate(ui->txtQuery->toPlainText().toStdString(),results);
	QString result_str;

	int i = 0;

	for (auto const& result : results) {

		result_str += QString(result.c_str());
		if (i < results.size()) {
			result_str += ", ";
		}
	}

	ui->txtResult->document()->setPlainText(result_str);

	for (auto const& result : results) {
		for (NODE_LIST* node_list : *list_of_node_lists_) {
			for (GUINode* node : *node_list) {
				if (node->node_->GetStmtIndex() == stoi(result.c_str())) {
					DrawNode(node, Qt::red);
				}
			}
		}
	}

}

// Reference this https://www.bogotobogo.com/Qt/Qt5_QGraphicsView_QGraphicsScene.php
void MainWindow::on_astButton_clicked() {

	string file_contents = file_contents_.toUtf8().constData();
	FrontendWrapper wrapper(file_contents);
	TNode* root_node = wrapper.GetAST();
	root_node->Print(root_node);

	
	int max_depth = CountMaxDepth(root_node, 0);
	int leaf_x = 0;

	InitialiseListOfNodeLists(max_depth);

	PopulateNodeList(root_node, NULL, 0, &leaf_x);

	PrintNodeList();

	DrawAST();

}

void MainWindow::on_cfgButton_clicked() {
	cout << "Hello World" << endl;
}

void MainWindow::on_verticalSlider_valueChanged(int value) {
	ui->graphicsView->resetMatrix();
	
	double scale = value / 50.0;

	ui->graphicsView->scale(scale, scale);

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

void MainWindow::PopulateNodeList(TNode* root_node, TNode* parent_node, int curr_depth, int* leaf_x) {

	if (curr_depth > list_of_node_lists_->size()) {
		return;
	}

	NODE_LIST* node_list = list_of_node_lists_->at(curr_depth);

	int y = curr_depth * unit_y_;
	int pos = node_list->size();
	int x = pos * unit_x_;

	if (root_node->GetChildrenVector().size() == 0) {
		GUINode* gui_node = new GUINode(root_node, parent_node, *leaf_x, y);
		*leaf_x += unit_x_;
		node_list->push_back(gui_node);
	}
	else {

		for (TNode* child : root_node->GetChildrenVector()) {
			PopulateNodeList(child, root_node, curr_depth + 1, leaf_x);
		}

		NODE_LIST* child_node_list = list_of_node_lists_->at(curr_depth + 1);
		NODE_LIST* filtered_child_node_list = new NODE_LIST();

		for (GUINode* gui_node : *child_node_list) {
			if (gui_node->parent_ == root_node) {
				filtered_child_node_list->push_back(gui_node);
			}
		}

		int start_x = filtered_child_node_list->at(0)->x_;
		int end_x = filtered_child_node_list->at(filtered_child_node_list->size() - 1)->x_;

		int parent_x = (end_x + start_x) / 2;

		GUINode* gui_node = new GUINode(root_node, parent_node, parent_x, y);
		node_list->push_back(gui_node);
		
	}

}

void MainWindow::PrintNodeList() {
	
	int level = 0;
	for (NODE_LIST* node_list : *list_of_node_lists_) {
		
		cout << "Level " << level << ": [";
		
		int idx = 0;
		for (GUINode* gui_node : *node_list) {
			cout << gui_node->node_->getData();
			if (idx < node_list->size() - 1) {
				cout << ", ";
			}
		}
		cout << "]" << endl;
		level += 1;
	}
}

void MainWindow::InitialiseListOfNodeLists(int max_depth) {

	list_of_node_lists_ = new LIST_OF_NODE_LISTS();
	
	for (int i = 0; i < max_depth + 1; i++) {
		NODE_LIST* node_list = new NODE_LIST;
		list_of_node_lists_->push_back(node_list);
	}
}

void MainWindow::DrawAST() {

	scene->clear();

	// set dimensions
	int diameter = 30;

	for (NODE_LIST* node_list : *list_of_node_lists_) {
		for (GUINode* gui_node : *node_list) {
			DrawNode(gui_node);
		}
	}

	for (int i = 1; i < list_of_node_lists_->size(); i++) {
		NODE_LIST* node_list = list_of_node_lists_->at(i);
		NODE_LIST* parent_node_list = list_of_node_lists_->at(i-1);

		for (GUINode* gui_node : *node_list) {
			for (GUINode* parent_node : *parent_node_list) {
				if (gui_node->parent_ == parent_node->node_) {
					//draw edge
					scene->addLine(
						QLine(
							gui_node->x_ + line_offset_x, 
							gui_node->y_, 
							parent_node->x_ + line_offset_x, 
							parent_node->y_ + line_offset_y
						), outline_pen_);
				}
			}
		}
		
	}

	ui->graphicsView->fitInView(ui->graphicsView->sceneRect(), Qt::KeepAspectRatio);

}

void MainWindow::DrawNode(GUINode* gui_node) {
	scene->addEllipse(gui_node->x_, gui_node->y_, diameter_, diameter_, outline_pen_, main_brush_);
	QGraphicsTextItem* text = scene->addText(QString::fromStdString(gui_node->node_->getData()));
	text->setPos(gui_node->x_ - text_offset_x, gui_node->y_ + text_offset_y);
}

void MainWindow::DrawNode(GUINode* gui_node, QColor color) {
	scene->addEllipse(gui_node->x_, gui_node->y_, diameter_, diameter_, outline_pen_, QBrush(color));
}