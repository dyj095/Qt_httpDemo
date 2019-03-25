#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHttpMultiPart>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QFileInfo>
#include <QEventLoop>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //QString fileName("E:/迅雷下载/ubuntukylin-16.04-desktop-amd64.iso");
    QString fileName("C:/Users/duyanjun/Pictures/mypic.jpg");
    file = new QFile(fileName);


    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);    

    QHttpPart enctypePart;
    enctypePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QString("form-data; name=\"%1\"").arg("enctype")));
    enctypePart.setBody(QString("multipart/form-data").toLatin1());
    multiPart->append(enctypePart);

    QHttpPart typePart;
    typePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QString("form-data; name=\"%1\"").arg("type")));
    typePart.setBody(QString::number(1).toLatin1());
    multiPart->append(typePart);

    QHttpPart md5Part;
    md5Part.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QString("form-data; name=\"%1\"").arg("md5")));
    md5Part.setBody(QString("123456").toLatin1());
    multiPart->append(md5Part);

    QHttpPart fileNamePart;
    fileNamePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QString("form-data; name=\"%1\"").arg("fileName")));
    fileNamePart.setBody(QString(file->fileName()).toLatin1());
    multiPart->append(fileNamePart);

    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QString("form-data; name=\"%1\"; filename=\"%2\"").arg("file").arg(file->fileName())));
    file->open(QIODevice::ReadOnly);
    filePart.setBodyDevice(file);
    file->setParent(multiPart);
    // we cannot delete the file now, so delete it with the multiPart
    multiPart->append(filePart);

    QUrl url("http://192.168.2.59:9090/fileservice/fileupload.do");
    QNetworkRequest request(url);
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    reply = manager->post(request, multiPart);
    multiPart->setParent(reply);
    connect(reply,SIGNAL(finished()), this, SLOT(httpUploadFinished()));
    connect(reply, SIGNAL(uploadProgress(qint64,qint64)), this, SLOT(uploadProgress(qint64,qint64)));
    QEventLoop eventLoop;
    connect(manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    eventLoop.exec();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::httpUploadFinished()
{
    if(reply->error() == QNetworkReply::NoError) {
        qDebug()<<"upload file finished";
        file->flush();
        file->close();
        reply->deleteLater();
        reply = NULL;
        delete file;
        file = NULL;
    } else {
        qDebug()<<"error string:"<<reply->errorString();
        reply->deleteLater();
    }
}

void MainWindow::uploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    if (bytesTotal > 0) {
        qint64 persent = (bytesSent* 100 /bytesTotal) ;
        qDebug() << "bytesSent:" << bytesSent << ",bytesTotal:" << bytesTotal << ",persent:" << persent << "%";
    }
}
