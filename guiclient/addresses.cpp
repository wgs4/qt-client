/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2017 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "addresses.h"

#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <QSqlError>
#include <QVariant>
#include <QToolBar>
#include <QToolButton>

#include <openreports.h>
#include <metasql.h>

#include "addresses.h"
#include "address.h"
#include "storedProcErrorLookup.h"
#include "parameterwidget.h"
#include "errorReporter.h"

addresses::addresses(QWidget* parent, const char*, Qt::WindowFlags fl)
  : display(parent, "addresses", fl)
{
  setReportName("AddressesMasterList");
  setWindowTitle(tr("Addresses"));
  setMetaSQLOptions("addresses", "detail");
  setNewVisible(true);
  setQueryOnStartEnabled(true);
  setParameterWidgetVisible(true);

  _crmacctid = -1;
  _detachAct = 0;

  parameterWidget()->append(tr("Account"), "crmacct_id", ParameterWidget::Crmacct);
  parameterWidget()->append(tr("Address"), "address", ParameterWidget::Text);
  parameterWidget()->append(tr("City"), "city", ParameterWidget::Text);
  parameterWidget()->append(tr("State"), "state", ParameterWidget::Text);
  parameterWidget()->appendComboBox(tr("Country"), "country", XComboBox::LocaleCountries);
  parameterWidget()->append(tr("Postal Code"), "postalcode", ParameterWidget::Text);
  parameterWidget()->append(tr("Show Inactive"), "showInactive", ParameterWidget::Exists);

  list()->addColumn(tr("Line 1"),	 -1, Qt::AlignLeft, true, "addr_line1");
  list()->addColumn(tr("Line 2"),	 150, Qt::AlignLeft, true, "addr_line2");
  list()->addColumn(tr("Line 3"),	 150, Qt::AlignLeft, true, "addr_line3");
  list()->addColumn(tr("City"),	         150, Qt::AlignLeft, true, "addr_city");
  list()->addColumn(tr("State"),	 75, Qt::AlignLeft, true, "addr_state");
  list()->addColumn(tr("Country"),	 150, Qt::AlignLeft, true, "addr_country");
  list()->addColumn(tr("Postal Code"),   75,Qt::AlignLeft, true, "addr_postalcode");
  list()->addColumn(tr("CRM Account(s)"), 150, Qt::AlignLeft, true, "crmacct");

  setupCharacteristics("ADDR");

  QToolButton * detachBtn = new QToolButton(this);
  detachBtn->setText(tr("Detach"));
  _detachAct = toolBar()->insertWidget(filterSeparator(), detachBtn);
  _detachAct->setEnabled(false);
  _detachAct->setVisible(false);

  if (_privileges->check("MaintainAddresses"))
  {
    connect(list(), SIGNAL(itemSelected(int)), this, SLOT(sEdit()));
    connect(list(), SIGNAL(valid(bool)), _detachAct, SLOT(setEnabled(bool)));
  }
  else
  {
    newAction()->setEnabled(false);
    connect(list(), SIGNAL(itemSelected(int)), this, SLOT(sView()));
  }

}

void addresses::sPopulateMenu(QMenu *pMenu, QTreeWidgetItem*, int)
{
  QAction *menuItem;

  menuItem = pMenu->addAction(tr("Edit..."), this, SLOT(sEdit()));
  menuItem->setEnabled(_privileges->check("MaintainAddresses"));

  menuItem = pMenu->addAction(tr("View..."), this, SLOT(sView()));
  menuItem->setEnabled(_privileges->check("ViewAddresses"));

  menuItem = pMenu->addAction(tr("Delete"), this, SLOT(sDelete()));
  menuItem->setEnabled(_privileges->check("MaintainAddresses"));
}

enum SetResponse addresses::set(const ParameterList& pParams)
{
  XWidget::set(pParams);
  QVariant param;
  bool	   valid;
  
  param = pParams.value("showRole", &valid);
  if (valid)
  {
    list()->addColumn(tr("Role"), 80, Qt::AlignLeft, true, "crmrole");
    list()->moveColumn(list()->column("crmrole"), 0);
  }

  sFillList();
  return NoError;
}

int addresses::crmacctId()
{
  return _crmacctid;
}

void addresses::sNew()
{
  ParameterList params;
  params.append("mode", "new");

  address newdlg(this, "", true);
  newdlg.set(params);

  if (newdlg.exec() != XDialog::Rejected)
    sFillList();
}

void addresses::sEdit()
{
  ParameterList params;
  params.append("mode", "edit");
  params.append("addr_id", list()->id());

  address newdlg(this, "", true);
  newdlg.set(params);

  if (newdlg.exec() != XDialog::Rejected)
    sFillList();
}

void addresses::sView()
{
  ParameterList params;
  params.append("mode", "view");
  params.append("addr_id", list()->id());

  address newdlg(this, "", true);
  newdlg.set(params);
  newdlg.exec();
}

void addresses::sDetach()
{
  int answer = QMessageBox::question(this, tr("Detach Address?"),
			tr("<p>Are you sure you want to detach this Address "
			   "from this Account?"),
			QMessageBox::Yes, QMessageBox::No | QMessageBox::Default);
  if (answer == QMessageBox::Yes)
  {
    int cntctId = list()->id();
    XSqlQuery detq;
    detq.prepare("DELETE FROM  crmacctaddrass "
                 "WHERE crmacctaddrass_crmacct_id=:crmacct_id "
                 " AND crmacctaddrass_addr_id=:addr_id ");
    detq.bindValue(":addr_id", list()->id());
    detq.bindValue(":crmacct_id", _crmacctid);
    detq.exec();
    if (detq.first())
    {
      int returnVal = detq.value("returnVal").toInt();
      if (returnVal < 0)
      {
        ErrorReporter::error(QtCriticalMsg, this, tr("Error detaching Address from Account (%1)")
                        .arg(returnVal),detq, __FILE__, __LINE__);
        return;
      }
      emit cntctDetached(cntctId);
    }
    else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Detaching Address"),
                                  detq, __FILE__, __LINE__))
      return;

    sFillList();
  }
}

QAction* addresses::detachAction()
{
  return _detachAct;
}

void addresses::setCrmacctid(int crmacctId)
{
  _crmacctid = crmacctId;
  if (_crmacctid == -1)
  {
    parameterWidget()->setDefault(tr("Account"), QVariant(), true);
    _detachAct->setVisible(false);
  }
  else
  {
    parameterWidget()->setDefault(tr("Account"), _crmacctid, true);
    setNewVisible(false);
    _detachAct->setVisible(true);
    list()->hideColumn(list()->column("crmacct"));
  }
}

void addresses::sDelete()
{
  XSqlQuery deleteAddress;
  deleteAddress.prepare("SELECT deleteAddress(:addr_id) AS result;");
  deleteAddress.bindValue(":addr_id", list()->id());
  deleteAddress.exec();
  if (deleteAddress.first())
  {
    int result = deleteAddress.value("result").toInt();
    if (result < 0)
    {
      QMessageBox::warning(this, tr("Cannot Delete Selected Address"),
			   storedProcErrorLookup("deleteAddress", result));
      return;
    }
    else
      sFillList();
  }
  else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Deleting Address"),
                                deleteAddress, __FILE__, __LINE__))
  {
    return;
  }
}
