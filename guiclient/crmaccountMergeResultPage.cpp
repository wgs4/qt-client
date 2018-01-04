/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2018 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "crmaccountMergeResultPage.h"

#include <QMessageBox>

#include <metasql.h>
#include <mqlutil.h>

#include "crmaccount.h"
#include "crmaccountMerge.h"
#include "errorReporter.h"

class CrmaccountMergeResultPagePrivate
{
  public:
    CrmaccountMergeResultPagePrivate(QWizardPage *parent)
      : _parent(parent)
    {
      _crmaccount = new crmaccount();
      _crmaccount->_save->setVisible(false);
      _crmaccount->_close->setVisible(false);

      QString errmsg;
      bool    ok = false;
      _obsoletemqlstr = MQLUtil::mqlLoad("crmaccountmerge", "wip", errmsg, &ok);
      if (!ok)
        ErrorReporter::error(QtCriticalMsg, _parent,
                             QT_TRANSLATE_NOOP("CrmaccountMergeResultPage",
                                               "Getting Source Accounts"),
                             errmsg, __FILE__, __LINE__);

      ParameterList params;
      params.append("mode", "view");
      _crmaccount->set(params);

      QWidget *sa = _parent->findChild<QWidget*>("_resultScrollAreaContents");
      QLayout *lyt = sa ? sa->layout() : 0;
      if (lyt)
        lyt->addWidget(_crmaccount);
      else
        ErrorReporter::error(QtWarningMsg, _parent,
                             QT_TRANSLATE_NOOP("CrmaccountMergeResultPage",
                                               "Could not draw Account"),
                             QT_TRANSLATE_NOOP("CrmaccountMergeResultPage",
                                "Could not find the portion of the window "
                                "in which to draw the target Account."));
    };

    crmaccount *_crmaccount;
    QString     _obsoletemqlstr;
    QWidget    *_parent;
};

CrmaccountMergeResultPage::CrmaccountMergeResultPage(QWidget *parent)
  : QWizardPage(parent),
    _data(0)
{
  setupUi(this);

  _data = new CrmaccountMergeResultPagePrivate(this);
}

CrmaccountMergeResultPage::~CrmaccountMergeResultPage()
{
  if (_data)
    delete _data;
}

void CrmaccountMergeResultPage::initializePage()
{
  XSqlQuery idq;
  idq.prepare("SELECT crmacct_id"
              "  FROM crmacct"
              " WHERE (crmacct_id=:id);");
  idq.bindValue(":id", field("_completedMerge"));
  idq.exec();
  if (idq.first())
    _data->_crmaccount->setId(idq.value("crmacct_id").toInt());
  else if (ErrorReporter::error(QtCriticalMsg, this,
                                tr("Error Getting Account"),
                                idq, __FILE__, __LINE__))
    return;
  else
  {
    QMessageBox::warning(this, tr("Could Not Find Account"),
                         tr("Could not find the merged Account (%1).")
                         .arg(field("_completedMerge").toString()));
    _data->_crmaccount->setId(-1);
  }
}

