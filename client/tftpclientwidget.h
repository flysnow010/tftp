#ifndef TFTPCLIENTWIDGET_H
#define TFTPCLIENTWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class TFtpClientWidget; }
QT_END_NAMESPACE

class TFtpClient;
class TFtpClientWidget : public QWidget
{
    Q_OBJECT

public:
    TFtpClientWidget(QWidget *parent = nullptr);
    ~TFtpClientWidget();

private slots:
    void onGetFile();
    void onPutFile();
    void onStarted();
    void onProgress(quint64 bytes, quint64 total);
    void onStop();
    void onFinished();
    void onError(QString const& error);
    void onSelectLocalFile();
private:
    void enableButtons(bool enable);
private:
    Ui::TFtpClientWidget *ui;
    TFtpClient* tftpClient;
};
#endif // TFTPCLIENTWIDGET_H
