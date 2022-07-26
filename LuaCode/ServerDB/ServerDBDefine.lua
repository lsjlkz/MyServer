---
--- Created by lsjlkz.
--- DateTime: 2022/7/21 16:02
--- Desc:
---



--LuaSql连Mysql比较麻烦
--首先要把luasql放到普通的lua源码里编译

--然后安装mysql后要关闭ssl

--关闭ssl后还需要修改密码加密规则

--修改加密规则方法如下
--1、登录Mysql：
--一般情况下命令行登录不了，所以需要用客户端登录，且客户端需要修改连接的SSL配置，改成no ssl
--mysql -u root -p
--2、修改账户密码加密规则并更新用户密码：
--        //修改加密规则（可以直接复制）
--ALTER USER 'root'@'localhost' IDENTIFIED BY 'password' PASSWORD EXPIRE NEVER;
--//更新一下用户的密码（可以直接复制）
--ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY 'password';
--3、刷新权限并重置密码
--//刷新权限（可以直接复制）
--FLUSH PRIVILEGES;
--4、重置密码
--//此处请自定义密码，红色的root就是博主自定义的密码
--alter user 'root'@'localhost' identified by 'root';
--此处将密码改为root

__G__ServerDBDefineTable = __G__ServerDBDefineTable or {}

__G__ServerDBDefineTable.Host = "localhost"
__G__ServerDBDefineTable.User = "admin"
__G__ServerDBDefineTable.Password = "admin"
__G__ServerDBDefineTable.Port = 3306

__G__ServerDBDefineTable.RoleDBName = "world"
__G__ServerDBDefineTable.RoleTableName = "role_data"

__G__ServerDBDefineTable.PersistentDBName = "world"
__G__ServerDBDefineTable.PersistentTableName = "persistent_data"


return __G__ServerDBDefineTable