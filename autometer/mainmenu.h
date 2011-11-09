#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QtGui>
#include <QtCore>
#include <QFrame>
#include <QMessageBox>

#include "stackedwidgets.h"

class MainMenu : public QWidget
{
    Q_OBJECT
public:
    explicit MainMenu(QWidget *parent = 0);
    QPushButton *m_help, *m_about, *m_start, *m_quit;
    QFrame *frame;
    QMessageBox *help, *about, *quit;

signals:

public slots:
    void openQuitPage();
    void openStartPage();
};

#endif // MAINMENU_H
