/* Copyright (C) 2009, Mikkel Krautz <mikkel@krautz.dk>

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

#ifndef _CONFIGDIALOG_DELEGATE
#define _CONFIGDIALOG_DELEGATE

#include "ConfigDialog.h"
#include "ConfigWidget.h"
#import <Cocoa/Cocoa.h>

@class NSToolbarItem;

@interface ConfigDialogDelegate : NSObject {
    ConfigDialog *configDialog;
    QMap<unsigned int, ConfigWidget *> *widgetMap;

    BOOL inExpertMode;
    NSMutableDictionary *nameWidgetMapping;
    NSMutableArray *identifiers;
    NSToolbar *toolbarCache;
}

- (id) initWithConfigDialog:
(ConfigDialog *)dialog andWidgetMap:
(QMap<unsigned int, ConfigWidget *> *) map inExpertMode:
(BOOL)flag;

- (NSArray *) toolbarAllowedItemIdentifiers:
(NSToolbar *)toolbar;
- (NSArray *) toolbarDefaultItemIdentifiers:
(NSToolbar *)toolbar;
- (NSArray *) toolbarSelectableItemIdentifiers:
(NSToolbar *)toolbar;

- (void) toolbarWillAddItem:
(NSNotification*)notification;
- (void) toolbarDidRemoveItem:
(NSNotification *)notification;

- (NSToolbarItem *) toolbar:
(NSToolbar *)toolbar itemForItemIdentifier:
(NSString *)identifier willBeInsertedIntoToolbar:
(BOOL)flag;

- (BOOL) validateToolbarItem:
(NSToolbarItem *)toolbarItem;

- (void) selectItem:
(ConfigWidget *)cw;
- (void) itemSelected:
(NSToolbarItem *)toolbarItem;

- (void) expertSelected:
(NSButton *)button;
- (BOOL) expertMode;

- (void) dealloc;

@end

#endif /* _CONFIGDIALOG_DELEGATE */
