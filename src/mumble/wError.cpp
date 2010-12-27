/*
   Whisper is a Mumble based voice client for OpenSim
   Copyright (C) 2009-2010  Volker Gaessler <volker.gaessler@vcomm.ch>

   This file is part of Whisper.

   Whisper is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>
*/

#include <cstdlib>
#include <QtCore>

#include "wConfigFile.h"
#include "wError.h"

using namespace std;
using namespace whisper;

//Singleton
Error* Error::pInstance = 0;
Error& Error::instance() {
    if(!pInstance) {
        pInstance = new Error;
    }
    return *pInstance;
}

//File to log
Error::Error() : pLog(NULL) {
	QString sLogLevel = ConfigFile::getInstance().getValue("debug", "log_level");
	bDebug = (sLogLevel == QString("debug"));

	// create the log file
	// Unix: /var/whisper.log
	// Windows: <userappdata>/whisper/whisper.log
	// Mac: To Do

	QString sLogFile = "/var/";

#ifdef Q_OS_WIN
	char* pcAppData = NULL;
	pcAppData = getenv("APPDATA");
	if (pcAppData) {
		sLogFile = pcAppData;
		sLogFile += "/whisper/";
	}
	else {
		sLogFile = "";
	}
#endif

	sLogFile += "whisper.log";

	pLog = new QFile(sLogFile);
}

//Opens the file
void Error::open() {
    if(!pLog->isOpen()) {
        pLog->open(QIODevice::WriteOnly);
    }
}

//Writes errors to log
void Error::writeErrorLog (QString & rs)
{
	QMutexLocker locker(&mWriteMutex);
    open();
    //cerr << rs.toAscii().data();
	pLog->write((QDateTime::currentDateTime()).toString("yyyy-MM-dd hh:mm:ss.zzz  ").toAscii());
    pLog->write(rs.toAscii());
    pLog->flush();
}

//Writes lines to log
void Error::writeOutLog (QString & rs)
{
	QMutexLocker locker(&mWriteMutex);
    open();
    //cout << rs.toAscii().data();
	pLog->write((QDateTime::currentDateTime()).toString("yyyy-MM-dd hh:mm:ss.zzz  ").toAscii());
    pLog->write(rs.toAscii());
    pLog->flush();
}
