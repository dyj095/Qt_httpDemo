# Qt_httpDemo
## 一、简介：<br>
基于QT的QNetworkAccessManager用java服务器（struts1）上传文件<br>

重点：<br>
> ```c++
> QHttpPart filePart;
// 重点是第2个参数filename不能少，否则服务器FormFile对象为null
> filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QString("form-data; name=\"%1\";filename=\"%2\"").arg("file").arg(file->fileName())));
> ```
