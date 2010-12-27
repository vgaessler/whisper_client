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
//#include "wError.h"

using namespace whisper;

ConfigFile* ConfigFile::cfInstance = 0;

ConfigFile& ConfigFile::getInstance() {
    if(!cfInstance) {
        cfInstance = new ConfigFile();
    }
    return *cfInstance;
}

// for initializing the file
void ConfigFile::init() {
	ConfigFile::getInstance();
}

ConfigFile::ConfigFile() {

	// get ini file location
	// on Unix: /etc/whisper.ini
	// on Windows <userappdata>/whisper/whisper.ini
	// on Mac: To Do

	QString sConfigFile = "/etc/";

#ifdef Q_OS_WIN
	sConfigFile = QDir(".").absolutePath();
	sConfigFile += "/whisper/";
#endif

	sConfigFile += "whisper.ini";

    //Creates the QFile object
    cf = new QFile(sConfigFile);
    //Opens the file
    if(cf->open(QIODevice::ReadOnly|QIODevice::Text))
    {
        //WDEBUG1("File opened!!");
        //Set flag to true if succeed
        this->valid = true;
        //Creates a temporary parameter object
        Parameter temp;
        //Creates a string in order to remember the family
        QString tempFamily;
        while(!cf->atEnd()) {
            QString line = QString(cf->readLine());
            if(!line.trimmed().isEmpty() && !line.trimmed().startsWith("#")) {
                //Family declaration RegExp
                QRegExp f("^\\s*\\[(\\w*\\s?)*\\]");

                //Key-Value declaration RegExp
                QRegExp kv("^\\w*\\s?=\\s?\\w*");

                //If it's a family header
                if(f.indexIn(line, 0) >= 0) {
                    //Splits both sides ([ and ])
                    QStringList family = line.split('[');
                    family = QString(family.at(1)).split(']');
                    //Saves the family name
                    tempFamily = family[0];
                    //WDEBUG2("Header %s detected!!", tempFamily.toAscii().data());
                }

                //If it's a key=value pair
                else if(kv.indexIn(line, 0) >= 0) {
                    //Splits on equal character
                    QStringList keyValue = line.split('=');
                    //Left side is key
                    temp.key = QString(keyValue.at(0)).remove(' ');
                    //Right side is value
                    temp.value = QString(keyValue.at(1)).remove(' ').remove(QRegExp("\n?"));
                    //Family
                    temp.family = tempFamily;
                    //WDEBUG3("KeyValue %s = %s detected!!", keyValue.at(0).toAscii().data(), keyValue.at(1).toAscii().data());
                    //Save the former temp into the list provided it's complete
                    if(temp.isValid())
                        this->lcf.append(temp);
                }
            }
        }
        cf->close();
    } else {
        this->valid = false;
    }
}

const QString ConfigFile::getValue(const QString &family, const QString &key) {
    for(int i=0; i < this->lcf.size(); i++) {
        Parameter temp = static_cast<Parameter>(lcf.at(i));
        if(temp.getFamily() == family && temp.getKey() == key)
            return temp.getValue();
    }
    return QString("");
}
