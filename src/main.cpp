#include<QCoreApplication>
#include<QApplication>
#include<iostream>

#include"client/tclient.h"
#include"server/tserver.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
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


    BlockingClient client;
    client.resize(400, 300); // Set the window size
    client.show();

    return a.exec();
}
