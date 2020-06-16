#include "ApiClient.h"

#include <QUrlQuery>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QImageReader>
#include <QVector>
#include <QThread>

const QString BASE_URL = "https://api.vk.com/method/%1?%2";

ApiClient::ApiClient(QObject* parent)
    : QObject(parent)
    , m_networkManager(new QNetworkAccessManager(this))
{}

QString ApiClient::getToken() const
{
    return m_token;
}

void ApiClient::setToken(const QString &token)
{
    m_token = token;
}

QUrlQuery ApiClient::versionAndToken()
{
    QUrlQuery query;

    query.addQueryItem("v", "5.60");
    query.addQueryItem("access_token", m_token);

    return query;
}

QNetworkRequest ApiClient::prepareRequest(const QString& method, const QUrlQuery& args)
{
    QString url = QString("https://api.vk.com/method/%1?%2&%3")
            .arg(method)
            .arg(versionAndToken().query())
            .arg(args.query());

    return QNetworkRequest(url);
}

void ApiClient::queryConversationsAsync(int offset, int count)
{
    QUrlQuery args;
    args.addQueryItem("offset", QString::number(offset));
    args.addQueryItem("count", QString::number(count));
    args.addQueryItem("filter", "all");
    args.addQueryItem("extended", "1");
    args.addQueryItem("fields", "photo_50,online");

    QNetworkReply* reply = m_networkManager->get(prepareRequest("messages.getConversations", args));
    connect(reply, &QNetworkReply::finished, [=] () {
        QJsonObject root = QJsonDocument::fromJson(reply->readAll()).object();
        if (root.contains("error")) {
            emit apiError(root.value("error").toObject().value("error_msg").toString());
            return;
        }

        QJsonObject response = root.value("response").toObject();
        QJsonArray conversations = response.value("items").toArray();
        QJsonArray profiles = response.value("profiles").toArray();

        QVector<Conversation> dialogs;
        for (const QJsonValue& item : qAsConst(conversations)) {
            QJsonObject conversation = item.toObject().value("conversation").toObject();
            QJsonObject peer = conversation.value("peer").toObject();
            int id = peer.value("id").toInt();

            if (peer.value("type").toString() == "user") {
                dialogs.append(getUserById(id, profiles));

            } else {
                QJsonObject chatSettings = conversation.value("chat_settings").toObject();
                QString title = chatSettings.value("title").toString();

                QImage photo = downloadPhotoSync(chatSettings.value("photo").toObject().value("photo_50").toString());

                dialogs.append({id, title, photo, true, true});
            }
        }

        emit queryConversationsDone(dialogs, response.value("count").toInt());
    });
}

Conversation ApiClient::getUserById(int id, const QJsonArray& profiles)
{
    for (const QJsonValue& profileVal : qAsConst(profiles)) {
        QJsonObject profile = profileVal.toObject();

        if (profile.value("id").toInt() == id) {
            QString name = profile.value("first_name").toString() + " " + profile.value("last_name").toString();
            bool online = profile.value("online").toInt();
            QImage photo = downloadPhotoSync(profile.value("photo_50").toString());

            return {id, name, photo, false, online};
        }
    }

    return {};
}

QImage ApiClient::downloadPhotoSync(const QString& url)
{
    QNetworkReply *reply = m_networkManager->get(QNetworkRequest(url));
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    return QImageReader(reply).read();
}



