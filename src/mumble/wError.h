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

#ifndef WERROR_H
#define WERROR_H

#pragma warning(disable : 4996)

//#include <iostream>
#include <QtCore>

namespace whisper {
    class Error {
    public:
        static Error& instance();
        void writeErrorLog(QString& rs);
        void writeOutLog(QString& rs);
		bool bDebug;
    protected:
        Error();
        void open();
    private:
        static Error* pInstance;
        QFile* pLog;
		QMutex mWriteMutex;
    };
}

#define WCODE wErrCode
#define WDEF int WCODE = 0

#define WASSERT(a) {if(!(a)){QString s; s.sprintf("ASSERTION: %s in file %s at %d\n", #a, __FILE__, __LINE__); Error::instance().writeErrorLog(s); wErrCode = -1; goto wEnd;}}

#define WWRITE1(a)          {QString s; s.sprintf("LOG: %s\n", a); Error::instance().writeOutLog(s);}
#define WWRITE2(a, b)       {QString s; s.sprintf("LOG: %s\n", QString().sprintf(a, b).toAscii().data()); Error::instance().writeOutLog(s);}
#define WWRITE3(a, b, c)    {QString s; s.sprintf("LOG: %s\n", QString().sprintf(a, b, c).toAscii().data()); Error::instance().writeOutLog(s);}

#define WDEBUG1(a)          {if(Error::instance().bDebug) {QString s; s.sprintf("DEBUG: %s\n", a); Error::instance().writeOutLog(s);}}
#define WDEBUG2(a, b)       {if(Error::instance().bDebug) {QString s; s.sprintf("DEBUG: %s\n", QString().sprintf(a, b).toAscii().data()); Error::instance().writeOutLog(s);}}
#define WDEBUG3(a, b, c)    {if(Error::instance().bDebug) {QString s; s.sprintf("DEBUG: %s\n", QString().sprintf(a, b, c).toAscii().data()); Error::instance().writeOutLog(s);}}

#define WTHROW1(a)          {QString s; s.sprintf("EXCEPTION: %s in file %s at %d \n", #a, __FILE__, __LINE__); Error::instance().writeErrorLog(s); wErrCode = -1; goto wEnd;}
#define WTHROW2(a, b)       {QString s; s.sprintf("EXCEPTION: %s in file %s at %d \n", QString().sprintf(a, b).toAscii().data(), __FILE__, __LINE__); Error::instance().writeErrorLog(s); wErrCode = -1; goto wEnd;}
#define WTHROW3(a, b, c)    {QString s; s.sprintf("EXCEPTION: %s in file %s at %d \n", QString().sprintf(a, b, c).toAscii().data(), __FILE__, __LINE__); Error::instance().writeErrorLog(s); wErrCode = -1; goto wEnd;}

#define WEND wEnd

#endif // WERROR_H
