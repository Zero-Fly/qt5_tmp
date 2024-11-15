#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QApplication>
#include <QTcpSocket>
#include <QSignalSpy>
#include <QTest>
#include "../../src/client/tclient.h"

// Test suite for the BlockingClient
class BlockingClientTest : public ::testing::Test {
protected:
    BlockingClient *client;

    void SetUp() override {
        client = new BlockingClient();
    }

    void TearDown() override {
        delete client;
    }
};



// Test suite for the Worker
class WorkerTest : public ::testing::Test {
protected:
    Worker *worker;
    QThread *thread;

    void SetUp() override {
        worker = new Worker("localhost", 12345);
        thread = new QThread();
        worker->moveToThread(thread);
    }

    void TearDown() override {
        thread->quit();
        thread->wait();
        delete worker;
        delete thread;
    }
};

  // Test that the Worker emits an error signal when connection fails
  TEST_F(WorkerTest, ConnectionFailureEmitsError) {
      QSignalSpy errorSpy(worker, SIGNAL(error(QString)));
      thread->start();
      worker->process();
      EXPECT_EQ(errorSpy.count(), 2);
      EXPECT_EQ(errorSpy.takeFirst().at(0).toString(), "Connection failed!");
  }

  // Test that the BlockingClient initializes correctly
  TEST_F(BlockingClientTest, Initialization) {
      EXPECT_EQ(client->getStatusLabel(), " -Fortune-");
  }

  // Main function to run the tests
  int main(int argc, char **argv) {
      QApplication app(argc, argv); // Create QApplication instance
      ::testing::InitGoogleTest(&argc, argv); // Initialize Google Test
      return RUN_ALL_TESTS(); // Run all tests
  }


#include "TestClient.moc"
