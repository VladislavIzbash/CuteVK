#pragma once

#include <QListView>

class QListView;
class KBusyIndicatorWidget;

class DialogListWidget : public QListView
{
    Q_OBJECT

public:
    explicit DialogListWidget(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event);

private:
    KBusyIndicatorWidget* m_fetchmoreSpinner;
    KBusyIndicatorWidget* m_loadingSpinner;


};

