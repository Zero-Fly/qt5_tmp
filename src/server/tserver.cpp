#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QRandomGenerator>
#include <QDataStream>
#include <QThread>
#include <iostream>
#include <QMessageBox>

class FortuneThread : public QThread
{
    Q_OBJECT

public:
    FortuneThread(qintptr socketDescriptor, const QString &fortune, QObject *parent = nullptr)
          : QThread(parent), socketDescriptor(socketDescriptor), text(fortune) {}

    void run() override{
      QTcpSocket tcpSocket;
      if (!tcpSocket.setSocketDescriptor(socketDescriptor)) {
          emit error(tcpSocket.error());
          return;
      }

      QByteArray block;
      QDataStream out(&block, QIODevice::WriteOnly);
      out << text;

      connect(&tcpSocket, &QAbstractSocket::disconnected, &tcpSocket, &QObject::deleteLater);

      tcpSocket.write(block);
      tcpSocket.disconnectFromHost();
      tcpSocket.waitForDisconnected();
    }


signals:
    void error(QTcpSocket::SocketError socketError);

private:
    int socketDescriptor;
    QString text;
};


class EchoServer : public QTcpServer {
    Q_OBJECT

public:
    EchoServer(QObject *parent = nullptr) : QTcpServer(parent) {
        tcpServer = new QTcpServer(this);
          if (!tcpServer->listen()) {
              std::cout<<"Server can't run"<< std::endl;
              return;
          }

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
    void onNewConnection() {
        QTcpSocket *clientSocket = nextPendingConnection();
        if(clientSocket){
            //incomingConnection(clientSocket);
          }
    }
    void incomingConnection(qintptr socketDescriptor) override {
            QString fortune = fortunes.at(QRandomGenerator::global()->bounded(fortunes.size()));
            FortuneThread *thread = new FortuneThread(socketDescriptor, fortune, this);
            connect(thread, &FortuneThread::finished, thread, &FortuneThread::deleteLater);
            thread->start();
   }
private:
    QStringList fortunes;
    QTcpServer *tcpServer = nullptr;
};



int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    EchoServer server;

    if (!server.listen(QHostAddress::Any, 4242)) {
        std::cout << "Server could not start!" << std::endl;
        return 1;
    }

    std::cout << "Server started on port" << server.serverPort() << std::endl;
    return a.exec();
}

#include "tserver.moc"
