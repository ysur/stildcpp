/***************************************************************************
 *   Copyright (C) 2007 - 2008 by Pavel Andreev                            *
 *   Mail: apavelm on gmail point com (apavelm@gmail.com)                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef __USERACTMENU_H__
#define __USERACTMENU_H__

#include <QMenu>
#include <QAction>
#include <QString>

#include "stilutils.h"

//
#include "client/stdinc.h"
#include "client/DCPlusPlus.h"
#include "client/Client.h"
#include "client/TaskQueue.h"
#include "client/User.h"
#include "client/FavoriteManager.h"
//

class UserActionMenu: public QMenu
{
	Q_OBJECT
public:
	QMenu * getMenuForUser(const QString &);
private:
	QString s_Nick;
	
	QAction * a_getFileList(const QString &);
	QAction * a_MatchQueue(const QString &);
	QAction * a_SendPrivateMessage(const QString &);
	QAction * a_SendPublicMessage(const QString &);
	QAction * a_AddtoFav(const QString &);
	QAction * a_GrantExtraSlot(const QString &);
	QAction * a_RemoveUserFromQueue(const QString &);
	QAction * a_CopyNickToClipboard(const QString &);
private slots:
	void sendPubM();
signals:
	void sig_sendPublicMessage(const QString &);
};

#endif // __USERACTMENU_H__
