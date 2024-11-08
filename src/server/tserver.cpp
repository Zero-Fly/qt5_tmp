#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QRandomGenerator>
#include <QDataStream>
#include <QThread>
#include <iostream>
#include <QMessageBox>
#include <QRunnable>
#include <QThreadPool>
#include <memory>
#include <QNetworkInterface>

class FortuneThread : public QObject, public QRunnable
{
    Q_OBJECT

public:
    FortuneThread(qintptr socketDescriptor, const QString &fortune, QObject *parent = 0)
          : socketDescriptor(socketDescriptor), text(fortune) {}

    void run() override{
      QTcpSocket socket;
      socket.setSocketDescriptor(socketDescriptor);

      QByteArray block;
      QDataStream out(&block, QIODevice::WriteOnly);
      out << text;
      socket.write(block);
      socket.flush();
      socket.disconnectFromHost();
      socket.waitForDisconnected();
      socket.deleteLater();
      std::cout<<"end sending"<<std::endl;
      emit result("end sending");
    }

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
    FortuneServer(QObject *parent = nullptr) : QTcpServer(parent) {

        fortunes << tr("You've been leading a dog's life. Stay off the furniture.")
                 << tr("You've got to think about tomorrow.")
                 << tr("You will be surprised by a loud noise.")
                 << tr("You will feel hungry again in another hour.")
                 << tr("You might have mail.")
                 << tr("You cannot kill time without injuring eternity.")
                 << tr("Computers are not intelligent. They only think they are.");


        //connect(this, &QTcpServer::newConnection, this, &EchoServer::onNewConnection);
    }

private slots:

    void incomingConnection(qintptr socketDescriptor) override {
            QString fortune = fortunes.at(QRandomGenerator::global()->bounded(fortunes.size()));
            std::cout<<"Create new tread\n";
            FortuneThread *handler = new FortuneThread(socketDescriptor, fortune, this);
            QThreadPool::globalInstance()->start(handler); // Start the handler in the thread pool
   }
private:
    QStringList fortunes;
    QTcpServer *tcpServer = nullptr;
};



int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    FortuneServer server;
    if (!server.listen()) {
          std::cout<<"Server can't run"<< std::endl;
    }
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
       // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
       if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
          ipAddressesList.at(i).toIPv4Address()) {
          ipAddress = ipAddressesList.at(i).toString();
          break;
       }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
       ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
       std::cout<<"The server is running on\n\nIP: "<< ipAddress.toStdString() << "\nport: "<< server.serverPort() <<"\n\n";


    return a.exec();
}

#include "tserver.moc"
