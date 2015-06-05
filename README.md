# dynet-server
分步式服务端框架，可以无限水平的扩展(开发中)
主要服务器有:代理服，登录服，场景服，数据库处理服和世界服，并计划在这基础上再扩展一个中心服，用于连接各个世界服。

构建vs项目过程：下载dynet-server全部代码，下mysql库(已经编译好)，地址：mysql已经编译好地址:http://pan.baidu.com/s/1qW7b42S，boost版本过大，自行到官网下载，版本为1.55
安装mysql数据库：建一个swa_server的数据库，然后导入/Server/sql下的sql文件，关于启动DyNetDp与数据连接相关配置去/Server/conf/dp.cfg配置
编译，如果编译出错，请检查是否缺少引用等等
运行，运行前需要将/conf下的所有配置文件复制到运行的路径下，并根据自己的情况修改配置

