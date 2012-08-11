#include "serialcom.h"
#include <QTimer>
#include <termios.h>

SerialCom::SerialCom(QObject *parent) :
    QObject(parent)
{
}


char SerialCom::Open(QString device, int baud)
{
    QByteArray deviceArray = device.toUtf8();
    const char* deviceString = deviceArray.constData();

    struct termios options;

    // Open device
    fd = open(deviceString, O_RDWR | O_NOCTTY | O_NDELAY);			// Open port
    if (fd == -1) return -2;						// If the device is not open, return -1
    fcntl(fd, F_SETFL, FNDELAY);					// Open the device in nonblocking mode

    // Set parameters
    tcgetattr(fd, &options);						// Get the current options of the port
    bzero(&options, sizeof(options));					// Clear all the options
   speed_t Speed;
    switch (baud)                                                      // Set the speed (Bauds)
    {
    case 0  :   Speed=B110; break;
    case 1  :   Speed=B300; break;
    case 2  :   Speed=B600; break;
    case 3  :   Speed=B1200; break;
    case 4  :   Speed=B2400; break;
    case 5  :   Speed=B4800; break;
    case 6  :   Speed=B9600; break;
    case 7  :   Speed=B19200; break;
    case 8  :   Speed=B38400; break;
    case 9  :   Speed=B57600; break;
    case 10 :   Speed=B115200; break;
    default : return -4;
}
    cfsetispeed(&options, Speed);
    options.c_cflag |= ( CLOCAL | CREAD |  CS8);    // Configure the device : 8 bits, no parity, no control
    options.c_iflag |= ( IGNPAR | IGNBRK );
    options.c_cc[VTIME]=0;                          // Timer unused
    options.c_cc[VMIN]=0;                           // At least on character before satisfy reading
    tcsetattr(fd, TCSANOW, &options);               // Activate the settings
    return (1);                                     // Success


}

void SerialCom::Close()
{

    close (fd);

    QString statusMessage = "Timeout Reached.  Successfully closed connection";
    emit closeStatus(statusMessage);
}


// Write a char on the current serial port.  Byte sent on the port must be terminated by '\0'.
char SerialCom::WriteChar(const char Byte)
{

    if (write(fd,&Byte,1)!=1)                                           // Write the char
        return -1;                                                      // Error while writting
    return 1;                                                           // Write operation successfull

}


// Write a string on the current serial port.  String sent on the port must be terminated by '\0'.

char SerialCom::WriteString(const char *String)
{

    int Length=strlen(String);                                          // Length of the string
    if (write(fd,String,Length)!=Length)                                // Write the string
        return -1;                                                      // error while writing
    return 1;                                                           // Write operation successfull

}


// Write an array of bytes on the current serial port.  NbBytes is number of bytes to send.

char SerialCom::Write(const void *Buffer, const unsigned int NbBytes)
{


    if (write (fd,Buffer,NbBytes)!=(ssize_t)NbBytes)                              // Write data
        return -1;                                                      // Error while writing
    return 1;                                                           // Write operation successfull

}


// Read a string from the serial device
int SerialCom::ReadStringNoTimeOut(char *String,char FinalChar)
{
    unsigned int    NbBytes=0;                                          // Number of bytes read
    int            ret;                                                // Returned value from Read
    while (NbBytes<255)                                          // While the buffer is not full
    {                                                                   // Read a byte with the restant time
        ret = read(fd,&String[NbBytes],1);
        if (ret==1)                                                     // If a byte has been read
          {
            if (String[NbBytes]==FinalChar)                             // Check if it is the final char
            {
                String  [++NbBytes]=0;                                  // Yes : add the end character 0
                return NbBytes;                                         // Return the number of bytes read
            }
            NbBytes++;                                                  // If not, just increase the number of bytes read
          }
        else
        {
            return NbBytes; // exit loop if no more bytes in buffer.
        }
    }
    return NbBytes;
}

void SerialCom::startListen()
{

    QTimer *listenTimer = new QTimer(this);
    connect(listenTimer, SIGNAL(timeout()), this, SLOT(Listen()));
    listenTimer->start(25);
}

void SerialCom::Listen()
{
    int numBytes = ReadStringNoTimeOut(buf,'\n');
    if(numBytes > 0)
   {
       QString receivedString = "<font color='blue'>Received: " + QString::fromAscii(buf,numBytes) + "</font>";
       // qDebug("%s\n", buf);
       emit(showReceived(receivedString));
   }
}



