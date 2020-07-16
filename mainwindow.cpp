#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QDebug>
#include <QPixmap>
#include <QBitmap>
#include <QPainter>

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
    //set buttons size and image
    int control_width=this->width()-36;//36 is approximately the left right margins combined
    int control_height=(this->height()-30)*3/8;//is approximately the spacing and toolbar,menu,title heights combined
    ui->borrow_button->setFixedWidth(control_width-6);
    ui->borrow_button->setFixedHeight((control_height-6)/2-4);
    ui->return_button->setFixedWidth((ui->borrow_button->width()-6)*2/5);//6 is spacing between return_button and share_button
    ui->return_button->setFixedHeight(ui->borrow_button->height());
    ui->share_button->setFixedWidth(ui->return_button->width()*3/2);
    ui->share_button->setFixedHeight(ui->return_button->height());
    ui->borrow_button->setImage(QPixmap(":/icons/icons/borrow.png"));
    ui->borrow_button->setRadius(10);
    ui->return_button->setImage(QPixmap(":/icons/icons/return.png"));
    ui->return_button->setRadius(10);
    ui->share_button->setImage(QPixmap(":/icons/icons/share.png"));
    ui->share_button->setRadius(10);
    //init timer
    timer=NULL;
    timeout=3000;
    //read book info from datebase
    top_books=Cache<BookInfo>(10);
    scanBooks("../datebase/books.txt");
    //read pesonnel data
    readStuffInfo();
    //read book cover image
    readBookCoverImages();
    //init current status of face recog and book info
    currentUser=-1;
    currentBook=-1;
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

void MainWindow::scanBooks(QString file)
{

}

void MainWindow::saveBooksData()
{

}

void MainWindow::readStuffInfo()
{

}

void MainWindow::readBookCoverImages()
{

}

void MainWindow::toFaceModule()
{
    ui->stackedWidget->setCurrentIndex(1);
    if(timer&&timer->isActive()){
        timer->stop();
    }
    timer=new QTimer(this);
    connect(timer,&QTimer::timeout,this,[&](){
        backToMain();
    });
    timer->start(timeout);
}

void MainWindow::toBookModule()
{
    ui->stackedWidget->setCurrentIndex(3);
    if(timer&&timer->isActive()){
        timer->stop();
    }
    timer=new QTimer(this);
    connect(timer,&QTimer::timeout,this,[&](){
        backToMain();
    });
    timer->start(timeout);
}

void MainWindow::backToMain()
{
    ui->stackedWidget->setCurrentIndex(0);
    currentUser=-1;
    currentBook=-1;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_borrow_button_clicked()
{
    toFaceModule();
}

void MainWindow::on_return_button_clicked()
{
    toFaceModule();
}

void MainWindow::on_share_button_clicked()
{
    toFaceModule();
}
