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
