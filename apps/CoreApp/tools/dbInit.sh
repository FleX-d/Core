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
	Freez		TEXT NOT NULL,
	Unfreez		TEXT NOT NULL,
	UpdateDB	TEXT NOT NULL,
	MD5		TEXT NOT NULL
);

INSERT INTO CoreAppDb(
ID,AppName,AppVer,Install,Uninstall,Start,Stop,Freez,Unfreez,UpdateDB,MD5) 
VALUES (
NULL,
'test',
'test',
'echo install',
'echo uninstall',
'echo start',
'echo stop',
'echo freez',
'echo unfreez',
'echo update; echo example of error; apt-get install ',
'');

.save CoreAppDb.db
.exit
EOF