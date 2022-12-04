#include "mainwindow.h"
#include "ui_mainwindow.h"

//构造
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{



    ui->setupUi(this);
    //设置窗口名称
    setWindowTitle("游戏菜单页面");
    StartW=new StartMainWindow;

    //返回主菜单信号与槽函数
    connect(StartW,&StartMainWindow::backToMainWindow,this,[=](){
        StartW->hide();
        this->show();
    });

}
//析构
MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_StartButton_clicked()//进入游戏页面
{
   this->hide();
   StartW->show();

   //发出开始游戏的信号，游戏页面开始读取单词
   emit this->StartW->startGame();
}

