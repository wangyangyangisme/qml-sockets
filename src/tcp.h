
#include <QtNetwork>


class TCPSocket : public QObject
{
    Q_OBJECT
    Q_ENUMS(QAbstractSocket::SocketState)
    
    Q_PROPERTY(QString host    MEMBER m_host    NOTIFY hostChanged)
    Q_PROPERTY(uint    port    MEMBER m_port    NOTIFY portChanged)
    
    Q_PROPERTY(QAbstractSocket::SocketState state MEMBER m_state NOTIFY stateChanged)
    
signals:
    void hostChanged();
    void portChanged();
    void stateChanged();
    
    void error(int socketError, const QString &message);
    
    void read(const QString &message);
    void connected();
    void disconnected();
    
public:
    TCPSocket()
    {
        m_socket = new QTcpSocket(this);
        
        QObject::connect(m_socket, &QAbstractSocket::stateChanged,
            [=](QAbstractSocket::SocketState socketState)
            { setProperty("state", socketState); });
        
        QObject::connect(m_socket, &QAbstractSocket::readyRead,
            [=]()
            { emit read(m_socket->readAll()); });
        
        QObject::connect(m_socket, &QAbstractSocket::connected,
            [=]()
            { emit connected(); });
        
        QObject::connect(m_socket, &QAbstractSocket::disconnected,
            [=]()
            { emit disconnected(); });
    };
    
    ~TCPSocket()
    { delete m_socket; m_socket = NULL; }
    
public slots:
    void connect()
    { m_socket->connectToHost(m_host, m_port); }
    
    void write(QString message)
    { m_socket->write(message.toLocal8Bit()); }
    
public:
    QString m_host;
    uint    m_port;
    QAbstractSocket::SocketState m_state;
    QTcpSocket *m_socket = NULL;
};