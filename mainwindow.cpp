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
#include <queue>

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
//    //test
//    qDebug()<<"test get all image files:"<<endl;
//    for(int i=0;i<filename.size();++i){
//        qDebug()<<"file base name:"<<QString::fromStdString(filename[i])<<",full name:"<<QString::fromStdString(files[i])<<endl;
//    }
//    //
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
//    toStuffInfo();
//    currentOperation=0;
//    toBookModule();
//    currentOperation=2;
//    currentUser=1007;
//    toBookModule();
}

void MainWindow::displayBooks(){
    if(libraryView){
        delete libraryView;
    }
    libraryView=new QHBoxLayout();
    ui->libraryScroll->setLayout(libraryView);
    int w=this->width()/4.5;
    int h=w*1.4;
    for(int i=0;i<top_books.size();++i){
        QPixmap pix=bookCovers[top_books.at(i).ISBN];
        QLabel *label=new QLabel();
        label->setFixedSize(QSize(w,h));
        label->setPixmap(pix.scaled(QSize(w,h)));
        label->setStyleSheet("QLabel{border:1px solid gray;}");
        libraryView->addWidget(label);
    }
//    int id1=-1,id2=-1,id3=-1,id4=-1;
//    if(top_books.size()>0){
//        id1=top_books.at(0).ISBN;
//    }
//    if(top_books.size()>1){
//        id2=top_books.at(1).ISBN;
//    }
//    if(top_books.size()>2){
//        id3=top_books.at(2).ISBN;
//    }
//    if(top_books.size()>3){
//        id4=top_books.at(3).ISBN;
//    }
//    QPixmap pix1=bookCovers[id1];
//    QPixmap pix2=bookCovers[id2];
//    QPixmap pix3=bookCovers[id3];
//    QPixmap pix4=bookCovers[id4];

//    ui->showbook1->setFixedSize(QSize(w,h));
//    ui->showbook1->setPixmap(pix1.scaled(ui->showbook1->size()));
//    ui->showbook2->setFixedSize(QSize(w,h));
//    ui->showbook2->setPixmap(pix2.scaled(ui->showbook2->size()));
//    ui->showbook3->setFixedSize(QSize(w,h));
//    ui->showbook3->setPixmap(pix3.scaled(ui->showbook3->size()));
//    ui->showbook4->setFixedSize(QSize(w,h));
//    ui->showbook4->setPixmap(pix4.scaled(ui->showbook4->size()));
    //缩放
//    ui->showbook1->setScaledContents(true);
//    ui->showbook2->setScaledContents(true);
//    ui->showbook3->setScaledContents(true);
//    //test
//    qDebug()<<"test display book:"<<endl;
//    qDebug()<<"=================="<<endl;
//    qDebug()<<"top_books size:"<<top_books.size()<<endl;
//    qDebug()<<"size of pix1:"<<pix1.size()<<endl;
//    qDebug()<<"size of pix2:"<<pix2.size()<<endl;
//    qDebug()<<"size of pix3:"<<pix3.size()<<endl;
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
    stuffHoldsView=NULL;
    libraryView=NULL;
}

void MainWindow::startupTasks()
{
    scanBooks("../database/books.txt",books);
    QList<int> keys=books.keys();
    for(int k:keys){
        top_books.put(books[k]);
    }
//    //test
//    qDebug()<<"scan books test:"<<endl;
//    qDebug()<<"================"<<endl;
//    qDebug()<<"number of books:"<<books.size()<<endl;
//    qDebug()<<"number of top books:"<<top_books.size()<<endl;
//    qDebug()<<"top_book info:"<<endl;
//    for(int i=0;i<top_books.size();++i){
//        BookInfo bi=top_books.at(i);
//        qDebug()<<"ISBN-"<<bi.ISBN<<",title<<"<<bi.title<<">>,author:"<<bi.author<<",published in:"<<bi.year<<",time stamp:"<<bi.timeStamp<<",frequency:"<<bi.frequency<<endl;
//        qDebug()<<"brief discription:"<<bi.brief<<endl;
//    }
//    //
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
    displayBooks();//byboss
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

void MainWindow::scanBooks(QString file,QHash<int,BookInfo> &results)
{
    file=QDir(file).absolutePath();
    std::ifstream in(file.toStdString());
    std::string linestr;
    BookInfo bookinfo;
    if(in){
        while(std::getline(in,linestr)){
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
            results.insert(bookinfo.ISBN,bookinfo);
        }
    }
    in.close();
}

void MainWindow::saveBooksData()
{
    QString fileName="../database/books.txt";
    QFileInfo fi(fileName);
    if(fi.isFile()){
        QString absName=fi.absoluteFilePath();
        QString path=fi.absolutePath();
        QString baseName=fi.baseName();
        system(QString("cp "+absName+" "+path+"/"+baseName+".backup").toStdString().c_str());
    }
    std::ofstream ofs(fileName.toStdString());
    if(ofs){
        QList<int> keys=books.keys();
        std::priority_queue<int> kpq;
        for(int k:keys){
            kpq.push(k);
        }
        while(kpq.size()>0){
            int k=kpq.top();
            kpq.pop();
            BookInfo bi=books[k];
            ofs<<bi.ISBN<<","<<bi.title.toStdString()<<","<<bi.author.toStdString()<<","<<bi.year<<","
              <<bi.brief.toStdString()<<","<<bi.timeStamp<<","<<bi.frequency<<","<<bi.inCloset<<std::endl;
        }
        ofs.close();
    }
//    std::ofstream f1("books.txt", std::ios::app);
//    if(!f1)return;
//    QHash<int,BookInfo> books;
//    QHash<int,BookInfo> ::const_iterator it;
//    int i = 0;
//    for(it = books.begin();it != books.end();it++,i++){
//        switch (i) {
//        case 0:
//            f1<<std::to_string(it.value().ISBN)<<",";
//            break;
//        case 1:
//            f1<<it.value().title.toStdString()<<",";
//            break;
//        case 2:
//            f1<<it.value().author.toStdString()<<",";
//            break;
//        case 3:
//            f1<<std::to_string(it.value().year)<<",";
//            break;
//        case 4:
//            f1<<it.value().brief.toStdString()<<",";
//            break;
//        case 5:
//            f1<<std::to_string(it.value().timeStamp)<<",";
//            break;
//        case 6:
//            f1<<std::to_string(it.value().frequency)<<",";
//            break;
//        case 7:
//            f1<<std::to_string(it.value().inCloset)<<endl;
//            break;
//        default:
//            break;
//        }

//    }
//    f1.close();
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
//    qDebug()<<"read stuff info test:"<<endl;
//    qDebug()<<"====================="<<endl;
//    for(auto k:stuffs.keys()){
//        qDebug()<<"id:"<<stuffs[k].id<<",name:"<<stuffs[k].name<<",quota:"<<stuffs[k].quota<<",and holds:";
//        const QVector<int> &holds=stuffs[k].current_hold;
//        for(int i=0;i<holds.size();++i)qDebug()<<holds[i]<<",";
//    }
//    //
}

void MainWindow::readBookCoverImages()
{
//    std::vector<std::string> filename;
//    std::vector<std::string> files;
//    char * filePath = "../database/BookImages";
//    getFiles(filePath,filename,files);
//    char str[30];
//    int size = filename.size();
//    std::string	str_temp;
//    for (int i = 0;i < size;i++){
//        int pase = 0;
//        pase = filename[i].find(".");
//        str_temp = filename[i].substr(0,pase);
//        QImage image;
//        QByteArray pData;
//        QString path = QString::fromStdString(files[i]);
//        QFile *file=new QFile(path);
//        file->open(QIODevice::ReadOnly);
//        pData=file->readAll();
//        image.loadFromData(pData);
//        int id = atoi(str_temp.c_str());
//        bookCovers.insert(id,image);
//        file->close();
//        delete file;
//    }
    std::vector<std::string> baseNames,fullNames;
    getFiles("../database/BookImages",baseNames,fullNames);
    for(int i=0;i<baseNames.size();++i){
        int id=stoi(baseNames[i]);
        QPixmap pix;
        pix.load(QString::fromStdString(fullNames[i]));
        bookCovers.insert(id,pix);
    }
    //reading default book cover
    QPixmap img;
    img.load("../icons/default_book.jpg");
    bookCovers[-1]=img;
//    //test
//    qDebug()<<"test read cover images:"<<endl;
//    qDebug()<<"======================="<<endl;
//    qDebug()<<"images count:"<<bookCovers.size()-1<<endl;
//    QList<int> keys=bookCovers.keys();
//    for(int i=0;i<bookCovers.size()-1;++i){
//        qDebug()<<"image#"<<keys[i]<<",size=>"<<bookCovers[keys[i]].width()<<"x"<<bookCovers[keys[i]].height()<<endl;
//    }
//    //
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
        StuffInfo st=stuffs[currentUser];
        ui->nameEdit->setText(st.name);
        ui->idEdit->setText(QString::number(st.id));
        QVector<int> hold_books=st.current_hold;
        QString quota_text=QString::number(hold_books.size())+"/"+QString::number(st.quota);
        if(hold_books.size()==st.quota){
            quota_text+="(额度用尽)";
        }
        ui->quotaEdit->setText(quota_text);
        //arrange books view of user current hold
        if(stuffHoldsView){
            delete stuffHoldsView;
        }
        stuffHoldsView=new QHBoxLayout();
        ui->scrollAreaWidgetContents->setLayout(stuffHoldsView);
        ui->scrollAreaWidgetContents->resize(20,ui->scrollAreaWidgetContents->height());
        int *left=new int();
        int *right=new int();
        int *top=new int();
        int *bot=new int();
        stuffHoldsView->getContentsMargins(left,top,right,bot);
        int book_height=ui->stuffHoldsScroll->height()-*top-*bot;
        int book_width=book_height/1.4;
        delete top,bot,left,right;
        for(int i=0;i<hold_books.size();++i){
            QPixmap pix=bookCovers[hold_books[i]];
            QLabel *label=new QLabel();
            label->setPixmap(pix.scaled(QSize(book_width,book_height)));
            stuffHoldsView->addWidget(label);
            ui->scrollAreaWidgetContents->resize(ui->scrollAreaWidgetContents->width()+label->width(),ui->scrollAreaWidgetContents->height());
        }
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
    std::vector<std::string> opImgs{"main.png", "book_borrow.png",
                                    "book_return.png", "book_share.png"};
    //-1:none;0:borrow;1:return;2:share
    std::string load_path = "../icons/" + opImgs[currentOperation+1];

    QPixmap pix;
    pix.load(QString::fromStdString(load_path));
    int w=this->width()*0.8;
    int h=this->height()*0.7;
    pix=pix.scaled(QSize(w,h));
    ui->operation_image->setAlignment(Qt::AlignCenter);
    ui->operation_image->setPixmap(pix);
    if(currentOperation == 0){
        ui->Finish_Op_button->setText("我已取到书，并且关好柜门");
    } else if(currentOperation == 1) {
        ui->Finish_Op_button->setText("我放回到图书，并且关好柜门");
    } else if(currentOperation == 2){
        ui->Finish_Op_button->setText("确认共享");
    }
}

void MainWindow::toBookInfo() {
    switchToPage(4);
    std::string bookImagePath;
    if(currentBook == -1){
        bookImagePath = "../icons/default_book.jpg";
        ui->BookInfo_again->setText("无法识别");
    } else {
        bookImagePath = "../database/BookImages/"+std::to_string(currentBook);
    }
    QPixmap pix;
    pix.load(QString::fromStdString(bookImagePath));

    int w=this->width()*0.5;
    int h=this->height()*0.4;
    pix=pix.scaled(QSize(w,h));
    ui->operation_image->setAlignment(Qt::AlignCenter);
    ui->operation_image->setPixmap(pix);

    if(currentOperation == 0){
        //借书
        ui->BookInfo_again->setText("再借一本");
    } else if(currentOperation == 1){
        //还书
        ui->BookInfo_again->setText("再还一本");
    } else if(currentOperation == 2) {
        //共享书
        ui->BookInfo_again->setText("继续分享");
    }
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
    currentOperation=0;
    toFaceModule();
//    toBookModule();
}

void MainWindow::on_return_button_clicked()
{
    currentOperation=1;
    toFaceModule();
//    toBookModule();
}

void MainWindow::on_share_button_clicked()
{
    currentOperation=2;
    toFaceModule();
//    toBookModule();
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
//    qDebug()<<"closing..."<<endl;
    if(timer&&timer->isActive()){
        timer->stop();
        delete timer;
        timer=NULL;
    }
    if(recog.isRunning()){
        recog.terminate();
    }
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
    StuffInfo st=stuffs[currentUser];
    if(currentUser!=-1&&st.current_hold.size()<st.quota){
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

void MainWindow::on_Finish_Op_button_clicked()
{
    // Get the ISNB number of the scanned book from the file in ISBN_scan folder
    std::string scan_path("../ISBN_scan/scan_book.txt");
    std::ifstream in_file(scan_path);
    std::string line, isbn;
    std::getline(in_file, line);
    std::stringstream linestream(line);
    std::getline(linestream, isbn, ',');

    try{
        currentBook = std::stoi(isbn);
    }catch(std::invalid_argument ia){
        currentBook = -1;//non-exist book,scan failed
    }
    //-1:none;0:borrow;1:return;2:share
    if(currentBook!=-1 && currentOperation == 2) {
        // 如果为共享书籍，写入books
        QHash<int,BookInfo> tmp;
        scanBooks("../ISBN_scan/scan_book.txt", tmp);
        books.insert(currentBook, tmp[currentBook]);
        top_books.put(tmp[currentBook]);
    } else if(currentBook!=-1 && currentOperation == 0) {
        // 借书
        BookInfo &scan_book = books[currentBook];
        scan_book.inCloset = 0;
    } else if(currentBook!=-1 && currentOperation == 1) {
        // 还书
        BookInfo &scan_book = books[currentBook];
        scan_book.inCloset = 1;
        scan_book.frequency++;

        top_books.put(scan_book);
        displayBooks();
    }
    toBookInfo();
}

void MainWindow::on_BookInfo_confirm_clicked()
{
    updateCountDown(0);
    backToMain();
}

void MainWindow::on_BookInfo_again_clicked()
{
    toBookModule();
}
