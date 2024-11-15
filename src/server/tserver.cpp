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
#include <QNetworkInterface>
#include "tserver.h"


void FortuneThread::run() {
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
  //std::cout<<"end sending"<<std::endl;
  emit result("end sending");
}



FortuneServer::FortuneServer(QObject *parent) : QTcpServer(parent) {

    fortunes << tr("You've been leading a dog's life. Stay off the furniture.")
             << tr("You've got to think about tomorrow.")
             << tr("You will be surprised by a loud noise.")
             << tr("You will feel hungry again in another hour.")
             << tr("You might have mail.")
             << tr("You cannot kill time without injuring eternity.")
             << tr("Computers are not intelligent. They only think they are.");


    //connect(this, &QTcpServer::newConnection, this, &EchoServer::onNewConnection);
}
QStringList FortuneServer::getFortuneList(){return fortunes;}

void FortuneServer::incomingConnection(qintptr socketDescriptor) {
        QString fortune = fortunes.at(QRandomGenerator::global()->bounded(fortunes.size()));
        //std::cout<<"Create new tread\n";
        FortuneThread *handler = new FortuneThread(socketDescriptor, fortune, this);
        QThreadPool::globalInstance()->start(handler); // Start the handler in the thread pool
}


