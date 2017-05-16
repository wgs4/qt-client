/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2014 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "returnWoMaterialBatch.h"

#include <QVariant>
#include <QMessageBox>
#include "inputManager.h"
#include "distributeInventory.h"
#include "errorReporter.h"

returnWoMaterialBatch::returnWoMaterialBatch(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
  : XDialog(parent, name, modal, fl)
{
  XSqlQuery returnreturnWoMaterialBatch;
  setupUi(this);

  connect(_wo, SIGNAL(valid(bool)), _return, SLOT(setEnabled(bool)));
  connect(_return, SIGNAL(clicked()), this, SLOT(sReturn()));
  connect(_close, SIGNAL(clicked()), this, SLOT(reject()));

  _captive = false;
  _transDate->setEnabled(_privileges->check("AlterTransactionDates"));
  _transDate->setDate(omfgThis->dbDate(), true);
  returnreturnWoMaterialBatch.bindValue(":date",  _transDate->date());

  omfgThis->inputManager()->notify(cBCWorkOrder, this, _wo, SLOT(setId(int)));

  // Change to Issued only
  //_wo->setType(cWoExploded | cWoReleased | cWoIssued);
  _wo->setType(cWoIssued);
  _wo->setFocus();
}

returnWoMaterialBatch::~returnWoMaterialBatch()
{
  // no need to delete child widgets, Qt does it all for us
}

void returnWoMaterialBatch::languageChange()
{
  retranslateUi(this);
}

enum SetResponse returnWoMaterialBatch::set(const ParameterList &pParams)
{
  XDialog::set(pParams);
  _captive = true;

  QVariant param;
  bool     valid;

  param = pParams.value("wo_id", &valid);
  if (valid)
  {
    _wo->setId(param.toInt());
  }

  return NoError;
}

void returnWoMaterialBatch::sReturn()
{
  XSqlQuery returnReturn;
  if (!_transDate->isValid())
  {
    QMessageBox::critical(this, tr("Invalid date"),
                          tr("You must enter a valid transaction date.") );
    _transDate->setFocus();
    return;
  }
  
  returnReturn.prepare( "SELECT wo_qtyrcv, wo_status "
             "FROM wo "
             "WHERE (wo_id=:wo_id);" );
  returnReturn.bindValue(":wo_id", _wo->id());
  returnReturn.exec();
  if (returnReturn.first())
  {
    if (_wo->method() == "A" && returnReturn.value("wo_qtyrcv").toDouble() != 0)
    {
      QMessageBox::warning( this, tr("Cannot return Work Order Material"),
                            tr( "This Work Order has had material received against it\n"
                                "and thus the material issued against it cannot be returned.\n"
                                "You must instead return each Work Order Material item individually.\n" ) );
      _wo->setId(-1);
      _wo->setFocus();
    }
    else
    {
      if(returnReturn.value("wo_status").toString() == "E" || 
         returnReturn.value("wo_status").toString() == "R" ||
         returnReturn.value("wo_status").toString() == "I")
      {
        XSqlQuery items;
        COALESCE(itemuomtouom(:itemsite_item_id, NULL, :womatl_uom_id, :qty), :qty)
        items.prepare("SELECT womatl_id, womatl_uom_id, womatl_qtyreq, womatl_qtyiss, "
                      " COALESCE(itemuomtouom(item_id, NULL, womatl_uom_id, "
                      "   CASE WHEN wo_qtyord >= 0 THEN womatl_qtyiss "
                      "        ELSE ((womatl_qtyreq - womatl_qtyiss) * -1) "
                      "   END), "
                      "   CASE WHEN wo_qtyord >= 0 THEN womatl_qtyiss "
                      "        ELSE ((womatl_qtyreq - womatl_qtyiss) * -1) "
                      "   END "
                      " ) AS qty, "
                      " itemsite_id, itemsite_item_id, item_number, "
                      " isControlledItemsite(itemsite_id) AS controlled "
                      "  FROM wo, womatl, itemsite, item"
                      " WHERE((wo_id=womatl_wo_id)"
                      "   AND (womatl_itemsite_id=itemsite_id)"
                      "   AND (itemsite_item_id=item_id)"
                      "   AND ( (wo_qtyord < 0) OR (womatl_issuemethod IN ('S','M')) )"
                      "   AND (womatl_wo_id=:wo_id)) "
                      "ORDER BY womatl_id;");
        items.bindValue(":wo_id", _wo->id());
        items.exec();

        int succeeded = 0;
        QList<QString> failedItems;
        QList<QString> errors;
        while(items.next())
        {
          if(items.value("qty").toDouble() == 0.0)
            continue;

          int itemlocSeries;

          // Stage distribution cleanup function to be called on error
          XSqlQuery cleanup;
          cleanup.prepare("SELECT deleteitemlocseries(:itemlocSeries, TRUE);");

          // Get the parent series id
          itemlocSeries = distributeInventory::SeriesCreate(items.value("itemsite_id").toInt(),
            items.value("qty").toDouble(), "WO", "IM", _wo->id());
          if (itemlocSeries <= 0)
            return;

          cleanup.bindValue(":itemlocSeries", itemlocSeries);

          // If controlled and backflush item has relevant qty for returnWoMaterial
          if (items.value("controlled").toBool() && 
             (items.value("womatl_qtyreq").toDouble() >= 0 ? 
              items.value("womatl_qtyiss").toDouble() >= items.value("qty").toDouble() : 
              items.value("womatl_qtyiss").toDouble() <= items.value("qty").toDouble()))
          {
           
            if (distributeInventory::SeriesAdjust(itemlocSeries, this, QString(), QDate(),
              QDate(), true) == XDialog::Rejected)
            {
              cleanup.exec();
              // If it's not the last item in the loop, ask the user to exit loop or continue
              if (items.at() != (items.size() -1))
              {
                if (QMessageBox::question(this,  tr("Return WO Material"),
                tr("Posting distribution detail for item number %1 was cancelled but "
                  "there are more items to issue. Continue issuing the remaining materials?")
                .arg(items.value("item_number").toString()),
                QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
                {
                  failedItems.append(items.value("item_number").toString());
                  errors.append("Detail Distribution Cancelled");
                  continue;
                }
                else
                {
                  failedItems.append(items.value("item_number").toString());
                  errors.append("Detail Distribution Cancelled");
                  break;
                }
              }
              else 
              {
                failedItems.append(items.value("item_number").toString());
                errors.append("Detail Distribution Cancelled");
                continue;
              }
            }
          }

          returnReturn.prepare("SELECT returnWoMaterial(:womatl_id, :qty, :itemlocSeries, :date, FALSE, TRUE, TRUE) AS result;");
          returnReturn.bindValue(":womatl_id", items.value("womatl_id").toInt());
          returnReturn.bindValue(":qty", items.value("qty").toDouble());
          returnReturn.bindValue(":itemlocSeries", itemlocSeries);
          returnReturn.bindValue(":date",  _transDate->date());
          returnReturn.exec();
          if (returnReturn.first())
          {
            if (returnReturn.value("result").toInt() < 0)
            {
              cleanup.exec();
              failedItems.append(items.value("item_number").toString());
              errors.append(tr("Return WO Material failed. %1")
                .arg(returnReturn.lastError().text()));
              continue;
            }
          }
          else
          {
            ErrorReporter::error(QtCriticalMsg, this, tr("Error Returning Work Order Material Batch, W/O ID #%1")
                                 .arg(_wo->id()),returnReturn, __FILE__, __LINE__);\
            return;
          }
          succeeded++;
        }

        if (errors.size() > 0)
        {
          QMessageBox dlg(QMessageBox::Critical, "Errors Issuing Material", "", QMessageBox::Ok, this);
          dlg.setText(tr("%1 Items succeeded.\n%2 Items failed.").arg(succeeded).arg(failedItems.size()));

          QString details;
          for (int i=0; i<failedItems.size(); i++)
            details += tr("Item %1 failed with:\n%2\n").arg(failedItems[i]).arg(errors[i]);
          dlg.setDetailedText(details);

          dlg.exec();
        }
      }
    }
  }

  if (_captive)
    accept();
  else
  {
    _wo->setId(-1);
    _wo->setFocus();
  }
}
