#include <QCoreApplication>
#include "MemoryDump.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    MemoryDump app(argc, argv);

    return a.exec();
}
