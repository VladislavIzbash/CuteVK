#pragma once

#include <QWidget>

class QListView;
class KBusyIndicatorWidget;

class DialogListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DialogListWidget(QWidget *parent = nullptr);

private slots:
    void hideSpinner();
    void showSpinner();

private:
    QListView *m_dialogList;
    KBusyIndicatorWidget *m_loadingSpinner;
};

