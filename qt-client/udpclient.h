#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QObject>
#include <QUdpSocket> // 必须包含这个头文件

class UdpClient : public QObject // 建议类名首字母大写
{
    Q_OBJECT
public:
    explicit UdpClient(QObject *parent = nullptr);

    // 注意这里把 message 作为参数传进来了
    Q_INVOKABLE void sendMessage(const QString &message, const QString &host, quint16 port);

    Q_INVOKABLE void bindSocket(quint16 port);

signals:
    void messageReceived(const QString &message);
    void msgLogin(const QString &message);
    void msgDeliver(const QString &message);
    void msgPickup(const QString &message);
    void msgRegister(const QString &message);

private slots:
    void processPendingDatagrams();

private:
    QUdpSocket *udpSocket;
};

#endif // UDPCLIENT_H  (只留这一个 endif)
