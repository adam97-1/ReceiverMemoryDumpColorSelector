#ifndef MEMORYDUMP_H
#define MEMORYDUMP_H

#include <QSerialPort>
#include <cstdint>
#include <QFile>

class MemoryDump : public QObject
{
#pragma pack(push, n)
    typedef struct
    {
        uint32_t header;
        uint32_t indexMain;
        int32_t indexStack;
        uint32_t stackPointer;
        uint32_t usedMem;
        uint32_t crc;
    } MemoryDumpReceive;
#pragma pack(pop)

    enum class ReciveState
    {
        Header1,
        header2,
        header3,
        header4,
        FillData
    };


public:
    MemoryDump(int argc, char *argv[]);

public slots:
    void onReadyRead();

private:
    QSerialPort m_serialPort;
    QVector<uint8_t> m_dataReceiveRaw;
    ReciveState m_reciveState {ReciveState::Header1};
    uint32_t m_oldMainIndex {0};

};

#endif // MEMORYDUMP_H
