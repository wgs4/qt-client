/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2017 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "prospect.h"

#include <QAction>
#include <QCloseEvent>
#include <QMenu>
#include <QMessageBox>
#include <QSqlError>
#include <QVariant>

#include <openreports.h>

#include "crmaccount.h"
#include "errorReporter.h"
#include "guiErrorCheck.h"
#include "parameterwidget.h"
#include "printQuote.h"
#include "salesOrder.h"
#include "storedProcErrorLookup.h"
#include <xcombobox.h>

prospect::prospect(QWidget* parent, const char* name, Qt::WindowFlags fl)
    : XWidget(parent, name, fl)
{
  setupUi(this);

  connect(_crmacct,     SIGNAL(clicked()),      this,   SLOT(sCrmAccount()));
  connect(_deleteQuote, SIGNAL(clicked()),	this,	SLOT(sDeleteQuote()));
  connect(_editQuote,	SIGNAL(clicked()),	this,	SLOT(sEditQuote()));
  connect(_newQuote,	SIGNAL(clicked()),	this,	SLOT(sNewQuote()));
  connect(_number,	SIGNAL(editingFinished()),	this,	SLOT(sCheckNumber()));
  connect(_printQuote,  SIGNAL(clicked()),	this,	SLOT(sPrintQuote()));
  connect(_quotes,	SIGNAL(populateMenu(QMenu*,QTreeWidgetItem*)),	this,	SLOT(sPopulateQuotesMenu(QMenu*)));
  connect(_save,	SIGNAL(clicked()),	this,	SLOT(sSave()));
  connect(_viewQuote,	SIGNAL(clicked()),	this,	SLOT(sViewQuote()));
  connect(omfgThis,	SIGNAL(quotesUpdated(int, bool)), this, SLOT(sFillQuotesList()));

  if (_privileges->check("MaintainQuotes"))
    connect(_quotes, SIGNAL(itemSelected(int)), _editQuote, SLOT(animateClick()));
  else
    connect(_quotes, SIGNAL(itemSelected(int)), _viewQuote, SLOT(animateClick()));

  _prospectid = -1;
  _crmacctid = -1;

  _taxzone->setAllowNull(true);
  _taxzone->setType(XComboBox::TaxZones);

  _quotes->addColumn(tr("Quote #"),          _orderColumn, Qt::AlignLeft, true, "quhead_number" );
  _quotes->addColumn(tr("Quote Date"),       _dateColumn,  Qt::AlignLeft, true, "quhead_quotedate" );

  _NumberGen = -1;

  _contacts = new contacts(this, "contacts", Qt::Widget);
  _contactTab->layout()->addWidget(_contacts);
  _contacts->setCloseVisible(false);
  _contacts->list()->hideColumn("crmacct_number");
  _contacts->list()->hideColumn("crmacct_name");
  _contacts->parameterWidget()->append("", "hasContext", ParameterWidget::Exists, true);
  _contacts->setParameterWidgetVisible(false);
  _contacts->setQueryOnStartEnabled(false);

  sBuildContacts();

  _charass->setType("PSPCT");
  _documents->setType("PSPCT");
  _comments->setType("PSPCT");
}

prospect::~prospect()
{
  // no need to delete child widgets, Qt does it all for us
}

void prospect::languageChange()
{
  retranslateUi(this);
}

enum SetResponse prospect::set(const ParameterList &pParams)
{
  XWidget::set(pParams);
  QVariant param;
  XSqlQuery getq;
  bool     valid;

  param = pParams.value("crmacct_id", &valid);
  if (valid)
  {
    _crmacctid = param.toInt();
    sSetCrmAccountId();
  }

  param = pParams.value("prospect_id", &valid);
  if (valid)
    _prospectid = param.toInt();

  if (_crmacctid >= 0 || _prospectid >= 0)
    if (! sPopulate())
      return UndefinedError;

  param = pParams.value("mode", &valid);
  if (valid)
  {
    if (param.toString() == "new")
    {
      _mode = cNew;

      getq.exec("SELECT NEXTVAL('crmacct_crmacct_id_seq') AS crmacct_id");
      if(getq.first())
      {
        _crmacctid = getq.value("crmacct_id").toInt();
        sSetCrmAccountId();
      }

      if(((_metrics->value("CRMAccountNumberGeneration") == "A") ||
          (_metrics->value("CRMAccountNumberGeneration") == "O"))
       && _number->text().isEmpty() )
      {
        getq.exec("SELECT fetchCRMAccountNumber() AS number;");
        if (getq.first())
        {
          _number->setText(getq.value("number"));
          _NumberGen = getq.value("number").toInt();
        }
      }
    }
    else if (param.toString() == "edit")
    {
      _mode = cEdit;
    }
    else if (param.toString() == "view")
    {
      _mode = cView;

      _save->hide();

      disconnect(_quotes, SIGNAL(itemSelected(int)), _editQuote, SLOT(animateClick()));
      disconnect(_quotes, SIGNAL(valid(bool)),       _editQuote, SLOT(setEnabled(bool)));
      disconnect(_quotes, SIGNAL(valid(bool)),       _editQuote, SLOT(setEnabled(bool)));
      disconnect(_quotes, SIGNAL(valid(bool)),     _deleteQuote, SLOT(setEnabled(bool)));

      connect(_quotes, SIGNAL(itemSelected(int)), _viewQuote, SLOT(animateClick()));

      _close->setText(tr("&Close"));
    }
  }

  bool canEdit = (cEdit == _mode || cNew == _mode);
  _number->setEnabled(canEdit &&
                      _metrics->value("CRMAccountNumberGeneration") != "A");
  _active->setEnabled(canEdit);
  _name->setEnabled(canEdit);
  _newQuote->setEnabled(cEdit == _mode);
  _notes->setReadOnly(! canEdit);
  _mainGroup->setEnabled(canEdit);

  return NoError;
}

int prospect::id() const
{
  return _prospectid;
}

/** \return one of cNew, cEdit, cView, ...
    \todo   change possible modes to an enum in guiclient.h (and add cUnknown?)
 */
int prospect::mode() const
{
  return _mode;
}

void prospect::sSetCrmAccountId()
{
  _contacts->setCrmacctid(_crmacctid);
  _contacts->sFillList();
  _charass->setId(_prospectid);
  _documents->setId(_prospectid);
  _comments->setId(_prospectid);
}

void prospect::sSave()
{
  QList<GuiErrorCheck> errors;
  errors << GuiErrorCheck(_number->text().trimmed().isEmpty(), _number,
                          tr("You must enter a number for this Prospect"))
         << GuiErrorCheck(_name->text().trimmed().isEmpty(), _name,
                          tr("You must enter a name for this Prospect"))
    ;
  // disallow overlap of prospect and customer numbers
  if (_number->text().trimmed() != _cachedNumber)
  {
    XSqlQuery dupq;
    dupq.prepare( "SELECT prospect_name AS name "
	       "FROM prospect "
	       "WHERE (UPPER(prospect_number)=UPPER(:prospect_number)) "
	       "  AND (prospect_id<>:prospect_id) "
	       "UNION "
	       "SELECT cust_name AS name "
	       "FROM custinfo "
	       "WHERE (UPPER(cust_number)=UPPER(:prospect_number));" );
    dupq.bindValue(":prospect_number", _number->text().trimmed());
    dupq.bindValue(":prospect_id",     _prospectid);
    dupq.exec();
    if (dupq.first())
      errors << GuiErrorCheck(true, _number,
			     tr("<p>The newly entered Prospect Number cannot "
				"be used as it is currently in use by '%1'. "
				"Please enter a different Prospect Number." )
			     .arg(dupq.value("name").toString()) );
  }

  if (GuiErrorCheck::reportErrors(this, tr("Cannot Save Prospect"), errors))
    return;

  XSqlQuery upsq;
  if (_mode == cEdit)
  {
    upsq.prepare( "UPDATE prospect SET "
               "       prospect_number=:prospect_number,"
               "       prospect_name=:prospect_name,"
               "       prospect_comments=:prospect_comments,"
               "       prospect_taxzone_id=:prospect_taxzone_id,"
               "       prospect_salesrep_id=:prospect_salesrep_id,"
               "       prospect_warehous_id=:prospect_warehous_id,"
               "       prospect_active=:prospect_active, "
               "       prospect_owner_username=:prospect_owner_username, "
               "       prospect_assigned_username=:prospect_assigned_username, "
               "       prospect_assigned=:prospect_assigned, "
               "       prospect_lasttouch=:prospect_lasttouch, "
               "       prospect_source_id=:prospect_source_id, "
               "       prospect_priority_id=:prospect_priority_id, "
               "       prospect_status=:prospect_status "
               " WHERE (prospect_id=:prospect_id)"
               " RETURNING prospect_id;" );
    upsq.bindValue(":prospect_id",	_prospectid);
  }
  else
    upsq.prepare("INSERT INTO prospect "
               "( prospect_id,	      prospect_number,	    prospect_name,"
               "  prospect_cntct_id,  prospect_taxzone_id,  prospect_comments,"
               "  prospect_salesrep_id, prospect_warehous_id, prospect_active, "
               "  prospect_owner_username, prospect_assigned_username, prospect_assigned, "
               "  prospect_lasttouch, prospect_source_id, prospect_priority_id, "
               "  prospect_status )"
               " VALUES "
               "( DEFAULT,     	      :prospect_number,	    :prospect_name,"
               "  :prospect_cntct_id, :prospect_taxzone_id, :prospect_comments,"
               "  :prospect_salesrep_id, :prospect_warehous_id, :prospect_active, "
               "  :prospect_owner_username, :prospect_assigned_username, :prospect_assigned, "
               "  :prospect_lasttouch, :prospect_source_id, :prospect_priority_id, "
               "  :prospect_status )"
               " RETURNING prospect_id;");

  upsq.bindValue(":prospect_number",	_number->text().trimmed());
  upsq.bindValue(":prospect_name",	_name->text().trimmed());
  upsq.bindValue(":prospect_comments",	_notes->toPlainText());
  upsq.bindValue(":prospect_active",	QVariant(_active->isChecked()));
  if (_taxzone->isValid())
    upsq.bindValue(":prospect_taxzone_id",  _taxzone->id());
  if (_salesrep->isValid())
    upsq.bindValue(":prospect_salesrep_id", _salesrep->id());
  if (_site->isValid())
    upsq.bindValue(":prospect_warehous_id", _site->id());
  upsq.bindValue(":prospect_owner_username", _owner->username());
  upsq.bindValue(":prospect_assigned_username", _assignedTo->username());
  upsq.bindValue(":prospect_assigned", _assigned->date());
  upsq.bindValue(":prospect_lasttouch", _lastTouch->date());
  if (_source->isValid())
    upsq.bindValue(":prospect_source_id", _source->id());
  if (_priority->isValid())
    upsq.bindValue(":prospect_priority_id", _priority->id());

// TODO Prospect Status

  upsq.exec();
  if (upsq.first())
    _prospectid = upsq.value("prospect_id").toInt();
  else if (ErrorReporter::error(QtCriticalMsg, this, tr("Saving Prospect"),
                                upsq, __FILE__, __LINE__))
    return;

  _NumberGen = -1;
  omfgThis->sProspectsUpdated();
  emit saved(_prospectid);
  if (_mode == cNew)
  {
    omfgThis->sCrmAccountsUpdated(_crmacctid);
    emit newId(_prospectid);   // cluster listeners couldn't handle set()'s emit
  }

  sSaveContacts();

  close();
}

void prospect::sCheckNumber()
{
  _number->setText(_number->text().trimmed().toUpper());

  if (_mode == cNew)
  {
    if (-1 != _NumberGen && _number->text().toInt() != _NumberGen)
    {
      XSqlQuery query;
      query.prepare( "SELECT releaseCRMAccountNumber(:Number);" );
      query.bindValue(":Number", _NumberGen);
      query.exec();
      _NumberGen = -1;
    }

    XSqlQuery getq;
    getq.prepare( "SELECT prospect_id "
               "FROM prospect "
               "WHERE (prospect_number=:prospect);" );
    getq.bindValue(":prospect", _number->text());
    getq.exec();
    if (getq.first())
    {
      _prospectid = getq.value("prospect_id").toInt();
      _mode = cEdit;
      sPopulate();
      emit newId(_prospectid);
      _name->setFocus();
    }
  }
}

void prospect::sPrintQuote()
{
  ParameterList params;
  params.append("quhead_id", _quotes->id());

  printQuote newdlg(this, "printQuote", true);
  newdlg.set(params);
  newdlg.exec();
}

void prospect::sNewQuote()
{
  ParameterList params;
  params.append("mode", "newQuote");
  params.append("cust_id", _prospectid);

  salesOrder *newdlg = new salesOrder();
  newdlg->set(params);
  omfgThis->handleNewWindow(newdlg);
}

void prospect::sEditQuote()
{
  ParameterList params;
  params.append("mode", "editQuote");
  params.append("quhead_id", _quotes->id());

  salesOrder *newdlg = new salesOrder();
  newdlg->set(params);
  omfgThis->handleNewWindow(newdlg);
}

void prospect::sViewQuote()
{
  ParameterList params;
  params.append("mode", "viewQuote");
  params.append("quhead_id", _quotes->id());

  salesOrder *newdlg = new salesOrder();
  newdlg->set(params);
  omfgThis->handleNewWindow(newdlg);
}

void prospect::sDeleteQuote()
{
  if (QMessageBox::question(this, tr("Delete Selected Quote"),
                            tr("Are you sure that you want to delete the selected Quote?" ),
                            QMessageBox::Yes,
                            QMessageBox::No | QMessageBox::Default) == QMessageBox::No)
    return;

  XSqlQuery delq;
  delq.prepare("SELECT deleteQuote(:quhead_id) AS result;");
  delq.bindValue(":quhead_id", _quotes->id());
  delq.exec();
  if (delq.first())
  {
    int result = delq.value("result").toInt();
    if (result < 0)
    {
      ErrorReporter::error(QtCriticalMsg, this, tr("Deleting Quote"),
                           storedProcErrorLookup("deleteQuote", result),
                           __FILE__, __LINE__);
      return;
    }
    else
      omfgThis->sQuotesUpdated(-1);
  }
  else if (ErrorReporter::error(QtCriticalMsg, this, tr("Deleting Quote"),
                                delq, __FILE__, __LINE__))
    return;
}

void prospect::sFillQuotesList()
{
  XSqlQuery getq;
  getq.prepare("SELECT quhead_id, quhead_number, quhead_quotedate "
	    "FROM quhead "
	    "WHERE (quhead_cust_id=:prospect_id) "
	    "ORDER BY quhead_number;");
  getq.bindValue(":prospect_id", _prospectid);
  getq.exec();
  if (ErrorReporter::error(QtCriticalMsg, this, tr("Getting Quote"),
                           getq, __FILE__, __LINE__))
    return;

  _quotes->populate(getq);
}

void prospect::sPopulateQuotesMenu(QMenu *menuThis)
{
  menuThis->addAction(tr("Edit..."),   this, SLOT(sEditQuote()));
  menuThis->addAction(tr("View..."),   this, SLOT(sViewQuote()));
  menuThis->addAction(tr("Delete..."), this, SLOT(sDeleteQuote()));
  menuThis->addAction(tr("Print..."),  this, SLOT(sPrintQuote()));
}

bool prospect::sPopulate()
{
  XSqlQuery getq;
  if (_prospectid >= 0)
  {
    getq.prepare("SELECT prospect.*, crmacct_id, crmacct_owner_username"
                 "  FROM prospect, crmacct"
                 " WHERE ((prospect_id=:prospect_id)"
                 "   AND  (prospect_id=crmacct_prospect_id));" );
    getq.bindValue(":prospect_id", _prospectid);
  }
  else if (_crmacctid >= 0)
  {
    getq.prepare("SELECT crmacct_active AS prospect_active,"
                 "       crmacct_name   AS prospect_name,"
                 "       crmacct_number AS prospect_number,"
                 "       crmacct_cntct_id_1 AS prospect_cntct_id,"
                 "       crmacct_owner_username AS prospect_owner_username,"
                 "       NULL AS prospect_comments, -1 AS prospect_taxzone_id,"
                 "      -1 AS prospect_salesrep_id, -1 AS prospect_warehous_id,"
                 "      crmacct_id, crmacct_owner_username"
                 "  FROM crmacct"
                 " WHERE (crmacct_id=:id);");
    getq.bindValue(":id", _crmacctid);
  }

  getq.exec();
  if (getq.first())
  {
    _crmacctid = getq.value("crmacct_id").toInt();
    sSetCrmAccountId();

    _number->setText(getq.value("prospect_number").toString());
    _cachedNumber = getq.value("prospect_number").toString();
    _name->setText(getq.value("prospect_name").toString());
    _taxzone->setId(getq.value("prospect_taxzone_id").toInt());
    _salesrep->setId(getq.value("prospect_salesrep_id").toInt());
    _site->setId(getq.value("prospect_warehous_id").toInt());
    _notes->setText(getq.value("prospect_comments").toString());
    _active->setChecked(getq.value("prospect_active").toBool());
    _crmowner = getq.value("crmacct_owner_username").toString();
    _owner->setUsername(getq.value("prospect_owner_username").toString());
    _assignedTo->setUsername(getq.value("prospect_assigned_username").toString());
    _assigned->setDate(getq.value("prospect_assigned").toDate());
    _lastTouch->setDate(getq.value("prospect_lasttouch").toDate());
    _source->setId(getq.value("prospect_source_id").toInt());
    _priority->setId(getq.value("prospect_priority_id").toInt());
//    _status->setCurrentIndex(_statusCodes.indexOf(getq.value("prospect_status").toString()));
  }
  else if (ErrorReporter::error(QtCriticalMsg, this, tr("Getting Prospect"),
                                getq, __FILE__, __LINE__))
    return false;

  _crmacct->setEnabled(_crmacctid > 0 &&
                       (_privileges->check("MaintainAllCRMAccounts") ||
                        _privileges->check("ViewAllCRMAccounts") ||
                        (omfgThis->username() == _crmowner && _privileges->check("MaintainPersonalCRMAccounts")) ||
                        (omfgThis->username() == _crmowner && _privileges->check("ViewPersonalCRMAccounts"))));

  sFillQuotesList();
  emit populated();

  return true;
}

void prospect::closeEvent(QCloseEvent *pEvent)
{
  if(cNew == _mode && -1 != _NumberGen)
  {
    XSqlQuery query;
    query.prepare( "SELECT releaseCRMAccountNumber(:Number);" );
    query.bindValue(":Number", _NumberGen);
    query.exec();
    _NumberGen = -1;
    ErrorReporter::error(QtCriticalMsg, this, tr("Getting Prospect"),
                         query, __FILE__, __LINE__);
  }
  XWidget::closeEvent(pEvent);
}

void prospect::sCrmAccount()
{
  ParameterList params;
  params.append("crmacct_id", _crmacctid);
  if ((cView == _mode && _privileges->check("ViewAllCRMAccounts")) ||
      (cView == _mode && _privileges->check("ViewPersonalCRMAccounts")
                      && omfgThis->username() == _crmowner) ||
      (cEdit == _mode && _privileges->check("ViewAllCRMAccounts")
                      && ! _privileges->check("MaintainAllCRMAccounts")) ||
      (cEdit == _mode && _privileges->check("ViewPersonalCRMAccounts")
                      && ! _privileges->check("MaintainPersonalCRMAccounts")
                      && omfgThis->username() == _crmowner))
    params.append("mode", "view");
  else if ((cEdit == _mode && _privileges->check("MaintainAllCRMAccounts")) ||
           (cEdit == _mode && _privileges->check("MaintainPersonalCRMAccounts")
                           && omfgThis->username() == _crmowner))
    params.append("mode", "edit");
  else if ((cNew == _mode && _privileges->check("MaintainAllCRMAccounts")) ||
           (cNew == _mode && _privileges->check("MaintainPersonalCRMAccounts")
                          && omfgThis->username() == _crmowner))
    params.append("mode", "edit");
  else
  {
    qWarning("tried to open Account window without privilege");
    return;
  }

  crmaccount *newdlg = new crmaccount();
  newdlg->set(params);
  omfgThis->handleNewWindow(newdlg);
}

void prospect::sBuildContacts()
{
  XSqlQuery getc;
  _rowId = -1;
  cmap.clear();
  getc.prepare("SELECT cntctphone_crmrole_id, cntctphone_phone "
               "FROM cntctphone "
               "WHERE cntctphone_cntct_id = 39;");
  getc.exec();
  ErrorReporter::error(QtCriticalMsg, this, tr("Getting Contact Details"),
                         getc, __FILE__, __LINE__);
  if (getc.size() == 0)
    return;

  _add = new QPushButton();
  _add->setObjectName(QString("_contactAdd%1").arg(_rowId));
  _add->setText("+");
  connect(_add, SIGNAL(clicked()), this, SLOT(sAddNewContactRow()));

  while(getc.next())
  {
    sAddNewContactRow();
    XComboBox *cRole   = findChild<XComboBox*>(QString("_contact%1").arg(_rowId));
    QLineEdit *cNumber = findChild<QLineEdit*>(QString("_contactPhone%1").arg(_rowId));
    cRole->setId(getc.value("cntctphone_crmrole_id").toInt());
    cNumber->setText(getc.value("cntctphone_phone").toString());
  }
} 

void prospect::sAddNewContactRow()
{
  _rowId++;

  QPushButton *rem = new QPushButton();
  QLineEdit *ph  = new QLineEdit();
  XComboBox  *cb   = new XComboBox(_contactsGrid);

  cb->setObjectName(QString("_contact%1").arg(_rowId));
  cb->setType(XComboBox::CRMRolePhone);
  ph->setObjectName(QString("_contactPhone%1").arg(_rowId));

  rem->setObjectName(QString("_contactRem%1").arg(_rowId));
  rem->setText("-");
  connect(rem, SIGNAL(clicked()), this, SLOT(sRemoveContact()));
 
  _contactsGrid->addWidget(cb, _rowId, 0);
  _contactsGrid->addWidget(ph, _rowId, 1);
  _contactsGrid->addWidget(rem, _rowId, 2);

  cmap[rem->objectName()] = _rowId;
  _contactsGrid->addWidget(_add, _rowId, 3);
}

void prospect::sRemoveContact()
{
  XSqlQuery remq;
  QString rem = qobject_cast<QPushButton*>(sender())->objectName();
  int i = cmap[rem];

  XComboBox *oldContact = findChild<XComboBox*>(QString("_contact%1").arg(cmap[rem]));
  QLineEdit *oldNumb = findChild<QLineEdit*>(QString("_contactPhone%1").arg(cmap[rem]));
  QPushButton *oldRem = findChild<QPushButton*>(QString("_contactRem%1").arg(cmap[rem]));

  remq.prepare("DELETE FROM cntctphone "
               "WHERE cntctphone_crmrole_id=:cntctphone_crmrole_id "
               " AND  cntctphone_cntct_id=:cntctphone_cntct_id "
               " AND  cntctphone_phone=:cntctphone_phone;");
  remq.bindValue(":cntctphone_crmrole_id", oldContact->id());
  remq.bindValue(":cntctphone_cntct_id", 39);
  remq.bindValue(":cntctphone_phone", oldNumb->text());
  remq.exec();
  ErrorReporter::error(QtCriticalMsg, this, tr("Removing Contact Details"),
                         remq, __FILE__, __LINE__);

  delete oldNumb;
  delete oldContact;
  delete oldRem;
  cmap.remove(rem);

  if (i == _rowId)
    _rowId--;
  _contactsGrid->addWidget(_add, _rowId, 3);
}

void prospect::sSaveContacts()
{
  XSqlQuery savec;

    savec.prepare("INSERT INTO cntctphone (cntctphone_crmrole_id, cntctphone_cntct_id, cntctphone_phone) "
                  " SELECT :cntctphone_crmrole_id, :cntctphone_cntct_id, :cntctphone_phone "
                  " WHERE NOT EXISTS (SELECT 1 FROM cntctphone WHERE cntctphone_crmrole_id=:cntctphone_crmrole_id "
                  "                                             AND  cntctphone_cntct_id=:cntctphone_cntct_id "
                  "                                             AND  cntctphone_phone=:cntctphone_phone);");

  foreach (int i, cmap) {
    XComboBox *cRole   = findChild<XComboBox*>(QString("_contact%1").arg(i));
    QLineEdit *cNumber = findChild<QLineEdit*>(QString("_contactPhone%1").arg(i));

    if (!cRole->isValid() || cNumber->text().trimmed().isEmpty())
      continue;

    savec.bindValue(":cntctphone_crmrole_id", cRole->id());
    savec.bindValue(":cntctphone_cntct_id", 39);
    savec.bindValue(":cntctphone_phone", cNumber->text());
    savec.exec();
    ErrorReporter::error(QtCriticalMsg, this, tr("Saving Contact Details"),
                         savec, __FILE__, __LINE__);
  }
}
