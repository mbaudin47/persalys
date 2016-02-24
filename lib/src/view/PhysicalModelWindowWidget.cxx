//                                               -*- C++ -*-
/**
 *  @brief QWidget to visualize the variables of the physical model
 *
 *  Copyright 2015-2016 EDF
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
 *  You should have received a copy of the GNU Lesser General Public
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "otgui/PhysicalModelWindowWidget.hxx"

#include "otgui/AnalyticalPhysicalModel.hxx"
#include "otgui/ModelEvaluation.hxx"
#include "otgui/LineEditWithQValidatorDelegate.hxx"

#include <QFileDialog>
#include <QComboBox>
#include <QHeaderView>

using namespace OT;

namespace OTGUI {

PhysicalModelWindowWidget::PhysicalModelWindowWidget(PhysicalModelItem * item)
  : QWidget()
  , physicalModel_(item->getPhysicalModel())
  , inputTableView_(0)
  , inputTableModel_(0)
  , addInputLineButton_(0)
  , removeInputLineButton_(0)
  , outputTableView_(0)
  , outputTableModel_(0)
  , addOutputLineButton_(0)
  , removeOutputLineButton_(0)
  , evaluateOutputsButton_(0)
{
  connect(item, SIGNAL(outputChanged()), this, SLOT(updateOutputTableModel()));
  connect(item, SIGNAL(inputChanged()), this, SLOT(updateInputTableModel()));
  buildInterface();
}


void PhysicalModelWindowWidget::buildInterface()
{
  QVBoxLayout * vbox = new QVBoxLayout(this);
  // Define Inputs
  QGroupBox * inputsBox = new QGroupBox(tr("Inputs"));
  QVBoxLayout * inputsLayout = new QVBoxLayout(inputsBox);

  inputTableView_ = new QTableView;
  inputTableView_->setEditTriggers(QTableView::AllEditTriggers);
  LineEditWithQValidatorDelegate * delegate = new LineEditWithQValidatorDelegate;
  inputTableView_->setItemDelegateForColumn(0, delegate);
  inputsLayout->addWidget(inputTableView_);

  if (physicalModel_.getImplementation()->getClassName() == "AnalyticalPhysicalModel")
  {
    addInputLineButton_ = new QPushButton(QIcon(":/images/list-add.png"), tr("Add"));
    addInputLineButton_->setToolTip(tr("Add an input"));
    connect(addInputLineButton_, SIGNAL(clicked(bool)), this, SLOT(addInputLine()));

    removeInputLineButton_ = new QPushButton(QIcon(":/images/list-remove.png"), tr("Remove"));
    removeInputLineButton_->setToolTip(tr("Remove the selected input"));
    connect(removeInputLineButton_, SIGNAL(clicked(bool)), this, SLOT(removeInputLine()));

    QHBoxLayout * buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(addInputLineButton_);
    buttonsLayout->addWidget(removeInputLineButton_);
    inputsLayout->addLayout(buttonsLayout);
  }

  vbox->addWidget(inputsBox);

  // Define Outputs
  QGroupBox * outputsBox = new QGroupBox(tr("Outputs"));
  QVBoxLayout * outputsLayout = new QVBoxLayout(outputsBox);

  outputTableView_ = new QTableView;
  outputTableView_->setEditTriggers(QTableView::AllEditTriggers);
  delegate = new LineEditWithQValidatorDelegate;
  outputTableView_->setItemDelegateForColumn(0, delegate);
  outputsLayout->addWidget(outputTableView_);

  QHBoxLayout * outputButtonsLayout = new QHBoxLayout;
  outputButtonsLayout->addStretch();

  if (physicalModel_.getImplementation()->getClassName() == "AnalyticalPhysicalModel")
  {
    addOutputLineButton_ = new QPushButton(QIcon(":/images/list-add.png"), tr("Add"));
    addOutputLineButton_->setToolTip(tr("Add an output"));
    connect(addOutputLineButton_, SIGNAL(clicked(bool)), this, SLOT(addOutputLine()));

    removeOutputLineButton_ = new QPushButton(QIcon(":/images/list-remove.png"), tr("Remove"));
    removeOutputLineButton_->setToolTip(tr("Remove the selected output"));
    connect(removeOutputLineButton_, SIGNAL(clicked(bool)), this, SLOT(removeOutputLine()));

    outputButtonsLayout->addWidget(addOutputLineButton_);
    outputButtonsLayout->addWidget(removeOutputLineButton_);
  }

  evaluateOutputsButton_ = new QPushButton(tr("Evaluate"));
  evaluateOutputsButton_->setToolTip(tr("Evaluate the value of the outputs"));
  connect(evaluateOutputsButton_, SIGNAL(clicked(bool)), this, SLOT(evaluateOutputs()));
  outputButtonsLayout->addWidget(evaluateOutputsButton_);

  outputsLayout->addLayout(outputButtonsLayout);

  vbox->addWidget(outputsBox);

  updateInputTableModel();
  updateOutputTableModel();
}


void PhysicalModelWindowWidget::updateInputTableModel()
{
  if (inputTableModel_)
    delete inputTableModel_;
  inputTableModel_ = new InputTableModel(physicalModel_);
  inputTableView_->setModel(inputTableModel_);
  connect(inputTableModel_, SIGNAL(errorMessageChanged(QString)), this, SIGNAL(errorMessageChanged(QString)));
}


void PhysicalModelWindowWidget::updateOutputTableModel()
{
  if (outputTableModel_)
    delete outputTableModel_;
  outputTableModel_ = new OutputTableModel(physicalModel_);
  outputTableView_->setModel(outputTableModel_);
  if (physicalModel_.getImplementation()->getClassName() != "AnalyticalPhysicalModel")
    outputTableView_->setColumnHidden(2, true);
  connect(outputTableModel_, SIGNAL(errorMessageChanged(QString)), this, SIGNAL(errorMessageChanged(QString)));
}


void PhysicalModelWindowWidget::addInputLine()
{
  inputTableModel_->addLine();
}


void PhysicalModelWindowWidget::addOutputLine()
{
  outputTableModel_->addLine();
}


void PhysicalModelWindowWidget::removeInputLine()
{
  if (inputTableView_->selectionModel()->hasSelection())
  {
    QModelIndex index = inputTableView_->selectionModel()->currentIndex();
    inputTableModel_->removeLine(index);
    int lastRow = inputTableModel_->rowCount()-1;

    if (lastRow+1)
      inputTableView_->selectRow(lastRow);
  }
}


void PhysicalModelWindowWidget::removeOutputLine()
{
  if (outputTableView_->selectionModel()->hasSelection())
    outputTableModel_->removeLine(outputTableView_->selectionModel()->currentIndex());
}


void PhysicalModelWindowWidget::evaluateOutputs()
{
  try
  {
    ModelEvaluation eval("anEval", physicalModel_);
    eval.run();
    NumericalSample outputSample(eval.getResult().getOutputSample());
    for (UnsignedInteger i = 0; i < outputSample.getDimension(); ++ i)
      physicalModel_.setOutputValue(physicalModel_.getOutputNames()[i], outputSample[0][i]);

    errorMessageChanged("");
  }
  catch (std::exception & ex)
  {
    errorMessageChanged(ex.what());
  }
}
}