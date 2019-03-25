#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkReply>
#include <QFile>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void httpUploadFinished();
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);
private:
    Ui::MainWindow *ui;
    QNetworkReply *reply;
    QFile *file;
};

#endif // MAINWINDOW_H
