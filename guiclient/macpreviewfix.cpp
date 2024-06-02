/****************************************************************************
 *
 * @author Dilshod Mukhtarov <dilshodm(at)gmail.com>
 * Jun 2024
 *
 ****************************************************************************/

#include "macpreviewfix.h"
#include <QPrinter>

bool isMacPrintPreview(QPrinter *printer)
{
#ifdef Q_OS_MAC
    return printer->printerName().isEmpty() && printer->outputFormat() == QPrinter::PdfFormat
        && printer->outputFileName().startsWith("/private");
#else
    return false;
#endif
}
