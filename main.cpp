#include "mymainwindow.h"
#include "ui_mymainwindow.h"
#include <QApplication>
#include <QT>
#include <Qstring>
#include <windows.h>
#include <QChar>
#include <QTime>
#include <QMessageBox>
#include <QFile>
#include<QTextStream>
#include<QTextCodec>
bool clear=true;
HHOOK hHook=NULL;
bool process=false,pause=false;
QString str="";
//using namespace std;
//long double *mas_size;
//long double *mas_time;
int t=0;
int tmp=0;
QVector <uint >vector_time,vector_time2;
QVector < uint> global_income;
LRESULT CALLBACK MyLowLevelKeyBoardProc(int nCode,WPARAM wParam,
LPARAM lParam )
{
    if(process && !pause){
        if(nCode == HC_ACTION && (wParam==WM_KEYDOWN)){

    KBDLLHOOKSTRUCT cKey=*((KBDLLHOOKSTRUCT*)lParam);
    wchar_t buffer[5];
    BYTE keyboard_state[256];
    GetKeyboardState(keyboard_state);
  // keyboard_state[VK_SHIFT]=GetKeyState(VK_SHIFT);
    keyboard_state[VK_CAPITAL]=GetKeyState(VK_CAPITAL);
    keyboard_state[VK_CONTROL]=GetKeyState(VK_CONTROL);
    keyboard_state[VK_MENU]=GetKeyState(VK_MENU);
   keyboard_state[VK_MENU]=GetKeyState(VK_TAB);
    HKL keyboard_layout = GetKeyboardLayout(0);
    clear=false;
    char lpszName[0x100]={0};
    DWORD dwMsg=1;
    dwMsg+=cKey.scanCode<<16;
    dwMsg+=cKey.flags<<24;

    GetKeyNameText(dwMsg,(LPTSTR)lpszName,255);
    ToUnicodeEx(cKey.vkCode,cKey.scanCode,
                           keyboard_state,buffer,4,0,keyboard_layout);
    buffer[4]=L'\0';
    QTime timee = QTime::currentTime();
   uint noww = QDateTime::currentDateTime().toTime_t();
   vector_time.push_front(noww);
   QFile file("D://testtt.txt");
    if (file.open( QIODevice::WriteOnly| QIODevice::Text| QIODevice::Append ))
    {
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
         //str=cKey.vkCode+QString::fromUtf16((ushort*)buffer)+QString::fromUtf16((ushort*)lpszName);
        stream <<timee.toString("hh:mm:ss")<<" "<< "key: "<<cKey.vkCode<<" \
"<<QString::fromUtf16((ushort*)buffer)<<" "<<QString::fromUtf16((ushort*)lpszName)<<endl;
          // k++;
         file.close();
    }
    else
    {QMessageBox Msgbox1;
        Msgbox1.setText("No file");
        Msgbox1.exec();}


        }}
      return CallNextHookEx(hHook,nCode,wParam,lParam);
}

int main(int argc, char *argv[])
{
    QString FAppPath = QCoreApplication::applicationDirPath() + "/";
        QSettings settings(FAppPath + "file.ini", QSettings::IniFormat);
        settings.setIniCodec("UTF-8");

        QString test = settings.value("Server/test").toString();
        QTextCodec *codec = QTextCodec::codecForName("cp866");
        QTextCodec::setCodecForLocale(codec);
       QApplication a(argc, argv);
    MyMainWindow w;
    w.show();

    w.ui->textBrowser->setText("Ваш звіт буде відображатись тут");
         hHook=SetWindowsHookEx(WH_KEYBOARD_LL,MyLowLevelKeyBoardProc,NULL,0);
        if(hHook==NULL)
        {
          QMessageBox Msgbox;
          Msgbox.setText("error");
          Msgbox.exec();
            }





    return a.exec();
}


