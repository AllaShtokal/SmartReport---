#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H

#include <QMainWindow>
#include <windows.h>
#include "qcustomplot.h"

namespace Ui {
class MyMainWindow;
}



class MyMainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MyMainWindow(QWidget *parent = 0);
   // static bool process;
void Keyfunc();

//void signalifmySizeChanged(int sizze);

    ~MyMainWindow();
protected:
 //void keyPressEvent(QKeyEvent*e);
public slots:
 //void slotifmySizeChanged(int sizze);
private slots:
    void on_exit_triggered();
    void on_open_triggered();
    void on_clear_clicked();
    void on_save_triggered();

    void on_save_2_clicked();

    void on_about_triggered();

    void on_start_triggered();

    void on_stop_triggered();

    void on_clear_2_clicked();
    void slotRangeChanged (const QCPRange &newRange);


    void on_new_2_triggered();

    void on_pause_triggered();

    void on_action_triggered();

    void on_help_triggered();

private:

public:
Ui::MyMainWindow *ui;
QCustomPlot *customPlot;    // Объявляем графическое полотно
   QCPGraph *graphic;          // Объявляем график


};

#endif // MYMAINWINDOW_H
