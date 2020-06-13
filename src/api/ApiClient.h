#pragma once

#include "src/chat/dialoglist/DialogListModel.h"

#include <QObject>
#include <QUrl>
#include <QNetworkRequest>

class QNetworkAccessManager;
class QNetworkReply;

class ApiClient : public QObject
{
    Q_OBJECT

public:
    ApiClient(const QString &token, QObject *parent = nullptr);

    void queryConversationsAsync(int offset, int count);

signals:
   void queryConversationsDone(QVector<DialogEntry> dialogs);

private:
    QNetworkAccessManager* m_networkManager;
    const QString m_token;

    QUrlQuery versionAndToken();

    QNetworkRequest prepareRequest(const QString &method, const QUrlQuery args);

    QJsonValue getResponseValue(QNetworkReply *source);

    DialogEntry queryUserInfoSync(int id);

    QImage downloadPhotoSync(const QString &url);
};

