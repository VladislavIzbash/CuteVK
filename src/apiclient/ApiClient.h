#pragma once

#include <QObject>

class ApiClient : public QObject
{
    Q_OBJECT

public:
    explicit ApiClient(QObject *parent = nullptr);

    void authorize(const QString &login, const QString &password);

signals:
    void authorized(QString token);

};

