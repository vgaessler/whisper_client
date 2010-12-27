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


#ifndef WEVENT_H
#define WEVENT_H

#include <QtCore/QByteArray>

#include "wXmlWriter.h"
#include "wMessage.h"

namespace whisper {

    // request base class
    class Event : public Message{
    public:
        void reset(QByteArray rsAccountHandle, QByteArray rsSessionGroupHandle, QByteArray rsSessionHandle);
        void compose();

    protected:
        QByteArray sAccountHandle;
        QByteArray sSessionGroupHandle;
        QByteArray sSessionHandle;

        virtual void getType(QByteArray& rsType)=0;
        virtual void composeEvent(XmlWriter& rsW)=0;
    };

    // Account Events
    class AccountLoginStateChangeEvent : public Event {
    public:
        void setState(const QByteArray rsState);
        void getType(QByteArray& rsType) {rsType = "AccountLoginStateChangeEvent";}
    protected:
        void composeEvent(XmlWriter& rW);
    private:
        QByteArray sState;
    };

    class BuddyAndGroupListChangeEvent : public Event {
    public:
        void getType(QByteArray& rsType) {rsType = "BuddyAndGroupListChangeEvent";}
        void setBuddiesText(QByteArray& rsBuddiesText) {sBuddiesText = rsBuddiesText;}
    protected:
        void composeEvent(XmlWriter& rW);
    private:
        QByteArray sBuddiesText;
    };

    class BuddyPresenceEvent : public Event {
    public:
        void getType(QByteArray& rsType) {rsType = "BuddyPresenceEvent";}
        void setBuddyUri(QByteArray& rsBuddyUri) {sBuddyUri = rsBuddyUri;}
        void setDisplayName(QByteArray& rs) {sDisplayName = rs;}
        void setApplication(QByteArray& rs) {sApplication = rs;}
        void setOnline(bool b) {bOnline = b;}
    protected:
        void composeEvent(XmlWriter& rW);
    private:
        QByteArray sBuddyUri;
        QByteArray sDisplayName;
        QByteArray sApplication;
        bool bOnline;
    };

    // Session events
    class SessionGroupAddedEvent : public Event {
    public:
        void getType(QByteArray& rsType) {rsType = "SessionGroupAddedEvent";}
    protected:
        void composeEvent(XmlWriter& rW);
    };

    // Session events
    class SessionGroupRemovedEvent : public Event {
    public:
        void getType(QByteArray& rsType) {rsType = "SessionGroupRemovedEvent";}
    protected:
        void composeEvent(XmlWriter& rW);
    };

    class SessionAddedEvent : public Event {
    public:
        void getType(QByteArray& rsType) {rsType = "SessionAddedEvent";}
        void setUri(QByteArray rUri) {sUri = rUri;}
    protected:
        void composeEvent(XmlWriter& rW);
    private:
        QByteArray sUri;
    };

    class SessionRemovedEvent : public Event {
    public:
        void getType(QByteArray& rsType) {rsType = "SessionRemovedEvent";}
        void setUri(QByteArray rUri) {sUri = rUri;}
    protected:
        void composeEvent(XmlWriter& rW);
    private:
        QByteArray sUri;
    };

    class SessionUpdatedEvent : public Event {
    public:
        void getType(QByteArray& rsType) {rsType = "SessionUpdatedEvent";}
        void setUri(QByteArray rUri) {sUri = rUri;}
    protected:
        void composeEvent(XmlWriter& rW);
    private:
        QByteArray sUri;
    };

    class MediaStreamUpdatedEvent : public Event {
    public:
        void getType(QByteArray& rsType) {rsType = "MediaStreamUpdatedEvent";}
        void setState(const QByteArray rsState);
    protected:
        void composeEvent(XmlWriter& rW);
    private:
        QByteArray sState;
    };

    class ParticipantAddedEvent : public Event {
    public:
        void getType(QByteArray& rsType) {rsType = "ParticipantAddedEvent";}
        void setParticipantUri(const QByteArray rs) {sParticipantUri = rs;}
        void setAccountName(const QByteArray rs) {sAccountName = rs;}
    protected:
        void composeEvent(XmlWriter& rW);
    private:
        QByteArray sParticipantUri;
        QByteArray sAccountName;
    };

    class ParticipantUpdatedEvent : public Event {
    public:
        void getType(QByteArray& rsType) {rsType = "ParticipantUpdatedEvent";}
        void setParticipantUri(const QByteArray rs) {sParticipantUri = rs;}
        void setIsSpeaking(const QByteArray rs) {sIsSpeaking = rs;}
        void setVolume(const QByteArray rs) {sVolume = rs;}
        void setEnergy(const QByteArray rs) {sEnergy = rs;}
    protected:
        void composeEvent(XmlWriter& rW);
    private:
        QByteArray sParticipantUri;
        QByteArray sIsSpeaking;
        QByteArray sVolume;
        QByteArray sEnergy;
    };

    class ParticipantRemovedEvent : public Event {
    public:
        void getType(QByteArray& rsType) {rsType = "ParticipantRemovedEvent";}
        void setParticipantUri(QByteArray rs) {sParticipantUri = rs;}
        void setAccountName(QByteArray rs) {sAccountName = rs;}
    protected:
        void composeEvent(XmlWriter& rW);
    private:
        QByteArray sParticipantUri;
        QByteArray sAccountName;
    };

    class AuxAudioPropertiesEvent : public Event {
    public:
        void getType(QByteArray& rsType) {rsType = "AuxAudioPropertiesEvent";}
        void setMicIsActive(bool value) {if(value == true) micIsActive = "true"; else micIsActive = "false";}
        void setMicEnergy(QByteArray energy) {micEnergy = energy;}
        void setMicVolume(QByteArray volume) {micVolume = volume;}
        void setSpeakerVolume(QByteArray volume) {speakerVolume = volume;}
    protected:
        void composeEvent(XmlWriter &rsW);
    private:
        QByteArray micIsActive;
        QByteArray micEnergy;
        QByteArray micVolume;
        QByteArray speakerVolume;
    };

}

#endif // WEVENT_H
