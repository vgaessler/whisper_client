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

#ifndef WGAMEHANDLER_H
#define WGAMEHANDLER_H

#include <QtNetwork>

#include "wMessage.h"

namespace whisper {

	// Abstract parent class for implementation of a game handler
	class GameHandler : public QThread {
	public:
		GameHandler(QObject *parent = 0) : QThread(parent) {bEnd = false; bInitialized = false;}
		virtual ~GameHandler() {}
		virtual void run() = 0;
		void end() {bEnd = true;}
		virtual void addParticipant(const unsigned int) = 0;
		virtual void removeParticipant(const unsigned int) = 0;
		virtual void setTalking(const unsigned int) = 0;
		virtual void talkingChanged(bool bTalking, QString& rUserName) = 0;
		virtual int  sendMessage(Message& rMsg) = 0;


	protected:
		void initialized() {bInitialized = true;}
		bool is_shutdown() {return bEnd;}
		bool is_active() {return !bEnd && bInitialized;}

	private:
		bool bEnd;
		bool bInitialized;
	};


	// Default game handler: does nothing
	class NullGameHandler : public GameHandler {
	public:
		NullGameHandler(QObject *parent = 0) : GameHandler(parent) {}
		virtual ~NullGameHandler() {}
		void run() {return;}
		void addParticipant(const unsigned int) {return;}
		void removeParticipant(const unsigned int) {return;}
		void setTalking(const unsigned int) {return;}

#pragma warning(disable : 4100)
		void talkingChanged(bool bTalking, QString& rUserName) {};
		int  sendMessage(Message& rMsg) {return 0;}
#pragma warning(default : 4100)
	};
}

#endif // WGAMEHANDLER_H
