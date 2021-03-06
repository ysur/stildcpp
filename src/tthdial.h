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

#ifndef __TTHDIAL_H__
#define __TTHDIAL_H__

#include <QDialog>
#include "stilutils.h"

#include "ui_tthdialog.h"

class TthDialog : public QDialog, private Ui::tthdlg
{ 
	Q_OBJECT
public:
	TthDialog(QWidget *parent,const QString &,const QString &,const QString &);
private:
	struct Selection { int start, length; };
	Selection saveSelection(QLineEdit &t);
	void restoreSelection(QLineEdit &t, Selection selection);
private slots:
	void fcp1();
	void fcp2();
	void fcp3();
};

#endif // __TTHDIAL_H__
