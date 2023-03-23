# TFtp

#### Description
QT5.12 based TFTP server and client.

#### Software Architecture

TFTP software includes Client and Server. Users download files through GetFile and upload files through PutFile.


1. GetFile sequence diagram
```
     ,----.          ,----------.                  ,----------.
     |User|          |TFTPClient|                  |TFTPServer|
     `-+--'          `----+-----'                  `----+-----'
       |      GetFile     |                             |      
       | ----------------->                             |      
       |                  |                             |      
       |                  | Request of get file over UDP|      
       |                  | ---------------------------->      
       |                  |                             |      
       |                  |   Reponse a file over UDP   |      
       |                  | <----------------------------      
       |                  |                             |      
       |      A File      |                             |      
       | <-----------------                             |      
     ,-+--.          ,----+-----.                  ,----+-----.
     |User|          |TFTPClient|                  |TFTPServer|
     `----'          `----------'                  `----------'
```
2. PutFile sequence diagram
```
     ,----.          ,----------.                  ,----------.
     |User|          |TFTPClient|                  |TFTPServer|
     `-+--'          `----+-----'                  `----+-----'
       |      PutFile     |                             |      
       | ----------------->                             |      
       |                  |                             |      
       |                  | Request of put file over UDP|      
       |                  | ---------------------------->      
       |                  |                             |      
       |                  |     Reponse ACK over UDP    |      
       |                  | <----------------------------      
       |                  |                             |      
       |        ACK       |                             |      
       | <-----------------                             |      
     ,-+--.          ,----+-----.                  ,----+-----.
     |User|          |TFTPClient|                  |TFTPServer|
     `----'          `----------'                  `----------'
```
3. Class diagram
   
   ![](https://gitee.com/flysnow_1/tftp/raw/master/doc/out/class/TftpClass.svg)
#### TFTP Protocol
1. TFTP protocol is a simple file transfer protocol based on UDP. The two sides of the protocol are Client and Server. Files are transferred between Client and Server through five types of messages. The first two bytes of the message are the message type, and the message content varies depending on the message type. There are three transmission modes: octet, netascii, and mail. octet is in binary mode, netascii is in text mode, and mail is in text mode. However, the received text is not saved to a file, but printed to the screen, which is no longer commonly used. The data length of a DATA message is 128 or 512 bytes, and the last packet may be less than 128 or 512 bytes.
  
2. The message types are as follows：
```
Code        Type   Desc
0x00 0x01   RRQ    Read message
0x00 0x02   WRQ    Write message
0x00 0x03   DATA   Data  message
0x00 0x04   ACK    Ack   message
0x00 0x05   ERROR  Error message
```
3.  The message format 
* 3.1 RRQ message
```
Code      FileName      NULL   MODE      NULL
0x00 0x01 filename.bin  0x00   octet     0x00
0x00 0x01 filename.txt  0x00   netascii  0x00
0x00 0x01 filename.txt  0x00   mail      0x00
```
* 3.2 WRQ message
```
Code      FileName      NULL   MODE      NULL
0x00 0x02 filename.bin  0x00   octet     0x00
0x00 0x02 filename.txt  0x00   netascii  0x00
0x00 0x02 filename.txt  0x00   mail      0x00
```
* 3.3 DATA message
```
Code      BlockNumber Data(128/512bytes)
0x00 0x03 0x00 0x01   data
```
* 3.4 ACK message
```
Code      BlockNumber
0x00 0x04 0x00 0x01
```
* 3.5 ERROR message
```
Code      ErrorCode   ErrorMsg        NULL
0x00 0x05 0x00 0x01   File Not Found  0x00
```
4. The error type
```
ErrorCode   ErrorMsg
0x00 0x01   File Not Found
0x00 0x02   Access Violation
0x00 0x03   Disk Full
0x00 0x04   Illegal Operation
0x00 0x05   Unknown TransferID
0x00 0x06   File Exists
0x00 0x07   No Such User
```