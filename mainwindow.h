#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void initCarousel();
    ~MainWindow();

private slots:
    void on_borrow_button_clicked();

    void on_return_button_clicked();

    void on_share_button_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
