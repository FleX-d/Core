#!/bin/bash

sqlite3 << EOF

CREATE TABLE CoreAppDb (
	ID		INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
	AppName		TEXT NOT NULL,
	AppVer		TEXT NOT NULL,
	Install		TEXT NOT NULL,
	Uninstall	TEXT NOT NULL,
	Start		TEXT NOT NULL,
	Stop		TEXT NOT NULL,
	Freeze		TEXT NOT NULL,
	Unfreeze	TEXT NOT NULL,
	UpdateDB	TEXT NOT NULL,
	Timeout		BIGINT NOT NULL,
	MD5		TEXT NOT NULL
);

INSERT INTO CoreAppDb(
ID,AppName,AppVer,Install,Uninstall,Start,Stop,Freeze,Unfreeze,UpdateDB,Timeout,MD5) 
VALUES (
NULL,
'test1',
'test1',
'echo install; dpkg -i ',
'echo uninstall; dpkg -r test1 ',
'echo start; echo lxc or docker command',
'echo stop; echo lxc or docker command',
'echo freeze; echo lxc or docker command',
'echo unfreeze; echo lxc or docker command',
'echo update; dpkg -i ',
0,
'');

INSERT INTO CoreAppDb(
ID,AppName,AppVer,Install,Uninstall,Start,Stop,Freeze,Unfreeze,UpdateDB,Timeout,MD5) 
VALUES (
NULL,
'test2',
'test2',
'echo install; dpkg -i ',
'echo uninstall; dpkg -r test2 ',
'echo start; echo lxc or docker command',
'echo stop; echo lxc or docker command',
'echo freeze; echo lxc or docker command',
'echo unfreeze; echo lxc or docker command',
'echo update; dpkg -i ',
0,
'');

INSERT INTO CoreAppDb(
ID,AppName,AppVer,Install,Uninstall,Start,Stop,Freeze,Unfreeze,UpdateDB,Timeout,MD5) 
VALUES (
NULL,
'test3',
'test3',
'echo install; dpkg -i ',
'echo uninstall; dpkg -r test3 ',
'echo start; echo lxc or docker command',
'echo stop; echo lxc or docker command',
'echo freeze; echo lxc or docker command',
'echo unfreeze; echo lxc or docker command',
'echo update; dpkg -i ',
0,
'');

.save ./CoreAppDb.db
.exit
EOF
