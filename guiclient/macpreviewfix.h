/****************************************************************************
 *
 * @author Dilshod Mukhtarov <dilshodm(at)gmail.com>
 * Jun 2024
 *
 ****************************************************************************/

#pragma once

class QPrinter;

/**
 * @brief isMacPrintPreview - Check if the printer is a Mac "Open preview" mode. In this mode
 * there must be no processEvents() calls, otherwise it might not display pdf file.
 * @param printer
 * @return true if it's a Mac print "Open in preview" mode.
 */
extern bool isMacPrintPreview(QPrinter *printer);
