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
#include <stdio.h>

/*
函数：SplitString
功能：实现字符逗号隔开
*/
void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
    std::string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(std::string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2-pos1));

        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length())
        v.push_back(s.substr(pos1));
}
//get all image files(*.jpg;*.jpeg;*.png;*.bmp) in the given folder
//filename:the base name of the files without path or suffix
//files:the full name of the files
void getFiles(std::string path, std::vector<std::string>& filename, std::vector<std::string>& files )
{
//    long   hFile   =   0;
//    struct _finddata_t fileinfo;
//    std::string p;
//    if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) !=  -1)
//    {
//        do
//        {
//            if((fileinfo.attrib &  _A_SUBDIR))
//            {
//                if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)
//                    getFiles( p.assign(path).append("\\").append(fileinfo.name), filename ,files);
//            }
//            else
//            {
//                filename.push_back(p.assign(fileinfo.name));
//                files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
//            }
//        }while(_findnext(hFile, &fileinfo)  == 0);
//        _findclose(hFile);
//    }
    QDir dir(QString::fromStdString(path));
    QFileInfoList f_list=dir.entryInfoList(QStringList()<<"*.jpg"<<"*.jpeg"<<"*.png"<<"*.bmp");
    for(int i=0;i<f_list.size();++i){
        filename.push_back(f_list[i].baseName().toStdString());
        files.push_back(f_list[i].absoluteFilePath().toStdString());
    }
    //test
    qDebug()<<"test get all image files:"<<endl;
    for(int i=0;i<filename.size();++i){
        qDebug()<<"file base name:"<<QString::fromStdString(filename[i])<<",full name:"<<QString::fromStdString(files[i])<<endl;
    }
    //
}

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
    initPages();
    initProperties();
    startupTasks();
    toStuffInfo();

    displayBooks();//byboss
}

void MainWindow::displayBooks(){

    int id1=1,id2=1,id3=1;
    if(top_books.size()==1)
    {
        id1=top_books.at(0).ISBN;
    }
    if(top_books.size()==2)
    {
        id1=top_books.at(0).ISBN;
        id2=top_books.at(1).ISBN;
    }
    if(top_books.size()>=3)
    {
        id1=top_books.at(0).ISBN;
        id2=top_books.at(1).ISBN;
        id3=top_books.at(2).ISBN;
    }
    QString s1 = QString::number(id1);
    QString s2 = QString::number(id2);
    QString s3 = QString::number(id3);
    QPixmap pix1,pix2,pix3;
    pix1.load("../database/BookImages/"+s1+".jpg");
    pix2.load("../database/BookImages/"+s2+".jpg");
    pix3.load("../database/BookImages/"+s3+".jpg");
    ui->showbook1->setFixedWidth(this->width()/4);
    ui->showbook1->setPixmap(pix1.scaled(ui->showbook1->width(),ui->showbook1->height(),Qt::KeepAspectRatio));
    ui->showbook2->setFixedWidth(this->width()/4);
    ui->showbook2->setPixmap(pix2.scaled(ui->showbook2->width(),ui->showbook2->height(),Qt::KeepAspectRatio));
    ui->showbook3->setFixedWidth(this->width()/4);
    ui->showbook3->setPixmap(pix3.scaled(ui->showbook3->width(),ui->showbook3->height(),Qt::KeepAspectRatio));
    //缩放
    ui->showbook1->setScaledContents(true);
    ui->showbook2->setScaledContents(true);
    ui->showbook3->setScaledContents(true);

}

void MainWindow::initPages()
{
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
    //init identity confirmation page
}

void MainWindow::initProperties()
{
    //init timer
    timer=NULL;
    timeout=16;
    //read book info from datebase
    top_books=Cache<BookInfo>(10);
    //init current status of face recog and book info
    currentOperation=-1;
    currentUser=-1;
    currentBook=-1;
}

void MainWindow::startupTasks()
{
    scanBooks("../datebase/books.txt");
    //read pesonnel data
    readStuffInfo();
    //read book cover image
    readBookCoverImages();
    //init system command environment
    initFullPathCmd();
    //init call back
    connect(&recog,&QThread::finished,this,[&](){
        ui->camera_label->stopSpinning();
    });
    connect(&recog,&Recognizing::recognized,this,[&](int result){
        if(result==-2){
            ui->tip_label->setText("请一次识别一个员工!");
        }
        else{
            currentUser=result;
            toStuffInfo();
        }
    });
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
    char* ch;
    QByteArray ba = file.toLocal8Bit();
    ch = ba.data();
    std::ifstream in(ch);
    std::string linestr;
    BookInfo bookinfo;
    std::vector<std::string> stringtemp;
    if(in){
        while(getline(in,linestr)){
            std::vector<std::string> ver;
            SplitString(linestr, ver,",");

            bookinfo.ISBN = atoi(ver[0].c_str());

            QString qlinestr1=QString::fromStdString(ver[1]);
            bookinfo.title = qlinestr1;

            QString qlinestr2=QString::fromStdString(ver[2]);
            bookinfo.author = qlinestr2;

            bookinfo.year = atoi(ver[3].c_str());

            QString qlinestr4=QString::fromStdString(ver[4]);
            bookinfo.brief = qlinestr4;

            if(ver[5] == ""){
                QDateTime time = QDateTime::currentDateTime();
                int timeT = time.toTime_t();
                bookinfo.timeStamp = timeT;
                bookinfo.frequency = 0;
            }else {
                bookinfo.timeStamp = atoi(ver[5].c_str());

                bookinfo.frequency = atoi(ver[6].c_str());
            }

            bookinfo.inCloset = atoi(ver[7].c_str());
            books.insert(bookinfo.ISBN,bookinfo);
        }
        top_books.put(bookinfo);
    }
    in.close();
}

void MainWindow::saveBooksData()
{
    std::ofstream f1("./database/books.txt", std::ios::app);
    if(!f1)return;
    QHash<int,BookInfo> books;
    QHash<int,BookInfo> ::const_iterator it;
    int i = 0;
    for(it = books.begin();it != books.end();it++,i++){
        switch (i) {
        case 0:
            f1<<std::to_string(it.value().ISBN)<<",";
            break;
        case 1:
            f1<<it.value().title.toStdString()<<",";
            break;
        case 2:
            f1<<it.value().author.toStdString()<<",";
            break;
        case 3:
            f1<<std::to_string(it.value().year)<<",";
            break;
        case 4:
            f1<<it.value().brief.toStdString()<<",";
            break;
        case 5:
            f1<<std::to_string(it.value().timeStamp)<<",";
            break;
        case 6:
            f1<<std::to_string(it.value().frequency)<<",";
            break;
        case 7:
            f1<<std::to_string(it.value().inCloset)<<endl;
            break;
        default:
            break;
        }

    }
    f1.close();
}

void MainWindow::readStuffInfo()
{
    std::string stuff_info_path("../database/stuffs.txt");
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
//    //test
//    for(auto k:stuffs.keys()){
//        qDebug()<<"id:"<<stuffs[k].id<<",name:"<<stuffs[k].name<<",quota:"<<stuffs[k].quota<<",and holds:";
//        const QVector<int> &holds=stuffs[k].current_hold;
//        for(int i=0;i<holds.size();++i)qDebug()<<holds[i]<<",";
//    }
//    //
}

void MainWindow::readBookCoverImages()
{
    std::vector<std::string> filename;
    std::vector<std::string> files;
    char * filePath = ".\\database\\BookImages";
    getFiles(filePath,filename,files);
    char str[30];
    int size = filename.size();
    std::string	str_temp;
    for (int i = 0;i < size;i++){
        int pase = 0;
        pase = filename[i].find(".");
        str_temp = filename[i].substr(0,pase);
        QImage image;
        QByteArray pData;
        QString path = QString::fromStdString(files[i]);
        QFile *file=new QFile(path);
        file->open(QIODevice::ReadOnly);
        pData=file->readAll();
        image.loadFromData(pData);
        int id = atoi(str_temp.c_str());
        bookCovers.insert(id,image);
    }
}

void MainWindow::switchToPage(int page)
{
    ui->stackedWidget->setCurrentIndex(page);
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

void MainWindow::toFaceModule()
{
    switchToPage(1);
    ui->tip_label->setText("请进行面部识别");
    //face recognize code
    QDir dir("../face_cam");
    QStringList nameFilters;
    nameFilters << "*.jpg" << "*.jpeg" << "*.png";
    QFileInfoList imgs = dir.entryInfoList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
    if(imgs.size()==0){
        ui->camera_label->setPixmap(QPixmap("../icons/no_cam.jpg"));
    }
    else if(imgs.size()==1){
        //read image to display
        ui->camera_label->setPixmap(QPixmap(imgs[0].absoluteFilePath()));
        ui->camera_label->startSpinning();
        recog.start();
    }
    else{
        ui->camera_label->setPixmap(QPixmap("../icons/no_cam.jpg"));
    }
}

void MainWindow::toStuffInfo()
{
    switchToPage(2);
    if(currentUser!=-1){
        QPixmap pix;
        pix.load("../database/faces/"+QString::number(currentUser)+".jpg");
        ui->headLabel->setFixedSize(QSize(220,220));
        float ratio=qMin(pix.width()/ui->headLabel->width(),pix.height()/ui->headLabel->height());
        pix=pix.scaled(QSize(pix.width()/ratio,pix.height()/ratio));
        pix=pix.copy(QRect((pix.width()-ui->headLabel->width())/2,0,ui->headLabel->width(),ui->headLabel->height()));
        ui->headLabel->setPixmap(pix);
        ui->nameEdit->setText(stuffs[currentUser].name);
        ui->idEdit->setText(QString::number(stuffs[currentUser].id));
        ui->quotaEdit->setText(QString::number(stuffs[currentUser].quota));
    }
    else{
        QPixmap pix;
        pix.load("../icons/unknown_person.png");
        ui->headLabel->setFixedSize(QSize(220,220));
        ui->headLabel->setPixmap(pix.scaled(ui->headLabel->size()));
        ui->nameEdit->setText("unknown");
        ui->idEdit->setText("-1");
        ui->quotaEdit->setText("0");
    }
}

void MainWindow::toBookModule()
{
    switchToPage(3);
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
    if(recog.isRunning()){
        recog.terminate();
    }
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
    count_down=time;
    if(ui->stackedWidget->currentIndex()==1){//in face recognition interface
        ui->faceReturn->setText(QString("<|back(%1)").arg(count_down));
    }
    else if(ui->stackedWidget->currentIndex()==3){

    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug()<<"closing..."<<endl;
    saveBooksData();
}

void Recognizing::run()
{
    std::vector<int> faces=face_recognize("../database/faces/","../face_cam");
    if(faces.size()==1){
        emit recognized(faces[0]);
    }
    else{//multi-face
        emit recognized(-2);
    }
}

void MainWindow::on_id_confirm_clicked()
{
    if(currentUser!=-1){
        toBookModule();
    }
    else{
        updateCountDown(0);
        backToMain();
    }
}

void MainWindow::on_id_cancel_clicked()
{
    updateCountDown(0);
    backToMain();
}
