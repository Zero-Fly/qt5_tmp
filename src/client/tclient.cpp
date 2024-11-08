#include <QApplication>
#include <QWidget>
#include <QTcpSocket>
#include <iostream>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMutex>
#include <QThread>
#include <QComboBox>
#include <QLineEdit>
#include <QWaitCondition>


class Worker : public QObject {
    Q_OBJECT

public:
    Worker(int port): port(port){};
    ~Worker(){};

public slots:
    void process(){
      std::cout<<"The server is running on\n\nIP: "<< host.toStdString() << "\nport: "<< port <<"\n\n";
      socket.abort();
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
    QString host = "192.168.22.157";
    int port;
    QTcpSocket socket;
};


class BlockingClient : public QDialog
{
    Q_OBJECT

public:
  BlockingClient(QWidget *parent = nullptr)
  : QDialog(parent)
  , boxLayout(new QVBoxLayout)
  , hostCombo(new QComboBox)
  , portLineEdit(new QLineEdit)
  , statusLabel(new QLabel(tr(" -Fortune-")))
  , getFortuneButton(new QPushButton(tr("Get Fortune")))
  {
    QHBoxLayout* ipHLayout = new QHBoxLayout;
    ipHLayout->addWidget(new QLabel(tr("IP: ")));
    hostCombo->addItem("localhost");
    ipHLayout->addWidget(hostCombo);
    QHBoxLayout* portHLayout = new QHBoxLayout;
    portHLayout->addWidget(new QLabel(tr("port: ")));
    portHLayout->addWidget(portLineEdit);

    boxLayout->addLayout(ipHLayout);
    boxLayout->addLayout(portHLayout);
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
    port = portLineEdit->text().toInt();
    this->Fort();
  }

  void Fort(){
    QThread* thread = new QThread;
    Worker* worker = new Worker(port);
    worker->moveToThread(thread);
    connect(worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(thread, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, &Worker::finished, this, &BlockingClient::showFortune);
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
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
    QString host = "192.168.22.157";
    int port = 4242;
    QTcpSocket socket;
    QString currentFortune;
    QComboBox *hostCombo = nullptr;
    QLineEdit *portLineEdit = nullptr;
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
