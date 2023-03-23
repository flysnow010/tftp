#include "tftpserverwidget.h"
#include "tftpserver.h"
#include "ui_tftpserverwidget.h"
#include <QFileDialog>
#include <QApplication>
#include <QTime>
#include <QSettings>
#include <QMessageBox>

TFtpServerWidget::TFtpServerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TFtpServerWidget)
    , tftpServer(new TFtpServer(this))
{
    ui->setupUi(this);
    loadSettings();

    connect(ui->btnBrowse, SIGNAL(clicked()), this, SLOT(selectTFtpDir()));
    connect(ui->currentDir, SIGNAL(currentIndexChanged(QString)), this, SLOT(setCurrentDir(QString)));
    connect(tftpServer, SIGNAL(startFile(QString,QString)), this, SLOT(onStartFile(QString,QString)));
    connect(tftpServer, SIGNAL(progress(QString,quint64,quint64)), this, SLOT(onProgress(QString,quint64,quint64)));
    connect(tftpServer, SIGNAL(stopFile(QString)), this, SLOT(onStopFile(QString)));
    connect(tftpServer, SIGNAL(bindError()), this, SLOT(onBindError()));
    tftpServer->start();
}

TFtpServerWidget::~TFtpServerWidget()
{
    saveSettinggs();
    tftpServer->stop();
    delete ui;
}

void TFtpServerWidget::selectTFtpDir()
{
    QString filePath = QFileDialog::getExistingDirectory(this,
                        "Select Dir", ui->currentDir->currentText());
    if(filePath.isEmpty())
        return;
    int index  = ui->currentDir->findText(filePath);
    if(index != -1)
        ui->currentDir->setCurrentIndex(index);
    else
    {
        if(ui->currentDir->count() >= MAX_PATH_SIZE)
            ui->currentDir->removeItem(0);
        ui->currentDir->addItem(filePath);
        ui->currentDir->setCurrentIndex(ui->currentDir->count()  - 1);
    }
}

void TFtpServerWidget::setCurrentDir(QString const& path)
{
    tftpServer->setFilePath(path);
}

void TFtpServerWidget::onStartFile(QString const&transferId, QString const& fileName)
{
    ui->clientTables->addTopLevelItem(new QTreeWidgetItem(QStringList()
                                            << transferId << fileName << QTime::currentTime().toString("hh:mm:ss")));
}

void TFtpServerWidget::onProgress(QString const&transferId, quint64 bytes, quint64 total)
{
    QList<QTreeWidgetItem*> items = ui->clientTables->findItems(transferId, Qt::MatchCaseSensitive);
    for(int i = 0; i < items.size(); i++)
    {
        if(total == 0)
            items[i]->setText(5, QString::number(bytes));
        else
        {   items[i]->setText(3, QString("%1%").arg(bytes * 100 / total));
            items[i]->setText(5, QString::number(total));
        }
        items[i]->setText(4, QString::number(bytes));
    }
}

void TFtpServerWidget::onStopFile(QString const&transferId)
{
    QList<QTreeWidgetItem*> items = ui->clientTables->findItems(transferId, Qt::MatchCaseSensitive);
    for(int i = 0; i < items.size(); i++)
    {
        int index = ui->clientTables->indexOfTopLevelItem(items[i]);
        ui->clientTables->takeTopLevelItem(index);
    }
}

void TFtpServerWidget::saveSettinggs()
{
    QSettings settings(QCoreApplication::applicationName(), QCoreApplication::applicationVersion());
    QStringList dirs;
    for(int i = 0; i < ui->currentDir->count(); i++)
        dirs << ui->currentDir->itemText(i);
    settings.setValue("dirs", dirs);
    settings.setValue("currentDir", ui->currentDir->currentText());
}

void TFtpServerWidget::loadSettings()
{
    QSettings settings(QCoreApplication::applicationName(), QCoreApplication::applicationVersion());
    QStringList dirs = settings.value("dirs", QStringList()).toStringList();
    QString currentDir = settings.value("currentDir", QString()).toString();
    ui->currentDir->addItems(dirs);

    int index  = ui->currentDir->findText(currentDir);
    if(index != -1)
    {
        tftpServer->setFilePath(currentDir);
        ui->currentDir->setCurrentIndex(index);
    }
    else
    {
        tftpServer->setFilePath(QApplication::applicationDirPath());
        ui->currentDir->addItem(QApplication::applicationDirPath());
    }
}

void TFtpServerWidget::onBindError()
{
    QMessageBox::critical(this, "TFtpServer", "Port(69) is already occupied!");
    ui->btnBrowse->setDisabled(true);
    ui->currentDir->setDisabled(true);
    setWindowTitle("TFtpServer is not running");
}

