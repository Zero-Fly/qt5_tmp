#include "tserver.h"
#include<QRandomGenerator>
#include <QDataStream>
#include<iostream>

tServer::tServer(QObject *parent)
: QTcpServer(parent)
{
  fortunes << tr("You've been leading a dog's life. Stay off the furniture.")
               << tr("You've got to think about tomorrow.")
               << tr("You will be surprised by a loud noise.")
               << tr("You will feel hungry again in another hour.")
               << tr("You might have mail.")
               << tr("You cannot kill time without injuring eternity.")
               << tr("Computers are not intelligent. They only think they are.");
}

void tServer::incomingConnection(qintptr socketDescriptor){
  QString fortune = fortunes.at(QRandomGenerator::global()->bounded(fortunes.size()));
  tThread *thread = new tThread(socketDescriptor, fortune, this);
  connect(thread, &tThread::finished, thread, &tThread::deleteLater);
  thread->start();
}

void tServer::say(){
  QString fortune = fortunes.at(QRandomGenerator::global()->bounded(fortunes.size()));
  std::cout<<"Wake the fuck up, Qt5! We have a server to rise." << std::endl;
  std::cout<<fortune.toStdString()<<std::endl;
}

tServer::~tServer(){}

tThread::tThread(int socketDescriptor, const QString &fortune, QObject *parent)
  : QThread(parent), socketDescriptor(socketDescriptor), text(fortune){}

void tThread::run(){
  QTcpSocket tcpSocket;

  if (!tcpSocket.setSocketDescriptor(socketDescriptor)) {
          emit error(tcpSocket.error());
          return;
      }
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_4_0);
  out << text;

  tcpSocket.write(block);
  std::cout<<"try to write"<<std::endl;
  tcpSocket.disconnectFromHost();
  tcpSocket.waitForDisconnected();
}

#include "tserver.moc"
