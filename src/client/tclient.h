#ifndef TCLIENT_H
#define TCLIENT_H
#include <QApplication>
#include <QWidget>
#include <QTcpSocket>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QVBoxLayout>


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


class tClient : public QWidget
{
    Q_OBJECT

public:
    tClient(QWidget *parent = nullptr);

private slots:
    void requestNewFortune();
    void showFortune(const QString &fortune);
    void displayError(int socketError, const QString &message);
    void enableGetFortuneButton();

private:
    QVBoxLayout *layout;
    QComboBox *hostCombo = nullptr;
    QLabel *hostLabel;
    QLabel *portLabel;
    QLineEdit *hostLineEdit;
    QLineEdit *portLineEdit;
    QLabel *statusLabel;
    QPushButton *getFortuneButton;
    QPushButton *quitButton;
    //QDialogButtonBox *buttonBox;

    FortuneThread thread;
    QString currentFortune;
};
#endif // TCLIENT_H
