# TFtp

#### 介绍
基于Qt5.12的TFTP的Server和Client。

#### 软件架构
TFTP软件包括Client和Server，用户通过GetFile来下载文件，通过PutFile来上传文件。
1. GetFile序列图
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
2. PutFile序列图
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
3. 类图
   
   ![](https://gitee.com/flysnow_1/tftp/raw/master/doc/out/class/TftpClass.svg)
#### TFTP协议
1. TFTP协议是基于UDP的简单文件传输协议，协议双方为Client和Server.Client和Server之间通过5种消息来传输文件,消息前两个字节Code是消息类型，消息内容随消息类型不同而不同。传输模式有三种：octet,netascii和mail，octet为二进制模式，netascii为文本模式，mail为文本模式，不过收到的文本不是保存到文件，而是打印出来，现在已不常用。DATA消息种数据长度是是128或512字节，最后一个数据包可能会小于128或512。
  
2. 消息类型如下：
```
Code        Type   Desc
0x00 0x01   RRQ    Read message
0x00 0x02   WRQ    Write message
0x00 0x03   DATA   Data  message
0x00 0x04   ACK    Ack   message
0x00 0x05   ERROR  Error message
```
1.  消息格式
* 3.1 RRQ消息
```
Code      FileName      NULL   MODE      NULL
0x00 0x01 filename.bin  0x00   octet     0x00
0x00 0x01 filename.txt  0x00   netascii  0x00
0x00 0x01 filename.txt  0x00   mail      0x00
```
* 3.2 WRQ消息
```
Code      FileName      NULL   MODE      NULL
0x00 0x02 filename.bin  0x00   octet     0x00
0x00 0x02 filename.txt  0x00   netascii  0x00
0x00 0x02 filename.txt  0x00   mail      0x00
```
* 3.3 DATA消息
```
Code      BlockNumber Data(128/512bytes)
0x00 0x03 0x00 0x01   data
```
* 3.4 ACK消息
```
Code      BlockNumber
0x00 0x04 0x00 0x01
```
* 3.5 ERROR消息
```
Code      ErrorCode   ErrorMsg        NULL
0x00 0x05 0x00 0x01   File Not Found  0x00
```
1. 错误类型
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