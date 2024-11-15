#include <gtest/gtest.h>
#include <QTcpSocket>
#include <QSignalSpy>
#include "../../src/server/tserver.h"



// Mock class for simulating socket connections
class MockSocket : public QTcpSocket {
    Q_OBJECT
public:
    MockSocket(qintptr socketDescriptor, QObject *parent = nullptr)
        : QTcpSocket(parent) {
        setSocketDescriptor(socketDescriptor);
    }
};

// Test suite for the FortuneServer
class FortuneServerTest : public testing::Test {
protected:
    FortuneServer server;
    //QCoreApplication a();
    void SetUp() override {
        // Start the server for testing
        ASSERT_TRUE(server.listen(QHostAddress::LocalHost, 4242)); // Use any available port
    }

    void TearDown() override {
        server.close();
    }
};

// Test that the server starts correctly
TEST_F(FortuneServerTest, ServerStarts) {
    EXPECT_TRUE(server.isListening());
}

// Test that the server handles a new connection
TEST_F(FortuneServerTest, HandlesNewConnection) {
    qintptr socketDescriptor = 123; // Mock socket descriptor
    FortuneThread thread(socketDescriptor, "Test fortune");

    QSignalSpy spy(&thread, &FortuneThread::result);
    thread.run(); // Simulate running the thread

    EXPECT_EQ(spy.count(), 1);
    EXPECT_EQ(spy.takeFirst().at(0).toString(), "end sending");
}

// Test that a fortune is sent to a client
TEST_F(FortuneServerTest, SendsFortune) {
    QTcpSocket client;
    client.abort();
    client.connectToHost(QHostAddress::LocalHost, 4242);

    ASSERT_TRUE(client.waitForConnected(3000));


    QDataStream in(&client);
    QString receivedFortune;

    // Wait for the server to send data
    ASSERT_TRUE(client.waitForReadyRead(3000));

    in.startTransaction();
    in >> receivedFortune;
    // Check that the received fortune is in the list of fortunes
    EXPECT_TRUE(server.getFortuneList().contains(receivedFortune));

    client.disconnectFromHost();
    client.waitForDisconnected();
}

#include "TestServer.moc"
