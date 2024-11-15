#include <QWidget>
#include <QTcpSocket>
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
#include <iostream>
#include <QWaitCondition>
#include "tclient.h"

void Worker::process() {
  socket.abort();
  socket.connectToHost(host, port); // Connect to the server
  if (!socket.waitForConnected(3000)) {
      std::cout << "Connection failed!" << std::endl;
      emit error("Connection failed!");
  }

  std::cout << "Connected to server!" << std::endl;
  QDataStream in(&socket);
  QString Fortune;

  if(!socket.waitForReadyRead(3000)){
      std::cout << "Reading failed!" << std::endl;
      emit error("Reading failed!");
  }
  in.startTransaction();
  in >> Fortune;
  socket.disconnectFromHost();
  emit finished(Fortune);
};


BlockingClient::BlockingClient(QWidget *parent) : QDialog(parent),
                                              boxLayout(new QVBoxLayout(parent)),
                                              hostLineEdit(new QLineEdit(parent)),
                                              portLineEdit(new QLineEdit(parent)),
                                              statusLabel(new QLabel(tr(" -Fortune-"), parent)),
                                              getFortuneButton(new QPushButton(tr("Get Fortune"), parent))
  {
    QHBoxLayout* ipHLayout = new QHBoxLayout(parent);
    ipHLayout->addWidget(new QLabel(tr("IP: "),parent));
    ipHLayout->addWidget(hostLineEdit);
    QHBoxLayout* portHLayout = new QHBoxLayout(parent);
    portHLayout->addWidget(new QLabel(tr("port: "),parent));
    portHLayout->addWidget(portLineEdit);

    boxLayout->addLayout(ipHLayout);
    boxLayout->addLayout(portHLayout);
    boxLayout->addWidget(statusLabel);
    boxLayout->addWidget(getFortuneButton);
    setLayout(boxLayout);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    connect(getFortuneButton, &QPushButton::clicked, this, &BlockingClient::requestNewFortune);
    //std::cout << "This is fucking rise!!!"<<std::endl;
  };

  void BlockingClient::PrintFort(){
    std::cout<< currentFortune.toStdString() << std::endl;
  }

  QString BlockingClient::getStatusLabel(){
    return this->statusLabel->text();
  };

  void BlockingClient::requestNewFortune() {
    getFortuneButton->setEnabled(false);
    host = hostLineEdit->text();
    port = portLineEdit->text().toInt();
    this->Fort();
  }

  void BlockingClient::Fort() {
    QThread* thread = new QThread;
    Worker* worker = new Worker(host, port);
    worker->moveToThread(thread);
    connect(worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(thread, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, &Worker::finished, this, &BlockingClient::showFortune);
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
  };
    void BlockingClient::showFortune(QString nextFortune){
          currentFortune = nextFortune;
          statusLabel->setText(currentFortune);
          getFortuneButton->setEnabled(true);
    };
    void BlockingClient::displayError(int socketError, const QString &message){};
    void BlockingClient::enableGetFortuneButton() {  getFortuneButton->setEnabled(true); };



