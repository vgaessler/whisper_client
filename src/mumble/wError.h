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
