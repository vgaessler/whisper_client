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

#ifndef WXMLWRITER_H
#define WXMLWRITER_H

#include <QtCore/QByteArray>

namespace whisper {

    class XmlWriter {
    public:
        XmlWriter(QByteArray* pD);
        XmlWriter(QByteArray& rD);

        void writeStart(const char *pcTag);
        void writeEnd(const char *pcTag);
        void writeAttribute(const char *pcName, const char *pcValue);
        void writeAttribute(const char *pcName, QString& rS);
        void writeText(const char *pcText);
        void writeText(QString& rS);
        void writeEmpty(const char *pcTag);

    private:
        QByteArray* pData;
    };
}

#endif // WXMLWRITER_H
