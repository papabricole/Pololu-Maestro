#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QtGui>
#include "ui_MainWindow.h"
#include <libusc.h>

class QxChannelStatus;

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT
  public:
    MainWindow(QWidget * parent = 0, Qt::WindowFlags flags = 0);

    virtual ~MainWindow();

  private slots:
    void open();
    void saveAs();
    void about();

  private:
     void createActions();
     void createMenus();
     void createToolBars();

     QMenu *fileMenu;
     QMenu *helpMenu;
     QAction *openAct;
     QAction *saveAsAct;
     QAction *exitAct;
     QAction *aboutAct;

     libusc_device* device;
     libusc_device **device_list;

     QVector<QxChannelStatus*> m_channelStatus;
};

#endif
