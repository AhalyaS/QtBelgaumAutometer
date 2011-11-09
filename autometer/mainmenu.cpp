#include "mainmenu.h"

MainMenu::MainMenu(QWidget *parent) :
    QWidget(parent)
{

    frame = new QFrame(this);

    QPixmap back_pix("C:\\QtSDK\\autometer\\Images\\back.png");
    back_pix.scaled(QSize(QApplication::desktop()->width(), QApplication::desktop()->height()), Qt::IgnoreAspectRatio);

    this->setStyleSheet("QPushButton {color: black; background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #008000, stop: 0.15 #008b00, stop: 0.5 #00cd00, stop: 0.85 #008b00, stop: 1 #008000); border-width: 1px; border-color: #339; border-style: solid; border-radius: 7; padding: 3px; font-size: 30px; padding-left: 5px; padding-right: 5px; min-width: 150px; min-height: 50px;}");
    m_help = new QPushButton("Help", this);
    m_about = new QPushButton("About", this);
    m_quit = new QPushButton("Quit", this);
    m_start = new QPushButton("Start", this);

    QVBoxLayout *layout = new QVBoxLayout();

    frame->setMinimumSize(QApplication::desktop()->width(), QApplication::desktop()->height()-50);
    frame->setLayout(layout);
    frame->setAutoFillBackground(true);
    QPalette p = palette();
    p.setBrush(QPalette::Background, back_pix);
    frame->setPalette(p);

    help = new QMessageBox(this);
    help->setText("Autometer is an application that calculates the auto fare for a distance travelled.");
    about = new QMessageBox(this);
    about->setText("Developer: StudentDeveloper \nVersion: 1.0");

    layout->addWidget(m_help);
    layout->addWidget(m_quit);
    layout->addWidget(m_about);
    layout->addWidget(m_start);

    layout->setAlignment(m_help, Qt::AlignCenter);
    layout->setAlignment(m_quit, Qt::AlignCenter);
    layout->setAlignment(m_about, Qt::AlignCenter);
    layout->setAlignment(m_start, Qt::AlignCenter);

    connect(m_help, SIGNAL(clicked()), help, SLOT(exec()));
    connect(m_quit, SIGNAL(clicked()), this, SLOT(openQuitPage()));
    connect(m_about, SIGNAL(clicked()), about, SLOT(exec()));
    connect(m_start, SIGNAL(clicked()), this, SLOT(openStartPage()));

    frame->show();
}


void MainMenu::openQuitPage()
{
   QApplication::quit();
}


void MainMenu::openStartPage()
{
    StackedWidgets *s = new StackedWidgets();
}

