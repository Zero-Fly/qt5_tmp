#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QApplication>
#include <QTcpSocket>
#include "../../src/client/tclient.cpp"

// Mock class for QTcpSocket to simulate network behavior
class MockTcpSocket : public QTcpSocket {
    Q_OBJECT

public:
    MOCK_METHOD0(waitForConnected, bool());
    MOCK_METHOD0(waitForReadyRead, bool());
    MOCK_METHOD1(connectToHost, void(QString, quint16));
    MOCK_METHOD0(disconnectFromHost, void());
    MOCK_METHOD0(readAll, QByteArray());
};

// Test Worker class
class WorkerTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockSocket = new MockTcpSocket;
        worker = new Worker("localhost", 4242);
    }

    void TearDown() override {
        delete worker;
        delete mockSocket;
    }

    MockTcpSocket *mockSocket;
    Worker *worker;
};

// Test that Worker connects successfully and emits finished signal
TEST_F(WorkerTest, SuccessfulConnection) {
    EXPECT_CALL(*mockSocket, connectToHost("localhost", 4242)).Times(1);
    EXPECT_CALL(*mockSocket, waitForConnected()).WillOnce(::testing::Return(true));
    EXPECT_CALL(*mockSocket, waitForReadyRead()).WillOnce(::testing::Return(true));
    EXPECT_CALL(*mockSocket, readAll()).WillOnce(::testing::Return(QByteArray("Test Fortune")));

    QSignalSpy spy(worker, &Worker::finished);
    worker->process();

    EXPECT_EQ(spy.count(), 1);
    EXPECT_EQ(spy.takeFirst().at(0).toString(), "Test Fortune");
}

// Test BlockingClient class
class BlockingClientTest : public ::testing::Test {
protected:
    BlockingClient client;
};

// Test that BlockingClient initializes correctly
TEST_F(BlockingClientTest, Initialization) {
    EXPECT_EQ(client.windowTitle(), "Fortune Client");
    EXPECT_FALSE(client.isVisible());
}

// Test that requestNewFortune disables the button
TEST_F(BlockingClientTest, RequestNewFortuneDisablesButton) {
    client.show();
    client.requestNewFortune();
    EXPECT_FALSE(client.getFortuneButton->isEnabled());
}

#include "TestClient.moc"
