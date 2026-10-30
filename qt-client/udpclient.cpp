#include "udpclient.h"
#include <QNetworkDatagram>

// 注意：这里的类名要统一成 UdpClient
UdpClient::UdpClient(QObject *parent)
    : QObject{parent}
{
    udpSocket = new QUdpSocket(this); // 拼写修正：QUdpSocket

    // 注意：这里的类名要统一
    connect(udpSocket, &QUdpSocket::readyRead, this, &UdpClient::processPendingDatagrams);
}

// 拼写修正：QString, quint16
void UdpClient::sendMessage(const QString &message, const QString &host, quint16 port){
    QByteArray data = message.toUtf8(); // 拼写修正：QByteArray
    udpSocket->writeDatagram(data, QHostAddress(host), port);
}

// 拼写修正：bindSocket, QHostAddress
void UdpClient::bindSocket(quint16 port){
    udpSocket->bind(QHostAddress::Any, port);
}

void UdpClient::processPendingDatagrams(){
    while(udpSocket->hasPendingDatagrams()){
        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        QByteArray data = datagram.data();
        switch(data[0]){
        case 'l':
            emit msgLogin(&data[1]);
            break;
        case 'r':
            emit msgRegister(&data[1]);
            break;
        case 'd':
            emit msgDeliver(&data[1]);
            break;
        case 'p':
            emit msgPickup(&data[1]);
            break;
        default:
            emit messageReceived(QString(datagram.data()));
        }
    }
}
