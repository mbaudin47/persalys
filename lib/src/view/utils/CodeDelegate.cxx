//                                               -*- C++ -*-
/**
 *  @brief QItemDelegate to define Python script
 *
 *  Copyright 2015-2019 EDF-Phimeca
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "otgui/CodeDelegate.hxx"

#include <QPlainTextEdit>
#include <QApplication>
#include <QPainter>
#include <QKeyEvent>
#include <QTextCursor>
#include <QScrollBar>

#include <iostream>

namespace OTGUI
{

// CodeEditor
CodeEditor::CodeEditor(QWidget * parent)
  : QPlainTextEdit(parent)
  , verticalScrollBarValue_(0)
  , horizontalScrollBarValue_(0)
{
#ifndef _WIN32
  QFont font("Monospace");
#else
  QFont font("Courier");
#endif
  font.setPointSize(9);
  font.setFixedPitch(true);
  setWordWrapMode(QTextOption::NoWrap);
  setFont(font);

  connect(verticalScrollBar(), SIGNAL(actionTriggered(int)), this, SLOT(updateVerticalScrollBarValue()));
  connect(horizontalScrollBar(), SIGNAL(actionTriggered(int)), this, SLOT(updateHorizontalScrollBarValue()));
}


void CodeEditor::keyPressEvent(QKeyEvent *e)
{
  if (e->key() == Qt::Key_Tab)
  {
    QTextCursor cursor(textCursor());
    cursor.insertText("    ");
    return;
  }

  QPlainTextEdit::keyPressEvent(e);

  if (e->key() == Qt::Key_Return)
  {
    int position = textCursor().position();
    emit codeEdited(this);
    QTextCursor cursor(textCursor());
    cursor.setPosition(position);
    setTextCursor(cursor);
  }
  // update scrollBar Values
  updateVerticalScrollBarValue();
  updateHorizontalScrollBarValue();
}


void CodeEditor::focusOutEvent(QFocusEvent *event)
{
  QPlainTextEdit::focusOutEvent(event);

  verticalScrollBar()->setValue(verticalScrollBarValue_);
  horizontalScrollBar()->setValue(horizontalScrollBarValue_);
}


void CodeEditor::updateVerticalScrollBarValue()
{
  verticalScrollBarValue_ = verticalScrollBar()->sliderPosition();
}


void CodeEditor::updateHorizontalScrollBarValue()
{
  horizontalScrollBarValue_ = horizontalScrollBar()->sliderPosition();
}


// CodeDelegate
bool CodeDelegate::eventFilter(QObject *obj, QEvent *event)
{
  if (event->type() == QEvent::KeyPress)
  {
    QKeyEvent * keyEvent = dynamic_cast<QKeyEvent*>(event);
    if (keyEvent->key() == Qt::Key_Tab)
    {
      return false;
    }
  }
  return QItemDelegate::eventFilter(obj, event);
}


CodeDelegate::CodeDelegate(QObject * parent)
  : QItemDelegate(parent)
{
}


QWidget *CodeDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  CodeEditor * textEdit = new CodeEditor(parent);
  connect(textEdit, SIGNAL(codeEdited(QWidget *)), this, SIGNAL(commitData(QWidget *)));
  return textEdit;
}


void CodeDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
  CodeEditor * textEdit = dynamic_cast<CodeEditor*>(editor);
  textEdit->setPlainText(index.model()->data(index, Qt::DisplayRole).toString());
}


void CodeDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
  QPlainTextEdit * textEdit = dynamic_cast<QPlainTextEdit*>(editor);
  model->setData(index, textEdit->toPlainText(), Qt::EditRole);
}


void CodeDelegate::paint(QPainter *painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  QVariant value = index.data(Qt::DisplayRole);
  if (value.isValid() && !value.isNull())
  {
    QTextDocument document;
#ifndef _WIN32
    QFont font("Monospace");
#else
    QFont font("Courier");
#endif
    font.setPointSize(9);
    font.setFixedPitch(true);
    document.setDefaultFont(font);
    document.setDocumentMargin(2);
    document.setPlainText(value.toString());
    painter->translate(option.rect.topLeft());
    document.drawContents(painter);
    painter->translate(-option.rect.topLeft());
  }
}
}
