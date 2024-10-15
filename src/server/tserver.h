#ifndef TSERVER_H
#define TSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>

class tServer : QTcpServer
{
  Q_OBJECT
public:
  tServer(QObject *parent = nullptr);
  void say();
  ~tServer();
protected:
    void incomingConnection(qintptr socketDescriptor) override;
private:
    QStringList fortunes;
};

class tThread: public QThread{
  Q_OBJECT
public:
  tThread(int socketDescriptor, const QString &fortune, QObject *parent);
  void run() override;
signals:
  void error(QTcpSocket::SocketError socketError);

private:
  int socketDescriptor;
  QString text;
};

#endif // TSERVER_H
