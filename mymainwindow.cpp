#include "mymainwindow.h"
#include "ui_mymainwindow.h"
#include "about.h"
#include <QKeyEvent>
#include <QTGui>
#include <QTime>
#include <QVector>
#include <QInputEvent>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <QKeySequence>
#include <QFileDialog>
#include <windows.h>
#include <QChar>
#include<QTextCodec>
#include<QProgressBar>
#include <QDesktopServices>
#include <QDir>
extern int t;
extern bool process,pause;
extern bool clear;
extern HHOOK hHook;
extern QString str;
extern int tmp;
//extern double *mas_size,*mas_time;
//extern QVector < double> global_income;
extern QVector < uint> vector_time,vector_time2;
MyMainWindow::MyMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyMainWindow)
{
    ui->setupUi(this);
  //график
    customPlot = new QCustomPlot();
    ui->gridLayout_3->addWidget(customPlot,0,0,1,1);  // Устанавливаем customPlot в окно проложения
    customPlot->setInteraction(QCP::iRangeZoom,true);   // Включаем взаимодействие удаления/приближения
     customPlot->setInteraction(QCP::iRangeDrag, true);  // Включаем взаимодействие перетаскивания графика
     customPlot->axisRect()->setRangeDrag(Qt::Horizontal);   // Включаем перетаскивание только по горизонтальной оси
     customPlot->axisRect()->setRangeZoom(Qt::Horizontal);   // Включаем удаление/приближение только по горизонтальной оси
     customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);   // Подпись координат по Оси X в качестве Даты и Времени
     customPlot->xAxis->setDateTimeFormat("hh:mm:ss");  // Устанавливаем формат даты и времени

     // Настраиваем шрифт по осям координат
     customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 6));
     customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 7));

     // Автоматическое масштабирование тиков по Оси X
     customPlot->xAxis->setAutoTickStep(true);

     /* Делаем видимыми оси X и Y по верхней и правой границам графика,
      * но отключаем на них тики и подписи координат
      * */
     customPlot->xAxis2->setVisible(false);
     customPlot->yAxis2->setVisible(false);
     customPlot->xAxis2->setTicks(false);
     customPlot->yAxis2->setTicks(false);
     customPlot->xAxis2->setTickLabels(false);
     customPlot->yAxis2->setTickLabels(false);

     customPlot->yAxis->setTickLabelColor(QColor(Qt::red)); // Красный цвет подписей тиков по Оси Y
     //customPlot->legend->setVisible(true);   //Включаем Легенду графика
     // Устанавливаем Легенду в левый верхний угол графика
     //customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);

     // Инициализируем график и привязываем его к Осям
     graphic = new QCPGraph(customPlot->xAxis, customPlot->yAxis);
     customPlot->addPlottable(graphic);  // Устанавливаем график на полотно
     //graphic->setName("Доход, Р");       // Устанавливаем
     graphic->setPen(QPen(QColor(Qt::red))); // Устанавливаем цвет графика
     graphic->setAntialiased(false);         // Отключаем сглаживание, по умолчанию включено
     graphic->setLineStyle(QCPGraph::lsStepLeft); // График в виде импульсных тиков lsImpulse

     /* Подключаем сигнал от Оси X об изменении видимого диапазона координат
      * к СЛОТу для переустановки формата времени оси.
      * */
     //connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)),
       //     this, SLOT(slotRangeChanged(QCPRange)));
    // connect(this, SIGNAL(signalifmySizeChanged(int)),
          //   this, SLOT(slotifmySizeChanged(int)));

}
MyMainWindow::~MyMainWindow()
{
    QFile("D://testtt.txt").remove();
    delete ui;
}

void MyMainWindow::Keyfunc( )
{
   QString texto="KeyPressed!";
   ui->textBrowser->setText(texto);
   }


void MyMainWindow::on_exit_triggered()
{
 close();
}

void MyMainWindow::on_open_triggered()
{

 QString filename=QFileDialog::getOpenFileName(
             this,
             tr("Open File"),
             "D://Mykursova//",
             "Text File (*.txt);;All files(*.*)"

             );
        QFile file(filename);
      if (!file.open(QIODevice::Text | QIODevice::ReadOnly))
       QMessageBox::information(0,"info","Error"+file.errorString()); // пытаемся открыть файл
       QTextStream in(&file);
        in.setCodec("UTF-8");
      int intfirst= in.readLine().toInt(); // читаем первую строку
      QVector <uint>vector_time_new(intfirst);
      QVector < uint> time(vector_time_new.size()), income(vector_time_new.size());
      for (int i=0,p=intfirst;i<intfirst;i++,p--)
      {
        in>>vector_time_new[i]; // переводим их в double и записываем в вектор
        time[i] =vector_time_new[i];
      income[i] = p;

      }
       QString strr,tmpp;
       in.setCodec("UTF-8");
        for (int i = intfirst; i < 2*intfirst; i++)
       strr+=in.read(i);
           ui->textBrowser->setText( strr);

        graphic->setData(time, income); // Устанавливаем данные
        customPlot->rescaleAxes();      // Масштабируем график по данным
       customPlot->replot();           // Отрисовываем график         // Отрисовываем график
}

void MyMainWindow::on_clear_clicked()
{
   ui->textBrowser->clear();
   ui->statusBar->showMessage("Очищено");
   QVector<uint>my_null;

     vector_time.clear();

   QFile("D://testtt.txt").remove();
   QFile file("D://testtt.txt");
   file.open(QIODevice::Text | QIODevice::WriteOnly);
   QTextStream in(&file);
   in<<"";
   clear=true;
   file.close();
    graphic->setData(vector_time,my_null); // Устанавливаем данные
   customPlot->rescaleAxes();      // Масштабируем график по данным
   customPlot->replot();
}

void MyMainWindow::on_save_triggered()
{
    QString filename=QFileDialog::getSaveFileName(
                this,
                tr("Save File"),
                "D://Mykursova//",
                "Text File (*.txt);;All Files"

                );
    if (!filename.isEmpty()){
         QFile file(filename);

        if(!file.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append ))
        QMessageBox::warning(this,"TextBrowser","Не можу записати файл"+filename);

        QTextStream in(&file);
         in.setCodec("UTF-8");
        in<<vector_time.size()<<endl;
               for(int i=0;i<vector_time.size();i++)
                 {

                  in<<vector_time[i]<<endl;
                }

                QTime timee = QTime::currentTime();
                in<<timee.toString("hh:mm:ss")<<endl;
        in<<ui->textBrowser->toPlainText().toUtf8();

          file.close();
    }
}

void MyMainWindow::on_save_2_clicked()
{
    MyMainWindow::on_save_triggered();
}

void MyMainWindow::on_about_triggered()
{
  About *wnd = new About(this);
  wnd->show();

}

void MyMainWindow::on_start_triggered()
{
    on_clear_clicked();
         process=true;
         clear=false;
           ui->statusBar->showMessage("Запис розпочато...");


}

void MyMainWindow::on_stop_triggered()
{
     ui->statusBar->showMessage("Запис завершено.");
     process=false;
     pause=false;
     MyMainWindow::on_clear_2_clicked();
     if (clear!=true)
          ui->statusBar->showMessage("Звіт сформовано");

}
void MyMainWindow::slotRangeChanged(const QCPRange &newRange)
{
   customPlot->xAxis->setDateTimeFormat((newRange.size() <=86400)? "hh:mm:ss" : "dd MMM yy");
}
void MyMainWindow::on_clear_2_clicked()//показати звіт
{

    QProgressBar *progressBar = new QProgressBar();
    progressBar->setAlignment(Qt::AlignRight);
       progressBar->setMaximumSize(300, 19);
       ui->statusBar->addPermanentWidget(progressBar);
      // ui->statusBar->addWidget(progressBar);
              progressBar->show();
    progressBar->setValue(0);
    ui->statusBar->showMessage("Звіт формується...");
    for(int i = 1; i <= 10; i++)
            {
                progressBar->setValue(i*10);
                QThread::msleep(100);
            } progressBar->setValue(0);
    //
    QFile file("D://testtt.txt");
   if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
   {QMessageBox::information(0,"info","Error"+file.errorString());
    ui->statusBar->showMessage("Помилка,неможливо відтворити сформований звіт");}
    //
   QTextStream in(&file);
   in.setCodec("UTF-8");
    ui->textBrowser->setText(in.readAll());
    //отображаем график
 //customPlot->addPlottable(graphic);  // Устанавливаем график на полотно

        // Объявляем вектора времени и
        QVector < uint> time(vector_time.size()), income(vector_time.size());
       // vector_time2.reserve(vector_time.size());

        // Заполняем график значениями

        for (int i=0,p=vector_time.size(); i<vector_time.size(); ++i,p--)
           {
                 //vector_time2.push_front(vector_time[i]);
                 time[i] = vector_time[i];
               // in<<QString::number(time[i])<<endl;
                                    income[i] = p;


             }
        if (clear==true)
           { vector_time.clear();
             ui->statusBar->showMessage("Звіт не сформовано. Жодної клавіші не натиснуто.");}
        else
          ui->statusBar->showMessage("Ви переглядаєте щойно сформований звіт.");

         progressBar->hide();
        graphic->setData(time, income); // Устанавливаем данные
        customPlot->rescaleAxes();      // Масштабируем график по данным
        customPlot->replot();           // Отрисовываем график         // Отрисовываем график

}




void MyMainWindow::on_new_2_triggered()
{
    MyMainWindow::on_clear_clicked();
    ui->statusBar->showMessage("Почніть новий запис.");
}

void MyMainWindow::on_pause_triggered()
{
  if (pause)
     { pause=false;
      ui->statusBar->showMessage("Запис продовжується");
  }
  else if(!pause)
  {pause=true;
   ui->statusBar->showMessage("Пауза");}
}

void MyMainWindow::on_action_triggered()
{
    QProgressBar *progressBar = new QProgressBar();
    progressBar->setAlignment(Qt::AlignRight);
       progressBar->setMaximumSize(300, 19);
       ui->statusBar->addPermanentWidget(progressBar);
      // ui->statusBar->addWidget(progressBar);
              progressBar->show();
    progressBar->setValue(0);
    ui->statusBar->showMessage("Звіт формується...");
    for(int i = 1; i <= 10; i++)
            {
                progressBar->setValue(i*10);
                QThread::msleep(100);
            } progressBar->setValue(0);
    QFile file("D://testtt.txt");
   if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
   {QMessageBox::information(0,"info","Error"+file.errorString());
    ui->statusBar->showMessage("Помилка,неможливо відтворити сформований звіт");}
    //
   QTextStream in(&file);
   in.setCodec("UTF-8");
    ui->textBrowser->setText(in.readAll());
    QVector < uint> time(vector_time.size()), income(vector_time.size());

        for (int i=0,p=vector_time.size(); i<vector_time.size(); ++i,p--)
           {
                   time[i] = vector_time[i];
                  income[i] = p;
             }
        if (clear==true)
           { vector_time.clear();
             ui->statusBar->showMessage("Звіт не сформовано. Жодної клавіші не натиснуто.");}
        else
          ui->statusBar->showMessage("Ви переглядаєте щойно сформований звіт.");

         progressBar->hide();
        graphic->setData(time, income); // Устанавливаем данные
        customPlot->rescaleAxes();      // Масштабируем график по данным
        customPlot->replot();           // Отрисовываем график
}

void MyMainWindow::on_help_triggered()
{
    //QDesktopServices::openUrl(QUrl::fromLocalFile("SmartReport.pdf"));
    QString myPdfPath = QApplication::applicationDirPath() + "/SmartReport.pdf";
    QDesktopServices::openUrl(QUrl(myPdfPath));
}
