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


#ifndef __ADLSEARCH_H__
#define __ADLSEARCH_H__

#include "mdi_c.h"
#include "config.h"
#include "stilutils.h"

#include "adlprop.h"

#include "client/ADLSearch.h"
#include "client/Client.h"

#include "ui_adl.h"

#include <QtGui>
#include <QtCore>

using namespace std;
using namespace dcpp;

class ADLSearchWindow : public MdiChild, private Ui::mdi_adl
{
	Q_OBJECT
public:
	ADLSearchWindow(QWidget *parent);
	~ADLSearchWindow();
	
	enum Status {
		STATUS_STATUS,
		STATUS_LAST
	};
	
private:
	QList<ADLSearch *> datalist;
	QList<QTreeWidgetItem *> datalistitem;
	QMenu * columnMenu;
	QMenu * cnxtMenu;
	
	enum {
		COLUMN_FIRST,
		COLUMN_ACTIVE_SEARCH_STRING = COLUMN_FIRST,
		COLUMN_SOURCE_TYPE,
		COLUMN_DEST_DIR,
		COLUMN_MIN_FILE_SIZE,
		COLUMN_MAX_FILE_SIZE,
		COLUMN_LAST
	};

	static int columnSizes[COLUMN_LAST];
	
	void addEntry(ADLSearch& , int index = -1);
	
private slots:
	void slotAdd();
	void slotRemove();
	void slotProp();
	
	void chooseColumn(QAction *action);
	void showColumnMenu(const QPoint &point);
	void showCnxtMenu(const QPoint& point);
};

#endif // __ADLSEARCH_H__
