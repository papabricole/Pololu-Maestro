#include "MainWindow.h"
#include "QxChannelStatus.h"
#include "ConfigurationFile.h"

#include <QtGui/QtGui>



MainWindow::MainWindow(QWidget * parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags),
    device(NULL)
{
    setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setUnifiedTitleAndToolBarOnMac(true);

    libusb_init(NULL);

    createActions();
    createMenus();
    createToolBars();

    /////////
    /*
    QFile file("toto.script.lst");
    if (file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QByteArray bytecode = file.readAll();

        libusc_write_script(device, bytecode.constData(), bytecode.size());
    }
*/

    ////////
    int numChannels = device ? libusc_get_num_channels(device) : 0;

    statusTableWidget->setAlternatingRowColors(true);
    statusTableWidget->setRowCount(numChannels);
    statusTableWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    statusTableWidget->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);

    for (int i = 0; i < numChannels; ++i)
    {
        m_channelStatus.append(new QxChannelStatus(device, i, statusTableWidget));
    }

    resize(800, 400);
}

MainWindow::~MainWindow()
{
    libusc_free_device_list(device_list, 1);
    libusb_exit(NULL);
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
    {
        QFile input(fileName);
        if (input.open(QIODevice::ReadOnly))
        {
            ConfigurationFile::load(device, &input);
            input.close();
        }
    }
}

void MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty())
        return;

    QFile output(fileName);
    if (output.open(QIODevice::WriteOnly))
    {
        ConfigurationFile::save(device, &output);
        output.close();
    }
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Recent Files"),
                       tr("The <b>Recent Files</b> example demonstrates how to provide a "
                          "recently used file menu in a Qt application."));
}

void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

void MainWindow::createToolBars()
{
    QToolBar* toolbar = addToolBar(tr("Toolbar"));

    QComboBox * deviceList = new QComboBox(toolbar);

    ssize_t cnt = libusc_get_device_list(NULL, &device_list);
    for (int i=0; i<cnt; i++)
    {
        libusc_device_settings settings;
        libusc_get_device_settings(device_list[i], &settings);
        deviceList->addItem("#" + QString::number(settings.serialDeviceNumber), qVariantFromValue((void*)device_list[i]));

        device = device_list[0];
    }

    toolbar->addWidget(deviceList);
}
