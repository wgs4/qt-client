/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2014 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include "guiclient.h"
#include "xdialog.h"

#include <parameter.h>

#include "ui_bankAccount.h"

class bankAccount : public XDialog, public Ui::bankAccount
{
    Q_OBJECT

public:
    bankAccount(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = Qt::WindowFlags {});
    ~bankAccount();

public slots:
    virtual enum SetResponse set(const ParameterList & pParams );
    virtual void sCheck();
    virtual void sNameChanged(QString);
    virtual void sSave();
    virtual void populate();
    virtual void sTogglePrefix();

protected:
    QValidator *_accountValidator;
    QValidator *_routingValidator;

protected slots:
    virtual void languageChange();
    virtual void sHandleTransmitGroup();
    virtual void sHandleType();

private:
    int _mode;
    int _bankaccntid;

};

#endif // BANKACCOUNT_H
