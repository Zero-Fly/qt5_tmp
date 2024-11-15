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
#include <QWaitCondition>


class Worker : public QObject {
    Q_OBJECT

public:
    Worker(QString host, int port): host(host), port(port) {};
    ~Worker() = default;

public slots:
    void process();

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
    BlockingClient(QWidget *parent = nullptr);
    QString getStatusLabel();
    void PrintFort();
private slots:
    void requestNewFortune();
    void Fort();
    void showFortune(QString nextFortune);
    void displayError(int socketError, const QString &message);
    void enableGetFortuneButton();

private:
    QString host = "192.168.22.157";
    int port = 4242;
    QTcpSocket socket;
    QString currentFortune;
    QLineEdit *hostLineEdit = nullptr;
    QLineEdit *portLineEdit = nullptr;
    QLabel *statusLabel;
    QPushButton *getFortuneButton = nullptr;
    QDialogButtonBox *buttonBox = nullptr;
    QVBoxLayout *boxLayout = nullptr;
};

