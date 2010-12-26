/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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

#include "wConfigFile.h"
#include "wError.h"
#include "wGameHandler.h"
#include "wViewerHandler.h"

#define WHISPER_VERSION "0.2.5"

// dir name in application directory: source
#define WHISPER_APP_DIR "whisper"		

// dir name in data directory
#define WHISPER_DATA_DIR "whisper"		

using namespace whisper;


void InitializeDataDir() {
	// check if whisper directory exitsts. If not created it and copy files.
	// currently only implemented for Windows.

	char* pcAppData = NULL;
	QString sConfigDir;

#ifdef Q_OS_WIN
	pcAppData = getenv("APPDATA");
#endif

	if (pcAppData) {
		sConfigDir = pcAppData;
		sConfigDir += "/";
		sConfigDir += WHISPER_DATA_DIR;
		sConfigDir += "/";
		QDir dir(sConfigDir);
		if (!dir.exists()) {
			dir.mkpath(sConfigDir);
		}
	}
}


// ------------------------------------------------------------------------------
// main
// ------------------------------------------------------------------------------

int main_application(int argc, char **argv, GameHandler *pGh);

int main(int argc, char **argv) {

	InitializeDataDir();

	// Load config data (no logging up to this point)
	ConfigFile::init();

	WWRITE2("Start Version %s", WHISPER_VERSION); 
	WWRITE2("Compiled at %s", __TIMESTAMP__);

	// GameHandler *pVh = new NullGameHandler(0);
	GameHandler *pVh = new ViewerHandler(0);

	return main_application(argc, argv, pVh);
}
