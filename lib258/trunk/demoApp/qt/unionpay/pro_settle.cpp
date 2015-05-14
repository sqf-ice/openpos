#include "pro_settle.h"
#include "ui_pro_settle.h"
#include "msginfo.h"
#include "mythread.h"
#include "iconv.h"

#include <include.h>
#include <global.h>
#include <xdata.h>


Pro_Settle::Pro_Settle(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Pro_Settle)
{	
    ui->setupUi(this);

    sandtimer = new QTimer();
    connect(sandtimer,SIGNAL(timeout()),this,SLOT(slottimer1GetCardInfo()));
    sandtimer->start( GUI_GETCARD_TIMES );
    ProUiFace.uiTimes=60;
    ucTransFlag = 0;
    g_ProgressBarFlag = 1;

    GetSettleInfo();
}
int Pro_Settle::GetSettleInfo(void)
{
    char uBuf[1024];
    // 读取交易统计信息
    SERV_DispTotal();
    // show static infomation to user
    if(ProUiFace.ProToUi.uiLines){
        if(strlen((char *)ProUiFace.ProToUi.aucLine1)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine1,strlen((char *)ProUiFace.ProToUi.aucLine1),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("1. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine2)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine2,strlen((char *)ProUiFace.ProToUi.aucLine2),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("2. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine3)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine3,strlen((char *)ProUiFace.ProToUi.aucLine3),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("3. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine4)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine4,strlen((char *)ProUiFace.ProToUi.aucLine4),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("4. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine5)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine5,strlen((char *)ProUiFace.ProToUi.aucLine5),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("5. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine6)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine6,strlen((char *)ProUiFace.ProToUi.aucLine6),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("6. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine7)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine7,strlen((char *)ProUiFace.ProToUi.aucLine7),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("7. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine8)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine8,strlen((char *)ProUiFace.ProToUi.aucLine8),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("8. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine9)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine1,strlen((char *)ProUiFace.ProToUi.aucLine9),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("1. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine10)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine1,strlen((char *)ProUiFace.ProToUi.aucLine10),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("1. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine11)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine1,strlen((char *)ProUiFace.ProToUi.aucLine11),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("1. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine12)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine2,strlen((char *)ProUiFace.ProToUi.aucLine12),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("2. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine13)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine3,strlen((char *)ProUiFace.ProToUi.aucLine13),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("3. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine14)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine4,strlen((char *)ProUiFace.ProToUi.aucLine14),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("4. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine15)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine5,strlen((char *)ProUiFace.ProToUi.aucLine15),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("5. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine16)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine6,strlen((char *)ProUiFace.ProToUi.aucLine16),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("6. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine17)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine7,strlen((char *)ProUiFace.ProToUi.aucLine17),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("7. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine18)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine8,strlen((char *)ProUiFace.ProToUi.aucLine18),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("8. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine19)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine1,strlen((char *)ProUiFace.ProToUi.aucLine19),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("1. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine20)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine1,strlen((char *)ProUiFace.ProToUi.aucLine20),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("1. aucLine1:%s",uBuf);
        }

        if(strlen((char *)ProUiFace.ProToUi.aucLine21)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine1,strlen((char *)ProUiFace.ProToUi.aucLine21),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("1. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine22)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine2,strlen((char *)ProUiFace.ProToUi.aucLine22),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("2. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine23)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine3,strlen((char *)ProUiFace.ProToUi.aucLine23),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("3. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine24)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine4,strlen((char *)ProUiFace.ProToUi.aucLine24),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("4. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine25)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine5,strlen((char *)ProUiFace.ProToUi.aucLine25),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("5. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine26)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine6,strlen((char *)ProUiFace.ProToUi.aucLine26),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("6. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine27)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine7,strlen((char *)ProUiFace.ProToUi.aucLine27),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("7. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine28)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine8,strlen((char *)ProUiFace.ProToUi.aucLine28),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("8. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine29)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine1,strlen((char *)ProUiFace.ProToUi.aucLine29),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("1. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine30)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine1,strlen((char *)ProUiFace.ProToUi.aucLine30),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("1. aucLine1:%s",uBuf);
        }


        if(strlen((char *)ProUiFace.ProToUi.aucLine31)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine1,strlen((char *)ProUiFace.ProToUi.aucLine31),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("1. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine32)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine2,strlen((char *)ProUiFace.ProToUi.aucLine32),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("2. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine33)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine3,strlen((char *)ProUiFace.ProToUi.aucLine33),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("3. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine34)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine4,strlen((char *)ProUiFace.ProToUi.aucLine34),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("4. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine35)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine5,strlen((char *)ProUiFace.ProToUi.aucLine35),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("5. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine36)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine6,strlen((char *)ProUiFace.ProToUi.aucLine36),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("6. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine37)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine7,strlen((char *)ProUiFace.ProToUi.aucLine37),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("7. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine38)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine8,strlen((char *)ProUiFace.ProToUi.aucLine38),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("8. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine39)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine1,strlen((char *)ProUiFace.ProToUi.aucLine39),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("1. aucLine1:%s",uBuf);
        }
        if(strlen((char *)ProUiFace.ProToUi.aucLine40)){
            memset(uBuf,0,sizeof(uBuf));
            gb2312toutf8((char *)ProUiFace.ProToUi.aucLine1,strlen((char *)ProUiFace.ProToUi.aucLine40),uBuf,sizeof(uBuf));
            ui->line_Static->append(tr(uBuf));
            qDebug("1. aucLine1:%s",uBuf);
        }

        memset((uchar*)&ProUiFace.ProToUi,0,sizeof(ProUiFace.ProToUi));
    }
    return 0;
}
Pro_Settle::~Pro_Settle()
{
    delete ui;
}
int Pro_Settle::gb2312toutf8(char *sourcebuf, size_t sourcelen, char *destbuf, size_t destlen)
{
    iconv_t cd;
    if( (cd = iconv_open("utf-8","gb2312")) ==0 )
        return -1;
    memset(destbuf,0,destlen);
    char **source = (char**)&sourcebuf;
    char **dest   = (char**)&destbuf;

    if(-1 == iconv(cd,source,&sourcelen,dest,&destlen))
        return -1;
    iconv_close(cd);
    return 0;

}

void Pro_Settle::slottimer1GetCardInfo()
{
    //时钟显示
    unsigned char str[6];
    memset(str,0,sizeof(str));
    str[0]='[';
    int_str(&str[1],2, &ProUiFace.uiTimes);
    str[3]=']';


    ProUiFace.uiTimes--;

    ui->label_times->setText(( char*)&str);
    if(ProUiFace.uiTimes == 0){
        sandtimer->stop();
        destroy();
    }
    qDebug("0. Pro_Settle uiTimes:%d.",ProUiFace.uiTimes);


}

void Pro_Settle::destroyWindow(int)
{

    //    a->terminate();
    memset((uchar*)&ProUiFace.OweDrv,0x00,sizeof(ProUiFace.OweDrv));
    memset((uchar*)&ProUiFace.ProToUi,0x00,sizeof(ProUiFace.ProToUi));
    memset((uchar*)&ProUiFace.UiToPro,0x00,sizeof(ProUiFace.UiToPro));
    destroy();
}

void Pro_Settle::on_pushButton_clicked()
{
    sandtimer->stop();
    destroy();
}

void Pro_Settle::on_pushButton_2_clicked()
{
//    if(ucTransFlag == 0){
//        GetSettleInfo();
//        ucTransFlag = 1;
//    }else{
        sandtimer->stop();
        ProUiFace.UiToPro.ucPrintDetailFlag = 0x01;
        ProUiFace.UiToPro.ucTransType= TRANS_SETTLE;
        MsgInfo *desk=new MsgInfo();
        connect(desk,SIGNAL(returnGrandFartherMenu(int)),this,SLOT(destroyWindow(int)));
        desk->show();
        desk->showFullScreen();
        // 启动线程 处理交易
//        MyThread *a=new MyThread();
//        a->start();
        // trans type=settle
//    }
}
}
