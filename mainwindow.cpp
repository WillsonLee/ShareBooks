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
    //set buttons style and size
    ui->borrow_button->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;");
    ui->return_button->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;");
    ui->share_button->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px;");
    int control_width=this->width()-36;//36 is approximately the left right margins combined
    int control_height=(this->height()-30)*3/8;//is approximately the spacing and toolbar,menu,title heights combined
    ui->borrow_button->setFixedWidth(control_width-6);
    ui->borrow_button->setFixedHeight((control_height-6)/2-4);
    ui->return_button->setFixedWidth((ui->borrow_button->width()-6)/2);//6 is spacing between return_button and share_button
    ui->return_button->setFixedHeight(ui->borrow_button->height());
    ui->share_button->setFixedWidth(ui->return_button->width());
    ui->share_button->setFixedHeight(ui->return_button->height());
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
