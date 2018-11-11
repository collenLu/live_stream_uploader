#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void closeEvent(QCloseEvent *event);

public slots:
    void stream();

private:
    Ui::Widget *ui;
    bool isStream = false;
};

#endif // WIDGET_H
