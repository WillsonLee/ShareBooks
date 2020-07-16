#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QDebug>
#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <face_recognize.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

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
//    int control_width=this->width()-36;//36 is approximately the left right margins combined
    int control_height=(this->height()-30)*3/8;//is approximately the spacing and toolbar,menu,title heights combined
//    ui->borrow_button->setFixedWidth(control_width-6);
    ui->borrow_button->setFixedHeight((control_height-6)/2-4);
//    ui->return_button->setFixedWidth((ui->borrow_button->width()-6)*2/5);//6 is spacing between return_button and share_button
    ui->return_button->setFixedHeight(ui->borrow_button->height());
//    ui->share_button->setFixedWidth(ui->return_button->width()*3/2);
    ui->share_button->setFixedHeight(ui->return_button->height());
    ui->borrow_button->setImage(QPixmap(":/icons/icons/borrow.png"));
    ui->borrow_button->setRadius(10);
    ui->return_button->setImage(QPixmap(":/icons/icons/return.png"));
    ui->return_button->setRadius(10);
    ui->share_button->setImage(QPixmap(":/icons/icons/share.png"));
    ui->share_button->setRadius(10);
    //init timer
    timer=NULL;
    timeout=3;
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
    //init system command environment
    initFullPathCmd();
}

void MainWindow::initCarousel()
{
    QDir path("../database/Carousel/");
    QFileInfoList list=path.entryInfoList(QStringList()<<"*.png"<<"*.jpg"<<"*.jpeg");
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
    std::string stuff_info_path("./database/stuffs.txt");
    std::ifstream in_file(stuff_info_path);  // open file
    if(in_file) {
        std::string line;
        while(std::getline(in_file, line)) {  // read a line
            std::istringstream ss(line);
            std::string tmp;
            StuffInfo tmp_stuff;
            // read id
            std::getline(ss, tmp, ',');
            if(tmp.empty())
                continue;
            tmp_stuff.id = std::stoi(tmp);
            // read name
            std::getline(ss, tmp, ',');
            tmp_stuff.name = tmp.c_str();
            // read quota
            std::getline(ss, tmp, ',');
            tmp_stuff.quota = std::stoi(tmp);
            // read currently borrowed books' id
            while(std::getline(ss, tmp, ',')) {
                tmp_stuff.current_hold.append(std::stoi(tmp));
            }
            stuffs[tmp_stuff.id] = tmp_stuff;
        }
    } else {
        std::cerr << "cann't open stuffs.txt!";
    }

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
    updateCountDown(timeout);
    connect(timer,&QTimer::timeout,this,[&](){
        backToMain();
    });
    timer->start(1000);
    //face recognize code
    QDir dir("../face_cam");
    QStringList nameFilters;
    nameFilters << "*.jpg" << "*.jpeg" << "*.png";
    QFileInfoList imgs = dir.entryInfoList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
    if(imgs.size()==0){
        qDebug()<<"no faces found"<<endl;
        ui->camera_label->setPixmap(QPixmap("../icons/no_cam.jpg"));
    }
    else if(imgs.size()==1){
        //read image to display
        ui->camera_label->setPixmap(QPixmap(imgs[0].absolutePath()));
        std::vector<int> faces=face_recognize("../database/faces/",dir.absolutePath().toStdString());
        if(faces.size()==1){
            if(faces[0]!=-1){//correctly recognized
                qDebug()<<"stuff id:"<<faces[0]<<endl;
            }
            else{//unrecognized face
                qDebug()<<"unrecognized face!"<<endl;
            }
        }
        else{//multi-face
            qDebug()<<"multi face detected!"<<endl;
        }
    }
    else{
        ui->camera_label->setPixmap(QPixmap("../icons/no_cam.jpg"));
        qDebug()<<"too much images!"<<endl;
    }
}

void MainWindow::toBookModule()
{
    ui->stackedWidget->setCurrentIndex(3);
    if(timer&&timer->isActive()){
        timer->stop();
    }
    timer=new QTimer(this);
    updateCountDown(timeout);
    connect(timer,&QTimer::timeout,this,[&](){
        backToMain();
    });
    timer->start(1000);
}

void MainWindow::backToMain()
{
    if(count_down>0){
        updateCountDown(count_down-1);
        return;
    }
    if(timer&&timer->isActive()){
        timer->stop();
        timer=NULL;
    }
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

void MainWindow::on_faceReturn_clicked()
{
    count_down=0;//set time out
    backToMain();
}

void MainWindow::updateCountDown(int time)
{
    if(ui->stackedWidget->currentIndex()==1){//in face recognition interface
        count_down=time;
        ui->faceReturn->setText(QString("<|back(%1)").arg(count_down));
    }
    else if(ui->stackedWidget->currentIndex()==3){

    }
}
