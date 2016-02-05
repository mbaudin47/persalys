// OTguiSubWindow.hxx

#ifndef OTGUISUBWINDOW_H
#define OTGUISUBWINDOW_H

#include <QStandardItem>
#include <QMdiSubWindow>
#include <QLabel>

namespace OTGUI {
class OTguiSubWindow : public QMdiSubWindow
{
  Q_OBJECT

public:
  OTguiSubWindow(QStandardItem * item);

  QStandardItem * getItem()const;

  QString getErrorMessage() const;
  void setErrorMessage(QString message);

public slots:
signals:
  void errorMessageChanged(QString);

protected:
  QLabel * errorMessageLabel_;

private:
  QStandardItem * item_;
  QString errorMessage_;
};
}
#endif