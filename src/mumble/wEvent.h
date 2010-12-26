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
