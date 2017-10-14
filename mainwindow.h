#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <capture.h>
#include <QThread>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Capture*            cap;
    QThread             capThread;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

signals:
    void startLoop();


private slots:
    void on_start_clicked();
    void on_stop_clicked();
    void on_save_clicked();
    void getImage(QImage img);
};

#endif // MAINWINDOW_H
