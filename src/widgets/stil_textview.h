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

#ifndef STIL_TEXTVIEW
#define STIL_TEXTVIEW

#include <QTextEdit>
#include <QMimeData>
#include <QClipboard>
#include <QTextCursor>
#include <QMenu>
#include <QScrollBar>
#include <QAbstractTextDocumentLayout>
#include <QTextDocumentFragment>
#include <QTextFragment>
#include <QTextEdit>

#include "stil_richtext.h"



// TODO: This class should be further refactored into more modular system.
// It should allow registering/unregistering of QRegExps corresponding to
// different actions. Also class's base API should be made public.
//
// Examples of future use:
// URLObject::getInstance()::registerAction(QRegExp("^http:\/\/"), act_open_browser);

class URLObject : public QObject
{
	Q_OBJECT
protected:
	URLObject();
public:
	static URLObject *getInstance();
	
	QMenu *createPopupMenu(const QString &lnk);

public slots:

	void popupAction() { emit openURL(_link); }
	void popupAction(QString lnk) { emit openURL(lnk); }

	void popupCopy(QString lnk)
	{
		QApplication::clipboard()->setText( copyString(lnk), QClipboard::Clipboard );
		if(QApplication::clipboard()->supportsSelection())
			QApplication::clipboard()->setText( copyString(lnk), QClipboard::Selection );
	
	}

	void popupCopy() { popupCopy(_link); }
signals:
	void openURL(QString);
private:
	QString _link;
	QAction *_act_mailto, *_act_magnet, *_act_send_message, *_act_browser, *_act_copy;
	
	QString copyString(QString from)
	{
		QString l = from;

		int colon = l.indexOf(':');
		if ( colon == -1 )
			colon = 0;
		QString service = l.left( colon );

		if ( service == "mailto" || service == "jabber" || service == "jid" || service == "xmpp" || service == "atstyle") {
			if ( colon > -1 )
				l = l.mid( colon + 1 );

			while ( l[0] == '/' )
				l = l.mid( 1 );
		}

		return l;
	}
	
};

///////////////////////////////////////////////////////

class StilTextView : public QTextEdit
{
	Q_OBJECT
public:
	StilTextView(QWidget *parent = 0);

	bool atBottom();
	QString anchorOnMousePress;
	bool hadSelectionOnMousePress;

	virtual void appendText(const QString &text);

	QString getHtml() const;
	
	struct Selection {
		int start, end;
	};
	Selection saveSelection(QTextCursor &cursor);
	void restoreSelection(QTextCursor &cursor, Selection selection);
	
	//QString fragmentToPlainText(const QTextFragment &fragment);
	//QString blockToPlainText(const QTextBlock &block);
	//QString documentFragmentToPlainText(const QTextDocument &doc, QTextFrame::Iterator frameIt);
	
public slots:
	void scrollToBottom();
	void scrollToTop();
	
protected:
	// make these functions unusable, because they modify
	// document structure and we can't override them to
	// handle Icons correctly
	void append(const QString &) { }
	void toHtml() const { }
	void toPlainText() const { }
	void insertHtml(const QString &) { }
	void insertPlainText(const QString &) { }
	void setHtml(const QString &) { }
	void setPlainText(const QString &) { }

	void insertFromMimeData( const QMimeData *source );

	// reimplemented
	void contextMenuEvent(QContextMenuEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	QMimeData *createMimeDataFromSelection() const;
	void resizeEvent(QResizeEvent *);
};

#endif
