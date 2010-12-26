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

#include "wError.h"
#include "wXmlWriter.h"

using namespace std;
using namespace whisper;


XmlWriter::XmlWriter(QByteArray* pD) {
    pData = pD;
}

XmlWriter::XmlWriter(QByteArray& rD) {
    pData = &rD;
}

void XmlWriter::writeStart(const char *pcTag){
    pData->append("<");
    pData->append(pcTag);
    pData->append(">");
}

void XmlWriter::writeEnd(const char *pcTag) {
    pData->append("</");
    pData->append(pcTag);
    pData->append(">");
}

void XmlWriter::writeAttribute(const char *pcName, const char *pcValue) {
    pData->truncate(pData->size()-1);
    pData->append(" ");
    pData->append(pcName);
    pData->append("=\"");
    pData->append(pcValue);
    pData->append("\">");
}

void XmlWriter::writeAttribute(const char *pcName, QString& rS) {
    writeAttribute(pcName, rS.toAscii().data());
}

void XmlWriter::writeText(const char *pcText) {
    pData->append(pcText);
}
void XmlWriter::writeText(QString& rS) {
    pData->append(rS.toAscii());
}

void XmlWriter::writeEmpty(const char* pcTag) {
    pData->append("<");
    pData->append(pcTag);
    pData->append(" />");
}
