#include "MemoryDump.h"
#include "Crc.h"
#include <QFileInfo>

MemoryDump::MemoryDump(int argc, char *argv[])
{
    if(argc < 2)
        m_serialPort.setPortName("/dev/ttyACM0");
    else
        m_serialPort.setPortName(argv[1]);


    QObject::connect(&m_serialPort, &QSerialPort::readyRead, this, &MemoryDump::onReadyRead);
    m_serialPort.setBaudRate(QSerialPort::Baud115200);
    m_serialPort.setDataBits(QSerialPort::Data8);
    m_serialPort.setParity(QSerialPort::NoParity);
    m_serialPort.setStopBits(QSerialPort::OneStop);
    m_serialPort.setFlowControl(QSerialPort::NoFlowControl);

    m_serialPort.open(QIODevice::ReadWrite);
}

void MemoryDump::onReadyRead()
{
    auto dataVect = m_serialPort.readAll();

    foreach (auto data, dataVect)
    {
        switch (m_reciveState)
        {
        case ReciveState::Header1:
            m_dataReceiveRaw.clear();
            if(data == '!')
            {
                m_dataReceiveRaw.push_back(data);
                m_reciveState = ReciveState::header2;
            }
            break;
        case ReciveState::header2:
            if(data == '!')
            {
                m_dataReceiveRaw.push_back(data);
                m_reciveState = ReciveState::header3;
            }
            break;
        case ReciveState::header3:
            if(data == '!')
            {
                m_dataReceiveRaw.push_back(data);
                m_reciveState = ReciveState::header4;
            }
            break;
        case ReciveState::header4:
            if(data == '!')
            {
                m_dataReceiveRaw.push_back(data);
                m_reciveState = ReciveState::FillData;
            }
            break;
        case ReciveState::FillData:
            m_dataReceiveRaw.push_back(data);
            if(m_dataReceiveRaw.count() >= static_cast<qsizetype>(sizeof(MemoryDumpReceive)))
            {
                MemoryDumpReceive *memoryDump = reinterpret_cast<MemoryDumpReceive*>(m_dataReceiveRaw.data());
                if(Crc32_Calc(m_dataReceiveRaw.data(), m_dataReceiveRaw.count() - 4, 0) == memoryDump->crc)
                {
                    QFile file(QString("Main%1.csv").arg(memoryDump->indexMain));
                    file.open(QFile::OpenModeFlag::Append);
                    QString ileOut("");
                    ileOut += std::to_string(memoryDump->indexMain);
                    ileOut += ";";
                    ileOut += std::to_string(memoryDump->indexStack);
                    ileOut += ";";
                    ileOut += std::to_string(0x20020000 - memoryDump->stackPointer);
                    ileOut += ";";
                    ileOut += std::to_string(memoryDump->usedMem);
                    ileOut += "\n";

                    file.write(ileOut.toUtf8());
                    file.close();
                }
                m_reciveState = ReciveState::Header1;
            }
            break;
        }
    }

}
