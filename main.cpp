#include"src/server/tserver.h"
#include"src/client/tclient.h"
#include <QApplication>
#include<iostream>

int main(int argc, char* argv[]){
  QApplication app(argc, argv);
  tServer server;
  tClient client;
  client.show();
  std::cout<<"Fuck u, Qt5!"<<std::endl;
  return app.exec();;
}
