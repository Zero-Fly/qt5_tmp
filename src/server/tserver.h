#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QRandomGenerator>
#include <QDataStream>
#include <QThread>
#include <QMessageBox>
#include <QRunnable>
#include <QThreadPool>
#include <QNetworkInterface>

class FortuneThread : public QObject, public QRunnable
{
    Q_OBJECT

public:
    FortuneThread(qintptr socketDescriptor, const QString &fortune, QObject *parent = nullptr)
        : socketDescriptor(socketDescriptor), text(fortune) {};

    void run() override;

signals:
    void error(QTcpSocket::SocketError socketError);
    void result(QString res);

private:
    qintptr socketDescriptor;
    QString text;
};


class FortuneServer : public QTcpServer {
    Q_OBJECT

public:
    FortuneServer(QObject *parent = nullptr);
    QStringList getFortuneList();

private slots:

    void incomingConnection(qintptr socketDescriptor) override;
private:
    QStringList fortunes;
    QTcpServer *tcpServer = nullptr;
};

