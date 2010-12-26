/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>,
                            Volker Gaessler <volker.gaessler@vcomm.ch

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
