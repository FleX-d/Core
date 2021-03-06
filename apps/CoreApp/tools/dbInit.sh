#!/bin/bash

mkdir -p /etc/CoreApp

sqlite3 << EOF

CREATE TABLE CoreAppDb (
	ID		INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
	AppName		TEXT NOT NULL,
	AppVer		TEXT NOT NULL,
	Install		TEXT NOT NULL,
	Uninstall	TEXT NOT NULL,
	Start		TEXT NOT NULL,
	Stop		TEXT NOT NULL,
	Freez		TEXT NOT NULL,
	Unfreez		TEXT NOT NULL,
	UpdateDB	TEXT NOT NULL,
	MD5		TEXT NOT NULL
);

INSERT INTO CoreAppDb(
ID,AppName,AppVer,Install,Uninstall,Start,Stop,Freez,Unfreez,UpdateDB,MD5) 
VALUES (
NULL,
'test1',
'test1',
'echo install; dpkg -i ',
'echo uninstall; dpkg -r test1 ',
'echo start; echo lxc or docker command',
'echo stop; echo lxc or docker command',
'echo freez; echo lxc or docker command',
'echo unfreez; echo lxc or docker command',
'echo update; dpkg -i ',
'');

INSERT INTO CoreAppDb(
ID,AppName,AppVer,Install,Uninstall,Start,Stop,Freez,Unfreez,UpdateDB,MD5) 
VALUES (
NULL,
'test2',
'test2',
'echo install; dpkg -i ',
'echo uninstall; dpkg -r test2 ',
'echo start; echo lxc or docker command',
'echo stop; echo lxc or docker command',
'echo freez; echo lxc or docker command',
'echo unfreez; echo lxc or docker command',
'echo update; dpkg -i ',
'');

INSERT INTO CoreAppDb(
ID,AppName,AppVer,Install,Uninstall,Start,Stop,Freez,Unfreez,UpdateDB,MD5) 
VALUES (
NULL,
'test3',
'test3',
'echo install; dpkg -i ',
'echo uninstall; dpkg -r test3 ',
'echo start; echo lxc or docker command',
'echo stop; echo lxc or docker command',
'echo freez; echo lxc or docker command',
'echo unfreez; echo lxc or docker command',
'echo update; dpkg -i ',
'');

.save /etc/CoreApp/CoreAppDb.db
.exit
EOF

if(touch /etc/CoreApp/CoreAppDb.db)
then
  echo Database inicilized!!
else
  echo Error run als root
fi