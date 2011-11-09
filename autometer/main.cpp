#include <QApplication>
#include "doodle.h"

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);

    Doodle d;
    d.show();

    return app.exec();
}
