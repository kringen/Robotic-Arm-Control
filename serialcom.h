#ifndef SERIALCOM_H
#define SERIALCOM_H
#include <sys/time.h>                                   // Used for TimeOut operations

// Include for windows
#if defined (_WIN32) || defined( _WIN64)
#include <windows.h>                                // Accessing to the serial port under Windows
#endif

// Include for Linux
#ifdef __linux__
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <termios.h>
#include <string.h>
#include <iostream>
#include <fcntl.h>                                  // File control definitions
#endif

#include <QObject>

class SerialCom : public QObject
{
    Q_OBJECT
public:
    explicit SerialCom(QObject *parent = 0);
    // ::: Configuration and initialization :::
    char    Open(QString device, int baud);      // Open a device
    //void    Close();                                                        // Close the device

    // ::: Read/Write operation on characters :::
    char    WriteChar(char);                                             // Write a char

    // ::: read/Write operation on strings :::
    char    WriteString(const char *String);

    // ::: Read/Write operation on bytes ::
    char    Write(const void *Buffer, const unsigned int NbBytes); // Write an array of bytes

    char buf[255];

    char positionByteArray[1];

    void Close();

    static bool timerActive;

private:
    int     ReadStringNoTimeOut  (char *String,char FinalChar);
#if defined (_WIN32) || defined( _WIN64)
    HANDLE          hSerial;
    COMMTIMEOUTS    timeouts;
#endif
#ifdef __linux__
    int  fd;
#endif

signals:
     void showReceived(QString receivedString);
     void closeStatus(QString statusMessage);

public slots:
     void Listen();
     void startListen();
};


// Class TimeOut
class TimeOut
{
        public:
                TimeOut();                                                      // Constructor
                void                InitTimer();                                // Init the timer
                unsigned long int   ElapsedTime_ms();                           // Return the elapsed time since initialization
        private:
                struct timeval      PreviousTime;
};


#endif // SERIALCOM_H
