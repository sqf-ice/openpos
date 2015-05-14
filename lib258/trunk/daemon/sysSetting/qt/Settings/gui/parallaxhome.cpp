#include "parallaxhome.h"
#include <QDebug>

//#include "mainentrythread.h"
#include "wpagui.h"


//MainEntryThread *mainEntryThread;

ParallaxHome::ParallaxHome(int argc, char *argv[], QWidget *parent) :
    QGraphicsView(parent),
    m_pageOffset(-1),
    pageIndex(0)
{
    setFrameShape(QFrame::NoFrame);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_DeleteOnClose);
   // setWindowTitle("Parallax Home");
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    dragFlag = Flag_Slide;
    pItem = NULL;
    pressFlag = false;


    testNames << tr("1.MMI") << tr("2.PRINTER") << tr("3.KEYBOARD") << tr("4.MAGNETIC")
              << tr("5.ICC") << tr("6.SAM") << tr("7.UART") << tr("8.MFC CARD")
              << tr("9.MODEM") << tr("1.GSM") << tr("2.ETHERNET")<< tr("3.WIFI")
              << tr("4.MASS \nMEMORY") << tr("5.BARCODE\n PRINT")<< tr("6.CAMERA")<<tr("7.SIGN IN")
              <<tr("8.LIGHT")<<tr("9.BEEP")<< tr("1.EXIT");


    testIcons << tr("./image/Attic.png") << tr("./image/Categories.png")
              << tr("./image/Currency.png") << tr("./image/Photos.png")
              << tr("./image/fring.png") << tr("./image/Games.png")
              << tr("./image/iFile.png") << tr("./image/MobileRSS.png")
              << tr("./image/IMDb.png") << tr("./image/Phone.png")
              << tr("./image/Notes.png")
              <<  tr("./image/Engadget.png")
              <<tr("./image/PS Mobile.png")<< tr("./image/Remote.png")
             << tr("./image/Videos.png")<<tr("./image/Shazam.png")<< tr("./image/Settings.png")
             << tr("./image/Settings.png")<< tr("./image/Settings.png");
            // << tr("./pic/Shazam.png");//<< tr("./pic/Skype.png")
            // << tr("./pic/Stocks.png");//...


    pageCount = ceil(static_cast<double>(testNames.count())/9); //pageCountÏòÉÏÉáÈëfd
    qDebug() << "pageCount = " << pageCount;

    setDragMode(RubberBandDrag);
    setAcceptDrops(true);
    setupScene(); //load item
    setScene(&m_scene);

    connect(&m_pageAnimator, SIGNAL(frameChanged(int)), SLOT(shiftPage(int)));
    m_pageAnimator.setDuration(500);
    m_pageAnimator.setFrameRange(0, 100);
    m_pageAnimator.setCurveShape(QTimeLine::EaseInCurve);

    choosePage(0);

    qDebug()<<"aaaaaaaaaaaa";
   // qt_main_entry_init(argc, argv);
   // qDebug()<<"bbbbbbbbbb";
//    mainEntryThread = new MainEntryThread(argc,argv,this);
//    connect(mainEntryThread,SIGNAL(sysEventSignal(int)),this,SLOT(sysEventSlot(int)));
//    mainEntryThread->start();
    qDebug()<<"ccccccccc";


//    this->parentWidget()->hide();
}

void ParallaxHome::sysEventSlot(int sysEvent)
{
//    ((struct seven *)sysEvent)->pt_eve_out->treatment_status = NOK;
//    switch ((((struct seven *)sysEvent)->event_nr + ((struct seven *)sysEvent)->event_type))
//    {
//    case OSEVENTTYPE_SEL+0xff:
//    case OSEVENTTYPE_SEL+0xfe:
//        qDebug("qtest show");
//        ((struct seven *)sysEvent)->pt_eve_out->treatment_status = NOK;
//        this->show();
//        break;
//    default:
//        mainEntryThread->wakeUp();
//        break;
//    }
}

void ParallaxHome::slideBy(qreal dx)
{
    m_pageOffset = static_cast<qreal>(pageIndex);
    int start = -(m_pageOffset + dx) * width();
    int end = m_pageOffset * width();
    m_pageAnimator.setFrameRange(start, end);
    m_pageAnimator.start();

    m_naviBar->setIndexPage(-pageIndex);
}

void ParallaxHome::choosePage(int page)
{
    if (m_pageAnimator.state() != QTimeLine::NotRunning)
        return;
    slideBy(-page - m_pageOffset);
}

void ParallaxHome::shiftPage(int frame) //¶¨Î»¸÷¸öitemµÄ×ø±ê
{
    int ww = width();
    int hh = height() - m_naviBar->rect().height();
    m_pageOffset = static_cast<qreal>(frame) / qreal(width()) ;
    int ofs = m_pageOffset * ww;
    for (int i = 0; i < m_items.count(); ++i)
    {
        QPointF pos = m_positions[i];
        QPointF xy(pos.x() * ww, pos.y() * hh);
        m_items[i]->setPos(xy + QPointF(ofs, 0));
    }
}

void ParallaxHome::setupScene()
{
    for (int i = 0; i < pageCount; i++) //Ò³Êý
    {
        int yp = 50;

        for (int j = 0; j < 3; j++) //ÐÐÊý
        {
            int xp = 40;
            for (int k = 0; k < 3; k++) //ÁÐÊý
            {
                if (!m_items.isEmpty() && m_items.count() >= testNames.count())
                    break;

                Node *node = new Node;

                int index = i*9 + j*3 + k;
                node->setText(testNames.at(index));//ÉèÖÃitemµÄÃû³Æ
                QPixmap pix(testIcons.at(index));
                pix.scaled(QSize(59, 50), Qt::IgnoreAspectRatio);
                node->setItemPixmap(pix); //ÉèÖÃitemµÄicon

                m_scene.addItem(node);
                m_items << node;

                qreal qx = static_cast<qreal>(xp)/qreal(240) + static_cast<qreal>(i);
                qreal qy = static_cast<qreal>(yp)/qreal(320-50);

                node->setItemPoint(QPointF(qx, qy));

                m_positions << QPointF(qx, qy);
                node->setZValue(1);
                xp = xp + 80; //ÁÐ¼ä¾à
            }
            yp = yp + 80-2; //ÐÐ¼ä¾à
        }
    }

    m_naviBar = new NaviBar(pageCount);
    m_scene.addItem(m_naviBar);
    m_naviBar->setIndexPage(-pageCount);
    m_naviBar->setZValue(2);

    QPixmap pixmap("./image/surfacing.png");
    //TODO
    pixmap = pixmap.scaled(width()*pageCount, height() + m_naviBar->rect().height() + 50);
    m_scene.addPixmap(pixmap); //ÉèÖÃ³¡¾°±³¾°

    m_scene.setItemIndexMethod(QGraphicsScene::NoIndex);
}

void ParallaxHome::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    layoutScene();
}


void ParallaxHome::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
          event->ignore();

    pressFlag = true;//Êó±êµãÔÚ×ÀÃæÄÚ
    oldPoint = event->pos();//È¡µÃ°´ÏÂÊ±µÄ×ø±ê

    QList<QGraphicsItem *> pitems = QGraphicsView::items(event->pos());
    isHasItem = pitems.count();

    if (isHasItem == 2) //background and item, select one item.
    {
        pItem = (Node*)QGraphicsView::itemAt(event->pos());
        if (!pItem->isNull())
        {
            qDebug()<<"is ining ***********";
            pItem->setBackgroundColor(QColor(159, 159, 159));
            dragFlag = Flag_Start;//????
        }
    }
    else //only background, no item is selected.
    {
        dragFlag = Flag_Slide; //????
        qDebug()<<"not  ining ***********";

    }

    event->accept();
}

void ParallaxHome::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
          event->ignore();

    curPoint = event->pos();
    int dValue = curPoint.x() - oldPoint.x();
    if (qAbs(dValue) >= 8)
        dragFlag = Flag_Slide;
    else
        dragFlag = Flag_Start;


    if (pressFlag)
    {
        if (dragFlag == Flag_Start)  //????
        {
            if (pItem == NULL)
                return;
            qDebug()<<"aaaaaaa ***********";

            pressFlag = false;
            pItem->setBackgroundColor(Qt::transparent);
            QString name = pItem->text();
            testByItemName(name);
            pItem = NULL;
        }
        else if (dragFlag == Flag_Slide) //????
        {
            if (pItem != NULL)
            {
                pItem->setBackgroundColor(Qt::transparent);
                pItem = NULL;
            }
           // qDebug()<<"bbbbbbbbbb ***********";


            //»¬¶¯¾àÀë³¬¹ý´°¿Ú¿í¶ÈµÄÒ»°ë²Å»á»»Ò³
            if((dValue >= width()/6) && (pageIndex < 0))
                pageIndex++;
            else if((dValue <= -width()/6) && (pageIndex > -pageCount+1))
                pageIndex--;

            choosePage(pageIndex);
        }
        event->accept();
    }
}

void ParallaxHome::mouseMoveEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
          event->ignore();

    if (pressFlag)
    {
        qDebug()<<"mouse move *******";
        int value = event->pos().x() - oldPoint.x();
       // qDebug()<<"curpos   oldpox  value  width()/6"<<event->pos().x()<<oldPoint.x()<<value<<width()/6;
        if (qAbs(value) >= 8)
            dragFlag = Flag_Slide;
//        else
//            dragFlag = Flag_Start;

       // qDebug()<<"move dragFlag=========="<<dragFlag;
        shiftPage(value + pageIndex * width());
        event->accept();
    }
}

void ParallaxHome::layoutScene()
{
    m_scene.setSceneRect(0, 0, pageCount * width() - 1, height() - 1);//ÉèÖÃ³¡¾°µÄ´óÐ¡
    centerOn(/*width() / 2, height() / 2*/0, 0); //¶¨Î»³¡¾°µÄ×ø±êÔ­µã

    int nw = m_naviBar->rect().width();
    int nh = m_naviBar->rect().height();
    m_naviBar->setPos((width() - nw) / 2, height() - nh);

    shiftPage(m_pageOffset * width());
}

void ParallaxHome::testByItemName(const QString &itemName) //´ò¿ª²âÊÔ½çÃæ
{
    qDebug() << Q_FUNC_INFO << "  itemName = " << itemName;
    if (itemName == testNames.at(0))
    {
        qDebug() << itemName;
        WpaGui *wpa = new WpaGui(qApp);
        wpa->show();

    }
    else if (itemName == testNames.at(1))
    {
        qDebug() << itemName;


    }
    else if (itemName == testNames.at(2))
    {

        qDebug() << itemName;
    }
    else if (itemName == testNames.at(3))
    {
        qDebug() << itemName;

    }
    else if (itemName == testNames.at(4))
    {

        qDebug() << itemName;
    }
    else if (itemName == testNames.at(5))
    {
        qDebug() << itemName;

    }
    else if (itemName == testNames.at(6))//uart OR SERIAL
    {


    }
    else if (itemName == testNames.at(7))//MFC CARD
    {
        qDebug() << itemName;


    }
    else if (itemName == testNames.at(8))
    {
        qDebug() << itemName;

    }
    else if (itemName == testNames.at(9))//gsm
    {
        qDebug() << itemName;

    }
    else if (itemName == testNames.at(10))
    {
        qDebug() << itemName;

    }
    else if (itemName == testNames.at(11))
    {
        qDebug() << itemName;


//        WpaGui *wpa = new WpaGui(qApp);
//        wpa->show();

    }
    else if (itemName == testNames.at(12))
    {
        qDebug() << itemName;

    }
    else if (itemName == testNames.at(13))
    {
        qDebug() << itemName;

    }
    else if (itemName == testNames.at(14))
    {
        qDebug() << itemName;

    }
    else if (itemName == testNames.at(15))
    {
        qDebug() << itemName;


    }

    else if (itemName == testNames.at(16))
    {

    }


    else if (itemName == testNames.at(17))
    {

    }
    else if (itemName == testNames.at(18))
    {
       // qDebug() << itemName;
        //mainEntryThread->wakeUp();
        this->hide();//close();

    }

}

ParallaxHome::~ParallaxHome()
{
   // qDebug()<<"IN ~ParallaxHome()";
    if(pItem!=NULL)
    {
        delete pItem;
        pItem = NULL;
    }

    if( m_naviBar!=NULL)
    {
        delete m_naviBar;
        m_naviBar = NULL;
    }

    qDeleteAll(m_items.begin(),m_items.end());
    m_items.clear();

//    for(QList<Node*>::iterator
//        i = m_items.begin();i<m_items.end();i++)
//    {
//        if(*i!=NULL)
//        {
//            delete *i;
//            *i = NULL;
//        }
//    }
   // qDebug()<<"22222IN ~ParallaxHome()";

//    while(!m_items.isEmpty())
//        delete m_items.takeFirst();

   // m_items.clear();
   // qDebug()<<"333333333IN ~ParallaxHome()";


}


void ParallaxHome::keyPressEvent(QKeyEvent *event)
{
   // qDebug()<<"pageINDEX"<<pageIndex;
    switch(event->key())
    {
      case Qt::Key_1:
        if(pageIndex==0)
        {

        }
        else if(pageIndex==-1)
        {

        }
        else if(pageIndex==-2)
        {
//            mainEntryThread->wakeUp();
//            this->hide();//close();
        }
        break;
    case Qt::Key_2:
        if(pageIndex==0)
        {

        }
        else if(pageIndex==-1)
        {

        }
       break;
    case Qt::Key_3:
        if(pageIndex==0)
        {

        }
        else if(pageIndex==-1)
        {

        }

       break;
    case Qt::Key_4:
        if(pageIndex==0)
        {

        }
        else if(pageIndex==-1)
        {

        }

       break;
    case Qt::Key_5:
        if(pageIndex==0)
        {


        }
        else if(pageIndex==-1)
        {

        }

       break;
    case Qt::Key_6:
        if(pageIndex==0)
        {

        }
        else if(pageIndex==-1)
        {

        }

       break;
    case Qt::Key_7:
        if(pageIndex==0)
        {


        }
        else if(pageIndex==-1)
        {

        }

       break;
    case Qt::Key_8:
        if(pageIndex==0)
        {
        }
        else if(pageIndex==-1)
        {
         }

       break;
    case Qt::Key_9:
        if(pageIndex==0)
        {

        }
        else if(pageIndex==-1)
        {

        }

       break;

    case Qt::Key_Escape:
    case Qt::Key_Backspace:
        {
           // qDebug()<<"escape********";
            //mainEntryThread->wakeUp();
            this->hide();//close();
        }
        break;
    }

}
