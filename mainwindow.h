#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QVector>
#include <QHash>
#include <QImage>
#include <QTimer>
#include <climits>
#include <QThread>

struct BookInfo{
    int ISBN;//use as book id
    QString title;
    QString author;
    int year;
    QString brief;
    uint timeStamp;
    int frequency;
    int inCloset;//书籍是否在库,1:in,0:not in
    void setTimeStamp(QDateTime time){
        timeStamp=time.toTime_t();
    }
    QDateTime getDateTime(){
        return QDateTime::fromTime_t(timeStamp);
    }
    friend  bool operator<(const BookInfo &a,const BookInfo &b) {
        return  a.frequency<b.frequency || (a.frequency==b.frequency && a.timeStamp<b.timeStamp);
    }
};

struct StuffInfo{
    int id;
    QString name;
    int quota;
    QVector<int> current_hold;
};

template<typename T>
struct Identical{
    //identical if having the same address
    bool operator ()(const T &a,const T &b){return &a==&b;}
};

template<>
struct Identical<BookInfo>{
    bool operator ()(const BookInfo &a,const BookInfo &b){return a.ISBN==b.ISBN;}
};

template<typename T,typename _Comp=std::less<T>,typename _Identical=Identical<T> >
class Cache{
private:
    QVector<T> data;
    int cap;
    bool _allow_duplicate;
    _Comp compare;
    _Identical identical;
public:
    Cache(){
        cap=INT_MAX;
        _allow_duplicate=false;
    }
    Cache(int capacity,int allow_duplicate=false){
        cap=capacity;
        _allow_duplicate=allow_duplicate;
    }
    void put(const T &target){
        if(!_allow_duplicate){
            remove(target);
        }
        int pos=data.size()-1;
        for(;pos>=0;){
            if(compare(data[pos],target)){//if data[pos]<target
                --pos;
            }
            else{
                break;
            }
        }
        //data[pos]>=target>data[pos+1]
        data.insert(data.begin()+pos+1,target);
        while (data.size()>cap) {
            data.pop_back();
        }
    }
    bool remove(const T &target){
        for(int i=0;i<data.size();++i){
            if(identical(target,data[i])){
                data.remove(i);
                return true;
            }
        }
        return false;
    }
    void removeAt(int i){
        if(i>=0&&i<data.size()){
            data.remove(i);
        }
    }
    T at(int i){return data[i];}
    int size(){return data.size();}
};

class Recognizing:public QThread{
    Q_OBJECT

signals:
    //id==-2:multi face;id==-1:unknown face;id>=0:id of stuff
    void recognized(int id);
    // QThread interface
protected:
    void run();
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void initPages();
    void initProperties();
    void startupTasks();
    //初始化轮播图片
    void initCarousel();
    //扫描书籍信息（扫描书库所有书籍或者获取扫描仪扫描的书籍信息）
    void scanBooks(QString file);
    //保存书籍信息
    void saveBooksData();
    //读取员工信息
    void readStuffInfo();
    //读取书籍封面信息
    void readBookCoverImages();
    void switchToPage(int page);
    //跳转到人脸模块
    void toFaceModule();
    void toStuffInfo();
    //跳转到书籍模块
    void toBookModule();
    void toBookInfo();
    //返回主页
    void backToMain();

    void displayBooks();
    ~MainWindow();

private slots:
    void on_borrow_button_clicked();

    void on_return_button_clicked();

    void on_share_button_clicked();

    void on_faceReturn_clicked();

    void on_id_confirm_clicked();

    void on_id_cancel_clicked();

private:
    Ui::MainWindow *ui;
    QHash<int,BookInfo> books;
    Cache<BookInfo> top_books;
    QHash<int,StuffInfo> stuffs;
    QHash<int,QVector<QImage> > faces;
    QHash<int,QImage> bookCovers;
    QTimer *timer;
    int timeout;//time duration after which return to main page(in seconds)
    int count_down;
    int currentOperation;//-1:none;0:borrow;1:return;2:share
    int currentUser;
    int currentBook;
    Recognizing recog;
    //count down
    void updateCountDown(int time);

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
