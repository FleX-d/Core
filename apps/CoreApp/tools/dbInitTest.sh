#!/bin/bash

mkdir -p /home/$USER/tmp/CoreApp

sqlite3 << EOF

CREATE TABLE TestCoreAppDb (
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

INSERT INTO TestCoreAppDb(ID,AppName,AppVer,Install,Uninstall,Start,Stop,Freeze,Unfreeze,UpdateDB,Timeout,MD5) 
VALUES (
	NULL,
	'Test',
	'Test',
	'echo install;',
	'echo uninstall;',
	'echo start;',
	'echo stop;',
	'echo freeze;',
	'echo unfreeze;',
	'echo update;',
	0,
	' '
);

INSERT INTO TestCoreAppDb(ID,AppName,AppVer,Install,Uninstall,Start,Stop,Freeze,Unfreeze,UpdateDB,Timeout,MD5) 
VALUES (
	NULL,
	'Test_timeout',
	'Test_timeout',
	'echo install; sleep 3;',
	'echo uninstall; sleep 3;',
	'echo start; sleep 3;',
	'echo stop; sleep 3;',
	'echo freeze; sleep 3;',
	'echo unfreeze; sleep 3;',
	'echo update; sleep 3;',
	1,
	' '
);

.save /home/$USER/tmp/CoreApp/TestCoreAppDb.db
.exit
EOF

if(touch /home/$USER/tmp/CoreApp/TestCoreAppDb.db)
then
  chmod 777 /home/$USER/tmp/CoreApp/TestCoreAppDb.db
  echo Database inicialized!!
else
  echo Error run as root
fi