// AnalysisItem.hxx

#ifndef ANALYSISITEM_H
#define ANALYSISITEM_H

#include "otgui/Item.hxx"
#include "otgui/Analysis.hxx"

namespace OTGUI {
class AnalysisItem : public Item
{
  Q_OBJECT

public:
  AnalysisItem(const Analysis & analysis, const QString & typeAnalysis=QString("Analysis"));

  virtual ~AnalysisItem();

  void setData(const QVariant & value, int role);

  Analysis getAnalysis() const;

public slots:
  void updateAnalysis(const Analysis & analysis);
signals:
  void analysisFinished(AnalysisItem*);

private:
  Analysis analysis_;
};
}
#endif