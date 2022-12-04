#include "mainwindow.h"
#include<QPushButton>
#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);//应用程序对象，有且仅有一个
    MainWindow w;
    w.show();
    return a.exec();
}








