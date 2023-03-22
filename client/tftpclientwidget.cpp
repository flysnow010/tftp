#include "tftpclientwidget.h"
#include "tftpclient.h"
#include "ui_tftpclientwidget.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QRandomGenerator>

TFtpClientWidget::TFtpClientWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TFtpClientWidget)
    , tftpClient(new TFtpClient(this))
{
    ui->setupUi(this);
    connect(ui->btnBrowse, SIGNAL(clicked()), this, SLOT(onSelectLocalFile()));
    connect(ui->btnGet, SIGNAL(clicked()), this, SLOT(onGetFile()));
    connect(ui->btnPut, SIGNAL(clicked()), this, SLOT(onPutFile()));
    connect(ui->btnBreak, SIGNAL(clicked()), this, SLOT(onStop()));
    connect(tftpClient, &TFtpClient::started, this, &TFtpClientWidget::onStarted);
    connect(tftpClient, &TFtpClient::progress, this, &TFtpClientWidget::onProgress);
    connect(tftpClient, &TFtpClient::finished, this, &TFtpClientWidget::onFinished);
    connect(tftpClient, &TFtpClient::error, this, &TFtpClientWidget::onError);
}

TFtpClientWidget::~TFtpClientWidget()
{
    delete ui;
}

void TFtpClientWidget::onGetFile()
{
    tftpClient->stopFileTransfer();
    tftpClient->setHostPort(ui->lineEditHost->text(), ui->spinBoxPort->value());
    tftpClient->getFile(ui->lineEditLocalFile->text(), ui->lineEditRemoteFile->text());
    ui->progressBar->setValue(0);
}

void TFtpClientWidget::onPutFile()
{
    tftpClient->stopFileTransfer();
    tftpClient->setHostPort(ui->lineEditHost->text(), ui->spinBoxPort->value());
    tftpClient->putFile(ui->lineEditLocalFile->text(), ui->lineEditRemoteFile->text());
    ui->progressBar->setValue(0);
}

void TFtpClientWidget::onStarted()
{
    enableButtons(false);
}

void TFtpClientWidget::onProgress(quint64 bytes, quint64 total)
{
    if(total > 0)
        ui->progressBar->setValue(bytes * 100 / total);
    else
    {
        int value = ui->progressBar->value();
        ui->progressBar->setValue(QRandomGenerator(value).bounded(value, 99));
    }
}

void TFtpClientWidget::onStop()
{
    enableButtons(true);
    tftpClient->stopFileTransfer();
}

void TFtpClientWidget::onFinished()
{
    if(tftpClient->isPut())
        QMessageBox::information(this, "TFtpClient", "Put is done!");
    else
        QMessageBox::information(this, "TFtpClient", "Get is done!");
    enableButtons(true);
}

void TFtpClientWidget::onError(QString const& error)
{
    QMessageBox::critical(this, "TFtpClient", error);
    enableButtons(true);
}

void TFtpClientWidget::onSelectLocalFile()
{
    static QString filePath;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Select File"),
                               filePath,
                               tr("All files (*.*)"));
    if(fileName.isEmpty())
        return;
    filePath = QFileInfo(fileName).filePath();
    ui->lineEditLocalFile->setText(fileName);
}

void TFtpClientWidget::enableButtons(bool enable)
{
    ui->btnGet->setEnabled(enable);
    ui->btnPut->setEnabled(enable);
    ui->btnBreak->setDisabled(enable);
}
