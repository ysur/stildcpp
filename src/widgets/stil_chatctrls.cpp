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

#include <QAbstractTextDocumentLayout>
#include <QAction>
#include <QApplication>
#include <QDesktopWidget>
#include <QEvent>
#include <QKeyEvent>
#include <QLayout>
#include <QMenu>
#include <QResizeEvent>
#include <QScrollBar>
#include <QTextCharFormat>
#include <QTextDocument>
#include <QTimer>

#include "stil_chatctrls.h"
//#include "common.h"
//#include "shortcutmanager.h"
//#include "spellhighlighter.h"
//#include "spellchecker.h"
//#include "psioptions.h"

//----------------------------------------------------------------------------
// ChatView
//----------------------------------------------------------------------------
ChatView::ChatView(QWidget *parent) : StilTextView(parent) , dialog_(0)
{
	setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);

	setReadOnly(true);
	setUndoRedoEnabled(false);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

#ifndef Q_WS_X11 // linux has this feature built-in
	connect(this, SIGNAL(selectionChanged()), SLOT(autoCopy()));
	connect(this, SIGNAL(cursorPositionChanged()), SLOT(autoCopy()));
#endif
}

ChatView::~ChatView()
{
}

void ChatView::setDialog(QWidget* dialog)
{
	dialog_ = dialog;
}

QSize ChatView::sizeHint() const
{
	return minimumSizeHint();
}

bool ChatView::focusNextPrevChild(bool next)
{
	return QWidget::focusNextPrevChild(next);
}

void ChatView::keyPressEvent(QKeyEvent *e)
{
	if(dialog_) {
		QKeySequence k(e->key() + (e->modifiers() & ~Qt::KeypadModifier));

		// Temporary workaround for what i think is a Qt bug
//		if(ShortcutManager::instance()->shortcuts("common.close").contains(k)
//			|| ShortcutManager::instance()->shortcuts("message.send").contains(k)) 
		{
			e->ignore();
			return;
		}

		// Ignore registered key sequences (and pass them up)
		foreach(QAction* act, dialog_->actions()) {
			foreach(QKeySequence keyseq, act->shortcuts()) {
				if(!keyseq.isEmpty() && keyseq.matches(k) == QKeySequence::ExactMatch) {
					e->ignore();
					//act->trigger();
					return;
				}
			}
		}
	}

/*	if(e->key() == Qt::Key_Escape)
		e->ignore(); 
#ifdef Q_WS_MAC
	else if(e->key() == Qt::Key_W && e->modifiers() & Qt::ControlModifier)
		e->ignore();
	else
#endif
	else if(e->key() == Qt::Key_Return && ((e->modifiers() & Qt::ControlModifier) || (e->modifiers() & Qt::AltModifier)) )
		e->ignore();
	else if(e->key() == Qt::Key_H && (e->modifiers() & Qt::ControlModifier))
		e->ignore();
	else if(e->key() == Qt::Key_I && (e->modifiers() & Qt::ControlModifier))
		e->ignore(); */
	/*else*/ 
						//if(e->key() == Qt::Key_M && (e->modifiers() & Qt::ControlModifier) && !isReadOnly()) // newline 
							//insert("\n");
/*	else if(e->key() == Qt::Key_U && (e->modifiers() & Qt::ControlModifier) && !isReadOnly())
		setText(""); */
	else
		StilTextView::keyPressEvent(e);
}

/**
 * Copies any selected text to the clipboard
 * if autoCopy is enabled and ChatView is in read-only mode.
 */
void ChatView::autoCopy()
{
	if (isReadOnly()) {
		copy();
	}
}

/**
 * Handle KeyPress events that happen in ChatEdit widget. This is used
 * to 'fix' the copy shortcut.
 * \param object object that should receive the event
 * \param event received event
 * \param chatEdit pointer to the dialog's ChatEdit widget that receives user input
 */
bool ChatView::handleCopyEvent(QObject *object, QEvent *event, ChatEdit *chatEdit)
{
	if (object == chatEdit && event->type() == QEvent::KeyPress) {
		QKeyEvent *e = (QKeyEvent *)event;
		if ((e->key() == Qt::Key_C && (e->modifiers() & Qt::ControlModifier)) ||
			(e->key() == Qt::Key_Insert && (e->modifiers() & Qt::ControlModifier)))
		{
			if (!chatEdit->textCursor().hasSelection() &&
				this->textCursor().hasSelection()) 
			{
				this->copy();
				return true;
			}
		}
	}
	
	return false;
}

void ChatView::appendText(const QString &text)
{
	bool doScrollToBottom = atBottom();
	
	// prevent scrolling back to selected text when 
	// restoring selection
	int scrollbarValue = verticalScrollBar()->value();
	
	StilTextView::appendText(text);
	
	if (doScrollToBottom)
		scrollToBottom();
	else
		verticalScrollBar()->setValue(scrollbarValue);
}

QString ChatView::formatTimeStamp(const QDateTime &time)
{
	return time.toString("hh:mm:ss"); //FIXME: replace "hh:mm:ss" user-defined string.
}

//----------------------------------------------------------------------------
// ChatEdit
//----------------------------------------------------------------------------
ChatEdit::ChatEdit(QWidget *parent) : QTextEdit(parent), dialog_(0)
{
	setWordWrapMode(QTextOption::WordWrap);
	setAcceptRichText(false);

	setReadOnly(false);
	setUndoRedoEnabled(true);

	setMinimumHeight(48);

	previous_position_ = 0;
	//setCheckSpelling(checkSpellingGloballyEnabled());
	//connect(PsiOptions::instance(),SIGNAL(optionChanged(const QString&)),SLOT(optionsChanged()));
}

ChatEdit::~ChatEdit()
{

}

void ChatEdit::setDialog(QWidget* dialog)
{
	dialog_ = dialog;
}

QSize ChatEdit::sizeHint() const
{
	return minimumSizeHint();
}

bool ChatEdit::focusNextPrevChild(bool next)
{
	return QWidget::focusNextPrevChild(next);
}

void ChatEdit::keyPressEvent(QKeyEvent *e)
{
	if(dialog_) {
		QKeySequence k(e->key() + (e->modifiers() & ~Qt::KeypadModifier));
		// Temporary workaround for what i think is a Qt bug
//		if(ShortcutManager::instance()->shortcuts("common.close").contains(k)
//			|| ShortcutManager::instance()->shortcuts("chat.send").contains(k)) 
		{
			e->ignore();
			return;
		}

		// Ignore registered key sequences (and pass them up)
		foreach(QAction* act, dialog_->actions()) {
			foreach(QKeySequence keyseq, act->shortcuts()) {
				if(!keyseq.isEmpty() && keyseq.matches(k) == QKeySequence::ExactMatch) {
					e->ignore();
					//act->trigger();
					return;
				}
			}
		}
	}
/*	if(e->key() == Qt::Key_Escape || (e->key() == Qt::Key_W && e->modifiers() & Qt::ControlModifier))
		e->ignore();
	else if(e->key() == Qt::Key_Return && 
	       ((e->modifiers() & Qt::ControlModifier) 
#ifndef Q_WS_MAC
	       || (e->modifiers() & Qt::AltModifier) 
#endif
	       ))
		e->ignore();
	else if(e->key() == Qt::Key_M && (e->modifiers() & Qt::ControlModifier)) // newline
		insert("\n");
	else if(e->key() == Qt::Key_H && (e->modifiers() & Qt::ControlModifier)) // history
		e->ignore();
	else  if(e->key() == Qt::Key_S && (e->modifiers() & Qt::AltModifier))
		e->ignore();
	else*/ if(e->key() == Qt::Key_U && (e->modifiers() & Qt::ControlModifier))
		setText("");
/*	else if((e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) && !((e->modifiers() & Qt::ShiftModifier) || (e->modifiers() & Qt::AltModifier)) && option.chatSoftReturn)
		e->ignore();
	else if((e->key() == Qt::Key_PageUp || e->key() == Qt::Key_PageDown) && (e->modifiers() & Qt::ShiftModifier))
		e->ignore();
	else if((e->key() == Qt::Key_PageUp || e->key() == Qt::Key_PageDown) && (e->modifiers() & Qt::ControlModifier))
		e->ignore(); */
#ifdef Q_WS_MAC
	else if (e->key() == Qt::Key_QuoteLeft && e->modifiers() == Qt::ControlModifier) {
		e->ignore();
	}
#endif
	else
	{
		QTextEdit::keyPressEvent(e);
	}
}

/**
 * Work around Qt bug, that QTextEdit doesn't accept() the 
 * event, so it could result in another context menu popping
 * out after the first one.
 */
void ChatEdit::contextMenuEvent(QContextMenuEvent *e) 
{
	last_click_ = e->pos();
	// Do normal menu
	QTextEdit::contextMenuEvent(e);
	e->accept();
}

/*!
 * \brief handles a click on a suggestion
 * \param the action is just the container which holds the suggestion.
 * 
 * This method is called by the framework whenever a user clicked on the child popupmenu
 * to select a suggestion for a missspelled word. It exchanges the missspelled word with the
 * suggestion which is the text of the QAction parameter.
 */
void ChatEdit::applySuggestion()
{
	QAction* act_suggestion = (QAction*) sender();
	int current_position = textCursor().position();
	
	// Replace the word
	QTextCursor	tc = cursorForPosition(last_click_);
	tc.movePosition(QTextCursor::StartOfWord, QTextCursor::MoveAnchor);
	tc.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
	int old_length = tc.position() - tc.anchor();
	tc.insertText(act_suggestion->text());
	tc.clearSelection();

	// Put the cursor where it belongs
	int new_length = act_suggestion->text().length();
	tc.setPosition(current_position - old_length + new_length);
	setTextCursor(tc);
}

/*!
 * \brief handles a click on the add2dict action of the parent popupmenu
 * \param Never used bool parameter
 * 
 * The method sets the cursor to the last mouseclick position and looks for the word which is placed there.
 * This word is than added to the dictionary of aspell.
 */
void ChatEdit::addToDictionary()
{
	QTextCursor	tc = cursorForPosition(last_click_);
	int current_position = textCursor().position();

	// Get the selected word
	tc.movePosition(QTextCursor::StartOfWord, QTextCursor::MoveAnchor);
	tc.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
	//SpellChecker::instance()->add(tc.selectedText());
	
	// Put the cursor where it belongs
	tc.clearSelection();
	tc.setPosition(current_position);
	setTextCursor(tc);
}

void ChatEdit::optionsChanged()
{
	
}

//----------------------------------------------------------------------------
// LineEdit
//----------------------------------------------------------------------------
LineEdit::LineEdit( QWidget *parent)
	: ChatEdit(parent)
{
	setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere); // no need for horizontal scrollbar with this
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	setMinimumHeight(0);

	connect(this, SIGNAL(textChanged()), SLOT(recalculateSize()));
}

LineEdit::~LineEdit()
{
}

QSize LineEdit::minimumSizeHint() const
{
	QSize sh = QTextEdit::minimumSizeHint();
	sh.setHeight(fontMetrics().height() + 1);
	sh += QSize(0, QFrame::lineWidth() * 2);
	return sh;
}

QSize LineEdit::sizeHint() const
{
	QSize sh = QTextEdit::sizeHint();
	sh.setHeight(int(document()->documentLayout()->documentSize().height())+6); //FIXME: "6" - Magic Number
	sh += QSize(0, QFrame::lineWidth() * 2);
	((QTextEdit*)this)->setMaximumHeight(sh.height());
	return sh;
}

void LineEdit::resizeEvent(QResizeEvent* e)
{
	ChatEdit::resizeEvent(e);
}
 
void LineEdit::recalculateSize()
{
	updateGeometry();
}

void LineEdit::updateScrollBar()
{
	setVerticalScrollBarPolicy(sizeHint().height() > height() ? Qt::ScrollBarAlwaysOn : Qt::ScrollBarAlwaysOff);
	ensureCursorVisible();
}