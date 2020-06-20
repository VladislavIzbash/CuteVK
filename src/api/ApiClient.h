#pragma once

#include <QObject>
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
    explicit ApiClient(QObject* parent = nullptr);

    QString getToken() const;
    void setToken(const QString &token);

public slots:
    void getConversations(int offset, int count);

    void getConversationStatus();

signals:
    void getConversationsDone(const QVector<Conversation>& conversations, int total);

    void getConversationStatusDone(const QMap<int, bool>& statusMap);

    void apiError(const QString& message);

private:
    QNetworkAccessManager* m_networkManager;
    QString m_token;

    QUrlQuery versionAndToken();

    QNetworkRequest prepareRequest(const QString& method, const QUrlQuery& args);

    Conversation getUserById(int id, const QJsonArray& profiles);

    QImage downloadPhotoSync(const QString& url);
};

