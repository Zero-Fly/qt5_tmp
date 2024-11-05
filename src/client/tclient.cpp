#include <QApplication>
#include <QWidget>
#include <QTcpSocket>
#include <iostream>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>

/*
class FortuneThread : public QThread
{
    Q_OBJECT

public:
    FortuneThread(QObject *parent = nullptr);
    ~FortuneThread();

    void requestNewFortune(const QString &hostName, quint16 port);
    void run() override;

signals:
    void newFortune(const QString &fortune);
    void error(int socketError, const QString &message);

private:
    QString hostName;
    quint16 port;
    QMutex mutex;
    QWaitCondition cond;
    bool quit;
};
*/

class Worker : public QObject {
    Q_OBJECT

public:
    Worker(){};
    ~Worker(){};

public slots:
    void process(){
      socket.connectToHost(host, port); // Connect to the server
      if (!socket.waitForConnected(3000)) {
          std::cout << "Connection failed!" << std::endl;
          emit error("Connection failed!");
          return;
      }

      std::cout << "Connected to server!" << std::endl;
      QDataStream in(&socket);
      QString Fortune;

      if(!socket.waitForReadyRead(3000)){
          std::cout << "Reading failed!" << std::endl;
          emit error("Reading failed!");
          return;
        }
      in.startTransaction();
      in >> Fortune;
      socket.disconnectFromHost();
      emit finished(Fortune);
    };

signals:
    void finished(QString fortune);
    void error(QString err);

private:
    QString host = "127.0.0.1";
    qint16 port = 4242;
    QTcpSocket socket;
};


class BlockingClient : public QDialog
{
    Q_OBJECT

public:
  BlockingClient(QWidget *parent = nullptr)
  : QDialog(parent)
  , boxLayout(new QVBoxLayout)
  , statusLabel(new QLabel(tr(" -Fortune-")))
  , getFortuneButton(new QPushButton(tr("Get Fortune")))
  {
    boxLayout->addWidget(statusLabel);
    boxLayout->addWidget(getFortuneButton);
    setLayout(boxLayout);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    connect(getFortuneButton, &QPushButton::clicked, this, &BlockingClient::requestNewFortune);
    std::cout << "This is fucking rise!!!"<<std::endl;
  };

  void PrintFort(){
    std::cout<< currentFortune.toStdString() << std::endl;
  }
private slots:
  void requestNewFortune(){
    getFortuneButton->setEnabled(false);
    this->Fort();
  }

  void Fort(){
    QThread* thread = new QThread;
    Worker* worker = new Worker();
    worker->moveToThread(thread);
    connect(worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(thread, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, &Worker::finished, this, &BlockingClient::showFortune);
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
    //std::cout<< Fortune.toStdString() << std::endl;
  };
    void showFortune(QString nextFortune){
          if (nextFortune == currentFortune) {
                  requestNewFortune();
                  return;
              }

          currentFortune = nextFortune;
          statusLabel->setText(currentFortune);
          getFortuneButton->setEnabled(true);
    };
    void displayError(int socketError, const QString &message){};
    void enableGetFortuneButton(){  getFortuneButton->setEnabled(true); };

private:
    QString host = "127.0.0.1";
    qint16 port = 4242;
    QTcpSocket socket;
    QString currentFortune;
    QLabel *statusLabel;
    QPushButton *getFortuneButton = nullptr;
    QDialogButtonBox *buttonBox = nullptr;
    QVBoxLayout *boxLayout = nullptr;
};

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    BlockingClient client;
    client.resize(400, 300); // Set the window size
    client.show();



    return a.exec();
}

#include <tclient.moc>
