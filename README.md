# TFtp

#### 介绍
基于Qt的TFTP的Server和Client。

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