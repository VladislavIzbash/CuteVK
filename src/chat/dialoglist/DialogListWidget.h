#pragma once

#include <QWidget>

class QListView;
class KBusyIndicatorWidget;
class QStateMachine;
class DialogListModel;

class DialogListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DialogListWidget(QWidget* parent = nullptr);

    DialogListModel* model();

private:  
    QStateMachine* m_stateMachine;
    QListView* m_dialogList;
    KBusyIndicatorWidget* m_loadingSpinner;

    void printError(const QString& message);
};

