#pragma once

#include <QObject>
#include <QUrl>
#include <QNetworkRequest>
#include <QImage>

class QNetworkAccessManager;
class QNetworkReply;

struct Conversation {
    int id;
    QString title;
    QImage photo;
    bool isChat;
    bool online;
};

Q_DECLARE_METATYPE(Conversation)

class ApiClient : public QObject
{
    Q_OBJECT

public:
    ApiClient(QObject* parent = nullptr);

    QString getToken() const;
    void setToken(const QString &token);

    void queryConversationsAsync(int offset, int count);

signals:
    void queryConversationsDone(const QVector<Conversation>& conversations, int total);

    void apiError(const QString& message);

private:
    QNetworkAccessManager* m_networkManager;
    QString m_token;

    QUrlQuery versionAndToken();

    QNetworkRequest prepareRequest(const QString& method, const QUrlQuery& args);

    Conversation getUserById(int id, const QJsonArray& profiles);

    QImage downloadPhotoSync(const QString& url);
};

