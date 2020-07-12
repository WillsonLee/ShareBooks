#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //disable maximise button and set window size unresizable
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());
    //set current view at main page and init main page layout
    ui->stackedWidget->setCurrentIndex(0);
    //init carousel view
    initCarousel();
    ui->borrow_button->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;");
    ui->return_button->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;");
    ui->share_button->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;");
}

void MainWindow::initCarousel()
{
    QDir path("../database/Carousel/");
    QFileInfoList list=path.entryInfoList(QStringList()<<"*.png"<<"*.jpg");
    for(int i=0;i<list.size();++i){
        ui->adsView->addImage(list.at(i).absoluteFilePath());
    }
    ui->adsView->startPlay();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_borrow_button_clicked()
{

}

void MainWindow::on_return_button_clicked()
{

}

void MainWindow::on_share_button_clicked()
{

}
