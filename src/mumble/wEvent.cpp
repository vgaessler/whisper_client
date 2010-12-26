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
 #include <iostream>
#include "wGlobal.h"
#include "wError.h"
#include "wXmlWriter.h"
#include "wEvent.h"

using namespace std;
using namespace whisper;

//Resets an event
void Event::reset(QByteArray rsAccountHandle, QByteArray rsSessionGroupHandle, QByteArray rsSessionHandle) {
    Message::sData.clear();
    sAccountHandle	= rsAccountHandle;
    sSessionGroupHandle = rsSessionGroupHandle;
    sSessionHandle = rsSessionHandle;
}

//Compose a new event
void Event::compose() {
    XmlWriter W(Message::sData);
    QByteArray qsType;

    getType(qsType);

    W.writeStart("Event");
    W.writeAttribute("type", qsType);
    composeEvent(W);

    W.writeEnd("Event");
}

//------------------
//Events definitions
//------------------

void AccountLoginStateChangeEvent::setState(const QByteArray rsState) {
    sState = rsState;
}


void AccountLoginStateChangeEvent::composeEvent(XmlWriter& rW) {
    rW.writeStart("AccountHandle");
    rW.writeText(sAccountHandle);
    rW.writeEnd("AccountHandle");

    rW.writeStart("StatusCode");
    rW.writeText("200");
    rW.writeEnd("StatusCode");

    rW.writeStart("StatusString");
    rW.writeText("OK");
    rW.writeEnd("StatusString");

    rW.writeStart("State");
    rW.writeText(sState);
    rW.writeEnd("State");
}

void BuddyPresenceEvent::composeEvent(XmlWriter& rW) {
    rW.writeStart("AccountHandle");
    rW.writeText(sAccountHandle);
    rW.writeEnd("AccountHandle");

    rW.writeStart("BuddyURI");
    rW.writeText(sBuddyUri);
    rW.writeEnd("BuddyURI");

    rW.writeStart("Presence");
    rW.writeText(bOnline ? "Online" : "Unknown");
    rW.writeEnd("Presence");

    rW.writeEmpty("Alias");

    if(sDisplayName.isEmpty())
        rW.writeEmpty("DisplayName");
    else {
        rW.writeStart("DisplayName");
        rW.writeText(sDisplayName);
        rW.writeEnd("DisplayName");
    }

    if(sApplication.isEmpty())
        rW.writeEmpty("Application");
    else {
        rW.writeStart("Application");
        rW.writeText(sApplication);
        rW.writeEnd("Application");
    }

    rW.writeEmpty("CustomMessage");
}

void BuddyAndGroupListChangeEvent::composeEvent(XmlWriter& rW) {
    rW.writeStart("AccountHandle");
    rW.writeText(sAccountHandle);
    rW.writeEnd("AccountHandle");

    rW.writeStart("Buddies");
    rW.writeText(sBuddiesText);
    rW.writeEnd("Buddies");

    rW.writeEmpty("Groups");
}

void SessionGroupAddedEvent::composeEvent(XmlWriter& rW) {
    rW.writeStart("AccountHandle");
    rW.writeText(sAccountHandle);
    rW.writeEnd("AccountHandle");

    rW.writeStart("SessionGroupHandle");
    rW.writeText(sSessionGroupHandle);
    rW.writeEnd("SessionGroupHandle");

    rW.writeStart("Type");
    rW.writeText("Normal");
    rW.writeEnd("Type");
}

void SessionGroupRemovedEvent::composeEvent(XmlWriter& rW) {
    rW.writeStart("SessionGroupHandle");
    rW.writeText(sSessionGroupHandle);
    rW.writeEnd("SessionGroupHandle");
}

void SessionAddedEvent::composeEvent(XmlWriter& rW) {
    rW.writeStart("SessionGroupHandle");
    rW.writeText(sSessionGroupHandle);
    rW.writeEnd("SessionGroupHandle");

    rW.writeStart("SessionHandle");
    rW.writeText(sSessionHandle);
    rW.writeEnd("SessionHandle");

    rW.writeStart("Uri");
    rW.writeText(sUri);
    rW.writeEnd("Uri");

    rW.writeStart("IsChannel");
    rW.writeText("true");
    rW.writeEnd("IsChannel");

    rW.writeStart("Incoming");
    rW.writeText("false");
    rW.writeEnd("Incoming");

    rW.writeEmpty("ChannelName");
    rW.writeEmpty("Alias");
    rW.writeEmpty("DisplayName");
    rW.writeEmpty("Application");
}

void SessionRemovedEvent::composeEvent(XmlWriter& rW) {
    rW.writeStart("SessionGroupHandle");
    rW.writeText(sSessionGroupHandle);
    rW.writeEnd("SessionGroupHandle");

    rW.writeStart("SessionHandle");
    rW.writeText(sSessionHandle);
    rW.writeEnd("SessionHandle");

    rW.writeStart("URI");
    rW.writeText(sUri);
    rW.writeEnd("URI");
}

void SessionUpdatedEvent::composeEvent(XmlWriter& rW) {
    rW.writeStart("SessionGroupHandle");
    rW.writeText(sSessionGroupHandle);
    rW.writeEnd("SessionGroupHandle");

    rW.writeStart("SessionHandle");
    rW.writeText(sSessionHandle);
    rW.writeEnd("SessionHandle");

    rW.writeStart("Uri");
    rW.writeText(sUri);
    rW.writeEnd("Uri");

    rW.writeStart("IsMuted");
    rW.writeText("0");
    rW.writeEnd("IsMuted");

    rW.writeStart("Volume");
    rW.writeText("50");
    rW.writeEnd("Volume");

    rW.writeStart("TransmitEnabled");
    rW.writeText("1");
    rW.writeEnd("TransmitEnabled");

    rW.writeStart("IsFocused");
    rW.writeText("0");
    rW.writeEnd("IsFocused");

    rW.writeStart("SpeakerPosition");
    rW.writeStart("Position");
    rW.writeStart("X");
    rW.writeText("0");
    rW.writeEnd("X");
    rW.writeStart("Y");
    rW.writeText("0");
    rW.writeEnd("Y");
    rW.writeStart("Z");
    rW.writeText("0");
    rW.writeEnd("Z");
    rW.writeEnd("Position");
    rW.writeEnd("SpeakerPosition");

    rW.writeStart("SessionFontId");
    rW.writeText("0");
    rW.writeEnd("SessionFontId");
}


void MediaStreamUpdatedEvent::setState(const QByteArray rsState) {
    sState = rsState;
}


void MediaStreamUpdatedEvent::composeEvent(XmlWriter& rW) {
    rW.writeStart("SessionGroupHandle");
    rW.writeText(sSessionGroupHandle);
    rW.writeEnd("SessionGroupHandle");

    rW.writeStart("SessionHandle");
    rW.writeText(sSessionHandle);
    rW.writeEnd("SessionHandle");

    rW.writeStart("StatusCode");
    rW.writeText("200");
    rW.writeEnd("StatusCode");

    rW.writeStart("StatusString");
    rW.writeText("OK");
    rW.writeEnd("StatusString");

    rW.writeStart("State");
    rW.writeText(sState);
    rW.writeEnd("State");

    rW.writeStart("Incoming");
    rW.writeText("false");
    rW.writeEnd("Incoming");
}


void ParticipantAddedEvent::composeEvent(XmlWriter& rW) {
    rW.writeStart("SessionGroupHandle");
    rW.writeText(sSessionGroupHandle);
    rW.writeEnd("SessionGroupHandle");

    rW.writeStart("SessionHandle");
    rW.writeText(sSessionHandle);
    rW.writeEnd("SessionHandle");

    rW.writeStart("ParticipantUri");
    rW.writeText(sParticipantUri);
    rW.writeEnd("ParticipantUri");

    rW.writeStart("AccountName");
    rW.writeText(sAccountName);
    rW.writeEnd("AccountName");

    rW.writeEmpty("DisplayName");

    rW.writeStart("ParticipantType");
    rW.writeText("0");
    rW.writeEnd("ParticipantType");

    rW.writeEmpty("Alias");

    rW.writeEmpty("Application");
}

void ParticipantUpdatedEvent::composeEvent(XmlWriter& rW) {
    rW.writeStart("SessionGroupHandle");
    rW.writeText(sSessionGroupHandle);
    rW.writeEnd("SessionGroupHandle");

    rW.writeStart("SessionHandle");
    rW.writeText(sSessionHandle);
    rW.writeEnd("SessionHandle");

    rW.writeStart("ParticipantUri");
    rW.writeText(sParticipantUri);
    rW.writeEnd("ParticipantUri");

    rW.writeStart("IsModeratorMuted");
    rW.writeText("false");
    rW.writeEnd("IsModeratorMuted");

    rW.writeStart("IsSpeaking");
    rW.writeText(sIsSpeaking);
    rW.writeEnd("IsSpeaking");

    rW.writeStart("Volume");
    rW.writeText(sVolume);
    rW.writeEnd("Volume");

    rW.writeStart("Energy");
    rW.writeText(sEnergy);
    rW.writeEnd("Energy");

    rW.writeStart("ActiveMedia");
    rW.writeText("1");
    rW.writeEnd("ActiveMedia");

    rW.writeStart("IsMutedForMe");
    rW.writeText("false");
    rW.writeEnd("IsMutedForMe");
}

void ParticipantRemovedEvent::composeEvent(XmlWriter& rW) {
    rW.writeStart("SessionGroupHandle");
    rW.writeText(sSessionGroupHandle);
    rW.writeEnd("SessionGroupHandle");

    rW.writeStart("SessionHandle");
    rW.writeText(sSessionHandle);
    rW.writeEnd("SessionHandle");

    rW.writeStart("ParticipantUri");
    rW.writeText(sParticipantUri);
    rW.writeEnd("ParticipantUri");

    rW.writeStart("AccountName");
    rW.writeText(sAccountName);
    rW.writeEnd("AccountName");

    rW.writeStart("Reason");
    rW.writeText("Left");
    rW.writeEnd("Reason");
}

void AuxAudioPropertiesEvent::composeEvent(XmlWriter& rW) {
    rW.writeStart("MicIsActive");
    rW.writeText(this->micIsActive);
    rW.writeEnd("MicIsActive");

    rW.writeStart("MicEnergy");
    rW.writeText(this->micEnergy);
    rW.writeEnd("MicEnergy");

    rW.writeStart("MicVolume");
    rW.writeText(this->micVolume);
    rW.writeEnd("MicVolume");

    rW.writeStart("SpeakerVolume");
    rW.writeText(this->speakerVolume);
    rW.writeEnd("SpeakerVolume");
}
