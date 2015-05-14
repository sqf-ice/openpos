#include "navibar.h"
#include <QDebug>

#define ICON_SIZE 10
#define ICON_PAD 5

NaviBar::NaviBar(const int count)
    : QGraphicsRectItem(),
      naviBarCount(count)
{
    setRect(0, 0, 240, ICON_SIZE);
    setPen(Qt::NoPen);

    x = (this->rect().width() - 20*naviBarCount)/2;

    setAllIndexPage();


    m_cursors1 = new QGraphicsRectItem;
    m_cursors1->setParentItem(this);
   // m_cursors1->setRect(x, 0, 20, 2);//xl
    m_cursors1->setRect(x+2, 2, 8, 8);
    m_cursors1->setPen(Qt::NoPen);
    m_cursors1->setBrush(QColor(0,30,105));//Qt::blue));//xl
    //m_cursors1->setBrush(QColor(Qt::white));
    m_cursors1->setOpacity(0.4);
    m_cursors1->setZValue(2);
}

//NaviBar::~NaviBar()
//{
//    qDeleteAll(m_icons.begin(),m_icons.end());
//    m_icons.clear();
//    qDeleteAll(m_cursor.begin(),m_cursor.end());
//    m_cursor.clear();

//    if(m_cursors1)
//    {
//        delete m_cursors1;
//        m_cursors1 = NULL;
//    }
//}
void NaviBar::setAllIndexPage()
{
    for (int i = 0; i < naviBarCount; ++i) {
        QGraphicsRectItem *m_cursors = new QGraphicsRectItem;
        m_cursors->setParentItem(this);
        m_cursors->setRect(x + (i * 21), -10, 12, 12);//xl
        //m_cursors->setRect(x + (i * 21), -10, 20, 2);
        m_cursors->setPen(Qt::NoPen);
        m_cursors->setBrush(QColor(175,175,175));//xl
        m_cursors->setOpacity(0.2);
        m_cursors->setZValue(1);
      //  m_cursor.append(m_cursors);
    }
}

void NaviBar::setIndexPage(int index)
{
    m_cursors1->setPos(index * 21, -10);
}

void NaviBar::paint(QPainter * painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //painter->setBrush(Qt::white);
    painter->setBrush(QColor(0,50,125));//Qt::blue);
    painter->setOpacity(0.2);
    painter->drawRect(option->rect.adjusted(-5, ICON_PAD, 5, 5)); //xl
    //painter->drawRect(option->rect.adjusted(-10, ICON_PAD, 10, 0));
}


// NaviBar::~NaviBar()
//{
//   // ////qDebug()<<"IN ~NaviBar*************";
//    if(m_cursors1!=NULL)
//    {
//        delete m_cursors1;
//        m_cursors1 = NULL;
//    }
//   // ////qDebug()<<"11111111IN ~NaviBar*************";

//    qDeleteAll(m_icons.begin(),m_icons.end());
//    m_icons.clear();
//   // ////qDebug()<<"2222222222IN ~NaviBar*************";

//    qDeleteAll(m_cursor.begin(),m_cursor.end());
//    m_cursor.clear();
//   // ////qDebug()<<"3333333333IN ~NaviBar*************";

//}
