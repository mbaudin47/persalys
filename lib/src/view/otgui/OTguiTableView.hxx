// OTguiTableView.hxx

#ifndef OTGUI_OTGUITABLEVIEW_H
#define OTGUI_OTGUITABLEVIEW_H

#include "otgui/OTguiTableModel.hxx"

#include <QTableView>

namespace OTGUI {
class OTguiTableView : public QTableView
{
  Q_OBJECT

public:
  OTguiTableView(QWidget *parent=0);
  OTguiTableView(const OT::NumericalSample & sample, QWidget *parent=0);

public slots:
  void contextMenu(const QPoint & pos);
  void exportData();

private:
  OTguiTableModel * model_;
};
}
#endif