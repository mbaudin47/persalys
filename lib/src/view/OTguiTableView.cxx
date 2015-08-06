// OTguiTableView

#include "OTguiTableView.hxx"

#include <QFileDialog>
#include <QMenu>

namespace OTGUI {

OTguiTableView::OTguiTableView(OT::NumericalSample & sample)
  : QTableView()
{
  setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenu(QPoint)));
  model_ = new OTguiTableModel(sample);
  setModel(model_);
}


OTguiTableView::~OTguiTableView()
{

}


// show the context menu when right clicking
void OTguiTableView::contextMenu(const QPoint & pos)
{
  QMenu * contextMenu(new QMenu(this));
  QAction * exportData = new QAction(tr("Export"), this);
  exportData->setStatusTip(tr("Export the data"));
  connect(exportData, SIGNAL(triggered()), this, SLOT(exportData()));
  contextMenu->addAction(exportData);
  contextMenu->popup(this->mapToGlobal(pos));
}


void OTguiTableView::exportData()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Export model as..."),
                     QDir::homePath(),
                     tr("CSV source files (*.csv)"));
  if ( ! fileName.isEmpty() )
  {
    if (!fileName.endsWith(".csv"))
      fileName += ".csv";

  model_->exportData(fileName);
  }
}
}