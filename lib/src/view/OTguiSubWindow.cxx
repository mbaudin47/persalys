// OTguiSubWindow.cxx

#include "otgui/OTguiSubWindow.hxx"

namespace OTGUI {

OTguiSubWindow::OTguiSubWindow(QStandardItem * item)
 : QMdiSubWindow()
 , item_(item)
{
}


QStandardItem * OTguiSubWindow::getItem()const
{
  return item_;
}


QString OTguiSubWindow::getErrorMessage() const
{
  return errorMessage_;
}


void OTguiSubWindow::setErrorMessage(QString message)
{
  errorMessage_ = message;
  emit errorMessageChanged(message);
}
}