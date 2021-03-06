#ifndef AppManagement_H
#define AppManagement_H

#include <QtGui>
#include "../gui/baseForm.h"

extern "C"{
#include "applist.h"
}

Q_DECLARE_METATYPE(APPINFOLIST)

class AppManagement : public BaseForm
{
    Q_OBJECT
    
public:
    explicit AppManagement(QWidget *parent = 0);
    ~AppManagement();
    
private:
    int COL_CHB;//column checkbox
    int COL_INFO;//column info
    int COL_ID;//column ID
    int APPROLE; //0xfe
    QTableWidget *tableWidget;
    APPINFOLIST appList[50];
    //QList<QStringList> delList;
    QList<APPINFOLIST> delList;
    int appNum;
    void setAppList();//QString filename);
    Button *btnDel;
    Button *btnUp;
    Button *btnDown;
    QMessageBox msgBox;
    QPushButton *btnYes;
    QPushButton *btnCancel;

private slots:
    void slot_up();
    void slot_down();
    void slot_delete();
    void slot_save();

};

#endif // AppManagement_H
