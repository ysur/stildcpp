/***************************************************************************
 *   Copyright (C) 2007 by Pavel Andreev                                   *
 *   Mail: apavelm on gmail dot com (apavelm@gmail.com)                    *
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

#ifndef __FAV_USR_H__
#define __FAV_USR_H__

//
#include "client/stdinc.h"
#include "client/DCPlusPlus.h"
#include "client/User.h"
#include "client/FavoriteManagerListener.h"
#include "client/FavoriteManager.h"
#include "client/FavoriteUser.h"

//
#include "UserInfoBase.h"
#include "mdi_c.h"
#include "stilutils.h"
#include "hub_win.h"

#include "ui_fav_users.h"

#include <QtDebug>
#include <QtGui>

class FavoriteUsersWindow : public MdiChild, private Ui::mdi_FavUsers, private dcpp::FavoriteManagerListener
{
	Q_OBJECT
public:
	FavoriteUsersWindow(QWidget *parent);
	~FavoriteUsersWindow();
private:
	
	enum {
		COLUMN_FIRST,
		COLUMN_NICK = COLUMN_FIRST,
		COLUMN_HUB,
		COLUMN_SEEN,
		COLUMN_DESCRIPTION,
		COLUMN_CID,
		COLUMN_LAST
	};
	
	class UserInfo : public UserInfoBase {
	public:
		UserInfo(const FavoriteUser& u);
		
		const tstring& getText(int col) const {
			return columns[col];
		}

		int getImage() const {
			return 0;
		}

		static int compareItems(UserInfo* a, UserInfo* b, int col) 
		{
			return wcscmp(a->columns[col].c_str(), b->columns[col].c_str());
		}

		void remove();

		void update(const FavoriteUser& u);

		tstring columns[COLUMN_LAST];
	};
	
	QList<FavoriteUser> datalist;
	FavoriteUser & GetFavUser(const QString & );
	int GetFavUserIndex(const QString & );
	
	enum { USER_UPDATED };

	void addUser(const dcpp::FavoriteUser& aUser);
	void updateUser(const dcpp::UserPtr& aUser);
	void removeUser(const dcpp::FavoriteUser& aUser);
	
	// hack
	void speak(int, const UserPtr& aUser);
	
	// FavoriteManagerListener
	virtual void on(UserAdded, const FavoriteUser& aUser) throw();
	virtual void on(UserRemoved, const FavoriteUser& aUser) throw();
	virtual void on(StatusChanged, const UserPtr& aUser) throw();
private slots:
	void onAutoGrant(int);
	void slotSpeak(int, const UserPtr&);
	void on_list_currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *);
	void slot_remove_user();
	void slot_ignore_user();
	void slot_desc_user();
signals:
	void sigSpeak(int, const UserPtr&);
};

#endif // __FAV_USR_H__
