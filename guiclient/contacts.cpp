/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2017 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "contacts.h"

#include <QAction>
#include <QInputDialog>
#include <QMenu>
#include <QToolBar>
#include <QToolButton>
#include <QVariant>
#include <QMessageBox>

#include "contact.h"
#include "errorReporter.h"
#include "parameterwidget.h"
#include "prospect.h"
#include "storedProcErrorLookup.h"

contacts::contacts(QWidget* parent, const char*, Qt::WindowFlags fl)
  : display(parent, "contacts", fl)
{
  setupUi(optionsWidget());
  setWindowTitle(tr("Contacts"));
  setReportName("ContactsMasterList");
  setMetaSQLOptions("contacts", "detail");
  setParameterWidgetVisible(true);
  setNewVisible(true);
  setSearchVisible(true);
  setQueryOnStartEnabled(true);

  _crmacctid = -1;
  _attachAct = 0;
  _detachAct = 0;

  if (_privileges->check("MaintainAllContacts") || _privileges->check("ViewAllContacts"))
  {
    parameterWidget()->append(tr("Owner"), "owner_username", ParameterWidget::User);
    parameterWidget()->append(tr("Owner Pattern"), "owner_usr_pattern", ParameterWidget::Text);
  }
  parameterWidget()->appendComboBox(tr("Contact Group"), "cntctgrp", XComboBox::ContactGroups);
  parameterWidget()->append(tr("Account"), "crmacct_id", ParameterWidget::Crmacct);
  parameterWidget()->append(tr("Name Pattern"), "cntct_name_pattern", ParameterWidget::Text);
  parameterWidget()->append(tr("Phone Pattern"), "cntct_phone_pattern", ParameterWidget::Text);
  parameterWidget()->append(tr("Email Pattern"), "cntct_email_pattern", ParameterWidget::Text);
  parameterWidget()->append(tr("Street Pattern"), "addr_street_pattern", ParameterWidget::Text);
  parameterWidget()->append(tr("City Pattern"), "addr_city_pattern", ParameterWidget::Text);
  parameterWidget()->append(tr("State Pattern"), "addr_state_pattern", ParameterWidget::Text);
  parameterWidget()->append(tr("Postal Code Pattern"), "addr_postalcode_pattern", ParameterWidget::Text);
  parameterWidget()->append(tr("Country Pattern"), "addr_country_pattern", ParameterWidget::Text);

  list()->addColumn(tr("First Name"),          80, Qt::AlignLeft,  true, "cntct_first_name");
  list()->addColumn(tr("Last Name"),          100, Qt::AlignLeft,  true, "cntct_last_name");
  list()->addColumn(tr("Active"),              50, Qt::AlignLeft,  true, "cntct_active");
  list()->addColumn(tr("Owner"),      _userColumn, Qt::AlignLeft, false, "cntct_owner_username");
  list()->addColumn(tr("CRM Accounts"),        -1, Qt::AlignLeft,  true, "crmacct");
  list()->addColumn(tr("Title"),               -1, Qt::AlignLeft,  true, "cntct_title");
  list()->addColumn(tr("Office #"),	      100, Qt::AlignLeft,  true, "contact_phone");
  list()->addColumn(tr("Mobile #"),           100, Qt::AlignLeft,  true, "contact_phone2");
  list()->addColumn(tr("Fax #"),              100, Qt::AlignLeft, false, "contact_phone3");
  list()->addColumn(tr("E-Mail"),             100, Qt::AlignLeft,  true, "cntct_email");
  list()->addColumn(tr("Web Address"),        100, Qt::AlignLeft, false, "cntct_webaddr");
  list()->addColumn(tr("Address"),             -1, Qt::AlignLeft, false, "addr_line1");
  list()->addColumn(tr("City"),                75, Qt::AlignLeft, false, "addr_city");
  list()->addColumn(tr("State"),               50, Qt::AlignLeft, false, "addr_state");
  list()->addColumn(tr("Country"),            100, Qt::AlignLeft, false, "addr_country");
  list()->addColumn(tr("Postal Code"),         75, Qt::AlignLeft, false, "addr_postalcode");

  list()->setSelectionMode(QAbstractItemView::ExtendedSelection);

  setupCharacteristics("CNTCT");

  QToolButton * attachBtn = new QToolButton(this);
  attachBtn->setText(tr("Attach"));
  _attachAct = toolBar()->insertWidget(filterSeparator(), attachBtn);
  _attachAct->setEnabled(false);
  _attachAct->setVisible(false);

  QToolButton * detachBtn = new QToolButton(this);
  detachBtn->setText(tr("Detach"));
  _detachAct = toolBar()->insertWidget(filterSeparator(), detachBtn);
  _detachAct->setEnabled(false);
  _detachAct->setVisible(false);

  connect(attachBtn, SIGNAL(clicked()),      this, SLOT(sAttach()));
  connect(detachBtn, SIGNAL(clicked()),      this, SLOT(sDetach()));

  connect(list(), SIGNAL(itemSelected(int)), this, SLOT(sOpen()));

  if (_privileges->check("MaintainAllContacts") || _privileges->check("MaintainPersonalContacts"))
  {
    _attachAct->setEnabled(true);
    connect(list(), SIGNAL(valid(bool)), _detachAct, SLOT(setEnabled(bool)));
  }
  else
  {
    newAction()->setEnabled(false);
  }
}

enum SetResponse contacts::set(const ParameterList& pParams)
{
  XWidget::set(pParams);
  QVariant param;
  bool	   valid;
  
  param = pParams.value("mode", &valid);
  if (valid)
  {
    if (param.toString() == "view")
    {
      _mode = cView;
      _attachAct->setEnabled(false);
      disconnect(list(), SIGNAL(valid(bool)), _detachAct, SLOT(setEnabled(bool)));
    }
  }

  param = pParams.value("showRole", &valid);
  if (valid)
  {
    list()->addColumn(tr("Role"), 80, Qt::AlignLeft, true, "crmrole");
    list()->moveColumn(list()->column("crmrole"), 0);
    _captive = true;
  }

  param = pParams.value("run", &valid);
  if (valid)
    sFillList();

  return NoError;
}

void contacts::sPopulateMenu(QMenu *pMenu, QTreeWidgetItem *, int)
{
  QAction *menuItem;

  bool editPriv =
      ((omfgThis->username() == list()->currentItem()->rawValue("cntct_owner_username") && _privileges->check("MaintainPersonalContacts")) ||
      _privileges->check("MaintainAllContacts")) && _mode != cView;

  bool viewPriv =
      (omfgThis->username() == list()->currentItem()->rawValue("cntct_owner_username") && _privileges->check("ViewPersonalContacts")) ||
      _privileges->check("ViewAllContacts");

  menuItem = pMenu->addAction(tr("Edit..."), this, SLOT(sEdit()));
  menuItem->setEnabled(editPriv);

  menuItem = pMenu->addAction(tr("View..."), this, SLOT(sView()));
  menuItem->setEnabled(viewPriv);

  menuItem = pMenu->addAction(tr("Delete"), this, SLOT(sDelete()));
  menuItem->setEnabled(editPriv);

  // Create, Edit, View Prospect:
  if (!_captive)
  {
    XSqlQuery sql;
    sql.prepare("WITH crmaccts AS ( "
                "SELECT crmaccttypes(crmacctcntctass_crmacct_id)#>>'{prospect}' AS prospectid"
                "  FROM cntct"
                "  JOIN crmacctcntctass ON cntct_id=crmacctcntctass_cntct_id"
                " WHERE cntct_id::TEXT IN (SELECT regexp_split_to_table(:cntct_id, ','))"
                "   AND crmaccttypes(crmacctcntctass_crmacct_id)#>>'{customer}' IS NULL) "
                "SELECT EXISTS(SELECT 1"
                "                FROM crmaccts"
                "               WHERE prospectid IS NOT NULL) AS edit,"
                "       EXISTS(SELECT 1"
                "                FROM crmaccts"
                "               WHERE prospectid IS NULL) AS new;");
    QStringList ids;
    foreach (XTreeWidgetItem *item, list()->selectedItems())
      ids << QString::number(item->id());
    sql.bindValue(":cntct_id", ids.join(","));
    sql.exec();
  
    if (sql.first())
    {
      bool editProspectPriv = _privileges->check("MaintainProspectMasters");
      bool viewProspectPriv = _privileges->check("MaintainProspectMasters ViewProspectMasters");
      
      if (sql.value("edit").toBool())
      {
        pMenu->addSeparator();
        menuItem = pMenu->addAction(tr("Edit Prospect"), this, SLOT(sEditProspect()));
        menuItem->setEnabled(editProspectPriv);
        menuItem = pMenu->addAction(tr("View Prospect"), this, SLOT(sViewProspect()));
        menuItem->setEnabled(viewProspectPriv);
      }
      if (sql.value("new").toBool())
      {
        pMenu->addSeparator();
        menuItem = pMenu->addAction(tr("Create Prospect..."), this, SLOT(sNewProspect()));
        menuItem->setEnabled(editProspectPriv);
      }
    }
    else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Checking CRM Accounts"),
                                  sql, __FILE__, __LINE__))
      return;
  }
}

void contacts::sNew()
{
  ParameterList params;
  setParams(params);
  params.append("mode", "new");

  contact newdlg(0, "", true);
  newdlg.set(params);
  newdlg.setWindowModality(Qt::WindowModal);

  if (newdlg.exec() != XDialog::Rejected)
    sFillList();
}

void contacts::sEdit()
{
  QList<XTreeWidgetItem*> selected = list()->selectedItems();
  for (int i = 0; i < selected.size(); i++)
  {
    ParameterList params;
    params.append("mode", "edit");
    params.append("cntct_id", ((XTreeWidgetItem*)(selected[i]))->id());

    contact* newdlg = new contact(0, "", false);
    newdlg->set(params);
    newdlg->setAttribute(Qt::WA_DeleteOnClose);
    newdlg->show();
  }
}

void contacts::sView()
{
  QList<XTreeWidgetItem*> selected = list()->selectedItems();
  for (int i = 0; i < selected.size(); i++)
  {
    ParameterList params;
    params.append("mode", "view");
    params.append("cntct_id", ((XTreeWidgetItem*)(selected[i]))->id());

    contact* newdlg = new contact(0, "", false);
    newdlg->set(params);
    newdlg->setAttribute(Qt::WA_DeleteOnClose);
    newdlg->show();
  }
}

void contacts::sDelete()
{

  if ( QMessageBox::warning(this, tr("Delete Contacts?"),
                            tr("<p>Are you sure that you want to completely "
                               "delete the selected contact(s)?"),
                            QMessageBox::Yes | QMessageBox::No,
                            QMessageBox::No) == QMessageBox::Yes)
  {
    XSqlQuery delq;
    XSqlQuery chk;
    chk.prepare("SELECT cntctused(:cntct_id) AS inUse;");
    delq.prepare("SELECT deleteCntct(:cntct_id, :cascade);");

    foreach (XTreeWidgetItem *selected, list()->selectedItems())
    {
      bool cascade = false;
      chk.bindValue(":cntct_id", selected->id());
      chk.exec();
      if (chk.first() && chk.value("inUse").toBool())
      {
        if (QMessageBox::warning(this, tr("Delete Parent Objects?"),
                                 tr("There are parent objects that use this contact. Do you wish "
                                    "to delete these objects as well?"),
                                 QMessageBox::Yes | QMessageBox::No,
                                 QMessageBox::No) == QMessageBox::Yes)
          cascade = true;
      }
      else if (ErrorReporter::error(QtCriticalMsg, this, tr("Checking Usage"),
                                    chk, __FILE__, __LINE__))
        return;

      delq.bindValue(":cntct_id", selected->id());
      delq.bindValue(":cascade", cascade);
      delq.exec();
      if (ErrorReporter::error(QtCriticalMsg, this, tr("Deleting Contact"),
                               delq, __FILE__, __LINE__))
        return;
    }

    sFillList();
  }
}

void contacts::sAttach()
{
  ContactCluster attached(this, "attached");
  attached.sEllipses();
  if (attached.id() > 0)
  {
    int answer = QMessageBox::Yes;

    if (attached.crmAcctId() > 0 && attached.crmAcctId() != _crmacctid)
      answer = QMessageBox::question(this, tr("Attach Contact?"),
			    tr("<p>This Contact is currently attached to a "
			       "different Account. Are you sure you want "
			       "to also attach this Account for this person?"),
			    QMessageBox::Yes, QMessageBox::No | QMessageBox::Default);
    if (answer == QMessageBox::Yes)
    {
      XSqlQuery attq;
      attq.prepare("SELECT attachContact(:cntct_id, :crmacct_id) AS returnVal;");
      attq.bindValue(":cntct_id", attached.id());
      attq.bindValue(":crmacct_id", _crmacctid);
      attq.exec();
      if (attq.first())
      {
        int returnVal = attq.value("returnVal").toInt();
        if (returnVal < 0)
        {
            ErrorReporter::error(QtCriticalMsg, this, tr("Error Attaching Contact"),
                                 storedProcErrorLookup("attachContact", returnVal),
                                 __FILE__, __LINE__);
            return;
        }
      }
      else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Attaching Contact"),
                                    attq, __FILE__, __LINE__))
	return;
    }
    sFillList();
  }
}

void contacts::sDetach()
{
  XTreeWidgetItem * item = (XTreeWidgetItem*)list()->currentItem();
  int answer = QMessageBox::question(this, tr("Detach Contact?"),
			tr("<p>Are you sure you want to detach this Contact "
			   "from this Account?"),
			QMessageBox::Yes, QMessageBox::No | QMessageBox::Default);
  if (answer == QMessageBox::Yes)
  {
    int cntctId = list()->id();
    QString role = item->rawValue("crmrole").toString();
    XSqlQuery detq;
    detq.prepare("SELECT detachContact(:cntct_id, :crmacct_id, :role) AS returnVal;");
    detq.bindValue(":cntct_id", cntctId);
    detq.bindValue(":crmacct_id", _crmacctid);
    if(role.length() > 0)
      detq.bindValue(":role", role);
    detq.exec();
    if (detq.first())
    {
      int returnVal = detq.value("returnVal").toInt();
      if (returnVal < 0)
      {
        ErrorReporter::error(QtCriticalMsg, this, tr("Error detaching Contact from Account (%1)")
                        .arg(returnVal),detq, __FILE__, __LINE__);
        return;
      }
      emit cntctDetached(cntctId);
    }
    else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Detaching Contact"),
                                  detq, __FILE__, __LINE__))
      return;

    sFillList();
  }
}

void contacts::setCrmacctid(int crmacctId)
{
  _crmacctid = crmacctId;
  if (_crmacctid == -1)
  {
    parameterWidget()->setDefault(tr("Account"), QVariant(), true);
    _attachAct->setVisible(false);
    _detachAct->setVisible(false);
  }
  else
  {
    parameterWidget()->setDefault(tr("Account"), _crmacctid, true);
    setNewVisible(false);
    _detachAct->setVisible(true);
  }
}

int contacts::crmacctId()
{
  return _crmacctid;
}

QAction* contacts::attachAction()
{
  return _attachAct;
}

QAction* contacts::detachAction()
{
  return _detachAct;
}

bool contacts::setParams(ParameterList &params)
{
  if (!display::setParams(params))
    return false;

  if (_activeOnly->isChecked())
    params.append("activeOnly",true);

  return true;
}

void contacts::sOpen()
{
  bool editPriv =
      (omfgThis->username() == list()->currentItem()->rawValue("cntct_owner_username") && _privileges->check("MaintainPersonalContacts")) ||
      (_privileges->check("MaintainAllContacts"));

  bool viewPriv =
      (omfgThis->username() == list()->currentItem()->rawValue("cntct_owner_username") && _privileges->check("ViewPersonalContacts")) ||
      (_privileges->check("ViewAllContacts"));

  if (editPriv)
    sEdit();
  else if (viewPriv)
    sView();
  else
    QMessageBox::information(this, tr("Restricted Access"), tr("You have not been granted privileges to open this Contact."));
}

void contacts::sNewProspect()
{
  XSqlQuery sql;
  XSqlQuery crmsql;
  ParameterList params;

  sql.prepare("SELECT DISTINCT crmacct_number"
              "  FROM cntct"
              "  JOIN crmacctcntctass ON cntct_id=crmacctcntctass_cntct_id"
              "  JOIN crmacct ON crmacct_id=crmacctcntctass_crmacct_id"
              " WHERE cntct_id=:cntct_id"
              "   AND crmaccttypes(crmacctcntctass_crmacct_id)#>>'{customer}' IS NULL"
              "   AND crmaccttypes(crmacctcntctass_crmacct_id)#>>'{prospect}' IS NULL"
              " ORDER BY crmacct_number;");
  crmsql.prepare("SELECT crmacct_id"
              "  FROM crmacct"
                " WHERE crmacct_number=:crmacct_number;");

  params.append("mode", "new");

  foreach (XTreeWidgetItem *item, list()->selectedItems())
  {
    QStringList crmaccts;

    sql.bindValue(":cntct_id", item->id());
    sql.exec();
    while (sql.next())
      crmaccts << sql.value("crmacct_number").toString();

    if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Fetching CRM Accounts"),
                             sql, __FILE__, __LINE__))
      return;

    if (crmaccts.size() == 0)
      continue;

    QString crmacctsel;
    if (crmaccts.size() == 1)
      crmacctsel = crmaccts[0];
    else
    {
      bool ok;
      crmacctsel = QInputDialog::getItem(this, tr("Multiple CRM Accounts"),
                                         tr("There are Multiple CRM Accounts with the selected Contact. Please select a CRM Account "
                                            "to use for the new Prospect:")
                                         , crmaccts, 0, false, &ok);
      if (!ok)
        continue;
    }


    crmsql.bindValue(":crmacct_number", crmacctsel);
    crmsql.exec();
    if (crmsql.first())
      params.append("crmacct_id", crmsql.value("crmacct_id").toInt());
    else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Fetching CRM Accounts"),
                                  crmsql, __FILE__, __LINE__))
      return;

    prospect *newdlg = new prospect();
    newdlg->set(params);
    omfgThis->handleNewWindow(newdlg);
  }
}

void contacts::sEditProspect()
{
  sOpenProspect("edit");
}

void contacts::sViewProspect()
{
  sOpenProspect("view");
}

void contacts::sOpenProspect(QString mode)
{
  foreach (XTreeWidgetItem *item, list()->selectedItems())
  {
    ParameterList params;
    params.append("mode", mode);

    QStringList prospects;

    XSqlQuery sql;
    sql.prepare("SELECT DISTINCT prospect_number"
                "  FROM cntct"
                "  JOIN crmacctcntctass ON cntct_id=crmacctcntctass_cntct_id"
                "  JOIN prospect ON crmacctcntctass_crmacct_id=prospect_crmacct_id"
                " WHERE cntct_id=:cntct_id"
                "   AND crmaccttypes(crmacctcntctass_crmacct_id)#>>'{customer}' IS NULL"
                "   AND crmaccttypes(crmacctcntctass_crmacct_id)#>>'{prospect}' IS NOT NULL"
                " ORDER BY prospect_number;");
    sql.bindValue(":cntct_id", item->id());
    sql.exec();
    while (sql.next())
      prospects << sql.value("prospect_number").toString();

    if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Fetching Prospects"),
                             sql, __FILE__, __LINE__))
      return;

    if (prospects.size() == 0)
      continue;

    QString prospectsel;
    if (prospects.size() == 1)
      prospectsel = prospects[0];
    else
    {
      bool ok;
      prospectsel = QInputDialog::getItem(this, tr("Multiple Prospects"),
                                      tr("There are multiple Prospects with the selected Contact. "
                                         "Please select a Prospect to edit:")
                                      , prospects, 0, false, &ok);
      if (!ok)
        continue;
    }

    sql.prepare("SELECT prospect_id"
                "  FROM prospect"
                " WHERE prospect_number=:prospect_number;");
    sql.bindValue(":prospect_number", prospectsel);
    sql.exec();
    if (sql.first())
      params.append("prospect_id", sql.value("prospect_id").toInt());
    else if (ErrorReporter::error(QtCriticalMsg, this, tr("Error Fetching Prospects"),
                                  sql, __FILE__, __LINE__))
      return;

    prospect *newdlg = new prospect();
    newdlg->set(params);
    omfgThis->handleNewWindow(newdlg);
  }
}
