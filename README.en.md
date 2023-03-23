# TFtp

#### Description
基于Qt的TFTP的Server和Client。

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