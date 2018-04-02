#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QFile>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QObject>
#include <QUrl>

class Downloader : public QObject {
    Q_OBJECT
    QFile *m_file;
    bool m_isReady;

public:
    explicit Downloader(QObject *parent = 0) : QObject(parent) {m_isReady = true;}
    virtual ~Downloader() {}

    void downloadFileFromURL(const QString &url, const QString &filePath);

private slots:
    void onDownloadFileComplete(QNetworkReply *reply);
};

QNetworkReply::NetworkError downloadURL(const QString &url, const QString &fileName);

#endif // DOWNLOADER_H
