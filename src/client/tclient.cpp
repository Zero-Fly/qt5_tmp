#include "tclient.h"
#include <QMessageBox>

FortuneThread::FortuneThread(QObject *parent) : QThread(parent){}

void FortuneThread::requestNewFortune(const QString &hostName, quint16 port)
{
    QMutexLocker locker(&mutex);
    this->hostName = hostName;
    this->port = port;
    if (!isRunning())
        start();
    else
        cond.wakeOne();
}

void FortuneThread::run()
{
    mutex.lock();
    QString serverName = hostName;
    quint16 serverPort = port;
    mutex.unlock();
    while (!quit) {
      const int Timeout = 5 * 1000;

      QTcpSocket socket;
      socket.connectToHost(serverName, serverPort);
      if (!socket.waitForConnected(Timeout)) {
        emit error(socket.error(), socket.errorString());
        return;
      }
      QDataStream in(&socket);
      in.setVersion(QDataStream::Qt_4_0);
      QString fortune;
      do {
        if (!socket.waitForReadyRead(Timeout)) {
          emit error(socket.error(), socket.errorString());
          return;
        }

        in.startTransaction();
        in >> fortune;
     } while (!in.commitTransaction());

     mutex.lock();
     emit newFortune(fortune);

     cond.wait(&mutex);
     serverName = hostName;
     serverPort = port;
     mutex.unlock();
    }
}

FortuneThread::~FortuneThread(){
  mutex.lock();
  quit = true;
  cond.wakeOne();
  mutex.unlock();
  wait();
}

tClient::tClient(QWidget *parent)
  : QWidget(parent)
  , layout(new QVBoxLayout)
  , hostCombo(new QComboBox)
  , portLineEdit(new QLineEdit)
  , getFortuneButton(new QPushButton(tr("Get Fortune")))
{
      layout->addWidget(hostCombo);
      layout->addWidget(getFortuneButton);
      setLayout(layout);
      setWindowTitle("ComboBox Example");
}

void tClient::requestNewFortune()
{
    getFortuneButton->setEnabled(false);
    thread.requestNewFortune(hostLineEdit->text(),
                             portLineEdit->text().toInt());
}

void tClient::showFortune(const QString &nextFortune)
{
    if (nextFortune == currentFortune) {
        requestNewFortune();
        return;
    }

    currentFortune = nextFortune;
    statusLabel->setText(currentFortune);
    getFortuneButton->setEnabled(true);
}

void tClient::displayError(int socketError, const QString &message)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the fortune server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The following error occurred: %1."));
    }

    getFortuneButton->setEnabled(true);
}

void tClient::enableGetFortuneButton(){
  getFortuneButton->setEnabled(true);
}

#include "tclient.moc"
