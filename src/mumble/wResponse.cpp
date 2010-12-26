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

#include "wXmlWriter.h"
#include "wRequest.h"
#include "wResponse.h"

using namespace std;
using namespace whisper;

void Response::reset(){
    sRequestId.clear();
    sAction.clear();
    sInputXml.clear();
}

int Response::compose(Actor& rActor) {
    Message::sData.clear();
    XmlWriter qW(Message::sData);

    qW.writeStart("Response");
    qW.writeAttribute("requestId", sRequestId);
    qW.writeAttribute("action", sAction);

    // <ReturnCode>
    qW.writeStart("ReturnCode");
    qW.writeText(rActor.isSuccess() ? "0" : "1");
    qW.writeEnd("ReturnCode");

    // <Results>
    qW.writeStart("Results");

    qW.writeStart("StatusCode");
    qW.writeText(rActor.getStatusCode());
    qW.writeEnd("StatusCode");

    if(rActor.getStatusString().isEmpty()) {
        qW.writeEmpty("StatusString");
    } else {
        qW.writeStart("StatusString");
        qW.writeText(rActor.getStatusString());
        qW.writeEnd("StatusString");
    }

    // get the response from the Actor
    rActor.response(&qW);

    qW.writeEnd("Results"); // <Results>

    qW.writeStart("InputXml");
    qW.writeText(sInputXml);
    qW.writeEnd("InputXml");

    qW.writeEnd("Response"); // <Response>

    return 0;

}
