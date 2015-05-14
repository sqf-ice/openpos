#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

struct gif_loading_struct
{
    int x;
    int y;
    int w;
    int h;
    int stop;
    char name[128];
    int tx;
    int ty;
    int tw;
    int th;
    char text[512];
    char style[128];
    int tposition;
};

#define MAXDATASIZE sizeof(struct gif_loading_struct)+1 /*每次最大数据传输量 */

class SocketServerThread : public QThread
{
    Q_OBJECT

protected:
    void run();

signals:
    void gifLoadingSign(struct gif_loading_struct *gif_content);

private:
    char g_gif_content[MAXDATASIZE];
};

#endif