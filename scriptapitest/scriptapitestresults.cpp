/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2017 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "scriptapitestresults.h"

#include "format.h"

#include "char.h"
#include "contactclustersetup.h"
#include "engineevaluate.h"
#include "include.h"
#include "jsconsole.h"
#include "metasqlhighlighterproto.h"
#include "orreportproto.h"
#include "parametereditproto.h"
#include "parameterlistsetup.h"
#include "qabstractsocketproto.h"
#include "qactionproto.h"
#include "qapplicationproto.h"
#include "qboxlayoutproto.h"
#include "qbufferproto.h"
#include "qbuttongroupproto.h"
#include "qbytearrayproto.h"
#include "qcoreapplicationproto.h"
#include "qcryptographichashproto.h"
#include "qdatawidgetmapperproto.h"
#include "qdateproto.h"
#include "qdialogbuttonboxproto.h"
#include "qdialogsetup.h"
#include "qdirproto.h"
#include "qdnsdomainnamerecordproto.h"
#include "qdnshostaddressrecordproto.h"
#include "qdnslookupproto.h"
#include "qdnsmailexchangerecordproto.h"
#include "qdnsservicerecordproto.h"
#include "qdnstextrecordproto.h"
#include "qdockwidgetproto.h"
#include "qdomattrproto.h"
#include "qdomcdatasectionproto.h"
#include "qdomcharacterdataproto.h"
#include "qdomcommentproto.h"
#include "qdomdocumentfragmentproto.h"
#include "qdomdocumentproto.h"
#include "qdomdocumenttypeproto.h"
#include "qdomelementproto.h"
#include "qdomentityproto.h"
#include "qdomentityreferenceproto.h"
#include "qdomimplementationproto.h"
#include "qdomnamednodemapproto.h"
#include "qdomnodelistproto.h"
#include "qdomnodeproto.h"
#include "qdomnotationproto.h"
#include "qdomprocessinginstructionproto.h"
#include "qdomtextproto.h"
#include "qdoublevalidatorproto.h"
#include "qeventloopproto.h"
#include "qeventproto.h"
#include "qfileinfoproto.h"
#include "qfileproto.h"
#include "qfontproto.h"
#include "qformlayoutproto.h"
#include "qgridlayoutproto.h"
#include "qhostaddressproto.h"
#include "qhostinfoproto.h"
#include "qiconproto.h"
#include "qintvalidatorproto.h"
#include "qiodeviceproto.h"
#include "qitemdelegateproto.h"
#include "qjsondocumentproto.h"
#include "qjsonobjectproto.h"
#include "qjsonvalueproto.h"
#include "qlayoutitemproto.h"
#include "qlayoutproto.h"
#include "qmainwindowproto.h"
#include "qmenubarproto.h"
#include "qmenuproto.h"
#include "qmessageboxsetup.h"
#include "qmimedatabaseproto.h"
#include "qmimetypeproto.h"
#include "qnetworkaccessmanagerproto.h"
#include "qnetworkinterfaceproto.h"
#include "qnetworkreplyproto.h"
#include "qnetworkrequestproto.h"
#include "qobjectproto.h"
#include "qprinterproto.h"
#include "qprocessenvironmentproto.h"
#include "qprocessproto.h"
#include "qpushbuttonproto.h"
#include "qserialportinfoproto.h"
#include "qserialportproto.h"
#include "qsizepolicyproto.h"
#include "qspaceritemproto.h"
#include "qsqldatabaseproto.h"
#include "qsqldriverproto.h"
#include "qsqlerrorproto.h"
#include "qsqlproto.h"
#include "qsqlqueryproto.h"
#include "qsqlrecordproto.h"
#include "qsqltablemodelproto.h"
#include "qsslcertificateextensionproto.h"
#include "qsslcertificateproto.h"
#include "qsslcipherproto.h"
#include "qsslconfigurationproto.h"
#include "qsslellipticcurveproto.h"
#include "qsslerrorproto.h"
#include "qsslkeyproto.h"
#include "qsslpresharedkeyauthenticatorproto.h"
#include "qsslproto.h"
#include "qsslsocketproto.h"
#include "qstackedwidgetproto.h"
#include "qtabwidgetproto.h"
#include "qtcpserverproto.h"
#include "qtcpsocketproto.h"
#include "qtextdocumentproto.h"
#include "qtexteditproto.h"
#include "qtimerproto.h"
#include "qtoolbarproto.h"
#include "qtoolbuttonproto.h"
#include "qtreewidgetitemproto.h"
#include "qtsetup.h"
#include "qudpsocketproto.h"
#include "qurlproto.h"
#include "qurlqueryproto.h"
#include "quuidproto.h"
#include "qvalidatorproto.h"
#include "qwebchannelproto.h"
#if QT_VERSION < 0x050900
#include "qwebelementcollectionproto.h"
#include "qwebelementproto.h"
#include "qwebframeproto.h"
#include "qwebpageproto.h"
#include "qwebsecurityoriginproto.h"
#include "qwebsettingsproto.h"
#endif
#include "qwebsocketcorsauthenticatorproto.h"
#include "qwebsocketproto.h"
#include "qwebsocketprotocolproto.h"
#include "qwebsocketserverproto.h"
#if QT_VERSION < 0x050900
#include "qwebviewproto.h"
#endif
#include "qwidgetproto.h"
#include "scriptapi_internal.h"
#include "scriptapitestresults.h"
#include "setupscriptapi.h"
#include "webchanneltransport.h"
#include "xsqlqueryproto.h"
#include "xvariantsetup.h"
#include "xwebsync.h"
#include "xwebsync_p.h"

class ScriptApiTestResultsPrivate {
  public:
    QScriptEngine *engine;

    QMap <QString, QString> tests;

    ScriptApiTestResultsPrivate(QScriptEngine *pEngine)
      : engine(pEngine)
    {
      tests["_setup"] =
          "function assert(expr) {"
          "  if (! expr) throw new Error('falsey expression');"
          "}"
          "function assertIsFunction(f, name) {"
          "  if (! f) throw new Error(name + ' is falsey');"
          "  if (typeof f != 'function') throw new Error(name + ' is not a function');"
          "}"
          "function assertIsConstructor(f, name, args) {"
          "  assertIsFunction(f, name);"
          "  var tmp;"
          "  var args;"
          "  if (!args || !args.length)"
          "    tmp = f();"
          "  else"
          "  switch (args.length) {"
          "    case 1: tmp = f(args[0]);                    break;"
          "    case 2: tmp = f(args[0], args[1]);           break;"
          "    case 3: tmp = f(args[0], args[1], args[2]);  break;"
          "    default:"
          "      tmp = f(args[0], args[1], args[2]);"
          "      print('ignoring more than 3 args');"
          "  }"
          "}"
          "function assertIsNotConstructor(f, name) {"
          "try {"
          "  var tmp = new f();"
          "  throw new Error(name + ' appears to be a constructor');"
          "} catch (e) {}"
          "}"
          "function assertHasProperty(obj, name, prop) {"
          "  if (! prop in obj) throw new Error(prop + ' is not a property of ' + name);"
          "}"
          ;
      tests["char"] = "throw new Error('do not know how to test char');"
        ;
      tests["contactclustersetup"] =
        "try { var tmp = ContactCluster;"
        "if (! tmp) throw new Error('ContactCluster should be exposed in widgets, not scriptapi');"
        "} catch (e) { if (e instanceof ReferenceError) true ; else throw e ; }"
        ;
      tests["engineEvaluate"] =
          "assertIsFunction(engineEvaluate, 'engineEvaluate');"
        ;
      tests["include"] =
          "assertIsFunction(include, 'include');"
          "include('storedProcErrorLookup'); // should be in public in all dbs"
          ;
      tests["jsconsole"] =
          "var error, info, log, warn, print, tmp;"
          "assertIsFunction(print, 'print'); print('print print');"
          "assertIsFunction(error, 'error'); error('print error');"
          "assertIsFunction(info,  'info');  info ('print info');"
          "assertIsFunction(log,   'log');   log; ('print log');"
          "assertIsFunction(warn,  'warn');  warn ('print warn');"
          ;
      tests["metasqlhighlighter"] =
          "assertIsConstructor(MetaSQLHighlighter, 'MetaSQLHighlighter');"
          "var highlt = new MetaSQLHighlighter();"
          ;
      tests["orreportproto"] =
          "assertIsConstructor(orReport, 'orReport');"
          "var tmp = new orReport();"
          "assertHasProperty(tmp, 'orReport', 'beginMultiPrint');"
          "assertHasProperty(tmp, 'orReport', 'endMultiPrint');"
          "tmp = new orReport('Alignment');"
          "["
          "'backgroundAlignment', 'backgroundImage', 'isValid',"
          " 'print', 'reportError', 'watermarkText'"
          "].forEach(function (e) {"
          "  assertIsFunction(tmp[e], e);"
        "});"
          ;
     tests["parametereditproto"] =
         "assertIsConstructor(ParameterEdit, 'ParameterEdit');"
         " var tmp = new ParameterEdit();"
         ;
     /*
     tests["parameterlistsetup"] =
        "assertIsConstructor(ParameterList, 'ParameterList');"
        "var tmp = new ParameterList();"
        ;
      */
     tests["qabstractsocket"] = 
        "assertIsNotConstructor(QAbstractSocket, 'QAbstractSocket');"
        "var tmp = QAbstractSocket;"
        "['ShareAddress', 'IPv4Protocol', 'PauseNever',"
        " 'ConnectionRefusedError', 'LowDelayOption', 'UnconnectedState',"
        " 'TcpSocket'].forEach(function (e) {"
        "  assertHasProperty(QAbstractSocket, 'QAbstractSocket', e);"
        "});"
	;
     tests["qactionproto"] =
        "['Trigger', 'NoRole'].forEach(function (e) {"
        "  assertHasProperty(QAction, 'QAction', e);"
        "});"
        "var pb   = new QPushButton(); print(pb);"
        "assertIsConstructor(QAction, 'QAction', [pb]);"
        "var act  = new QAction(pb);"
        "['activate', 'data', 'setData'].forEach(function (e) {"
        "  assertIsFunction(act[e], e);"
        "});"
        "var act2 = new QAction('TestQAction', pb);"
        "assert(act2);"
        ;
     tests["qapplicationproto"] =
        "var tmp = QApplication;"       // no constructor yet
        ;
      /*
     tests["qboxlayoutproto"] =
	"assertIsConstructor(qboxlayout, 'qboxlayout');"
	"var tmp = new qboxlayout();"
	;
     tests["qbuffer"] = 
	"assertIsConstructor(qbuffer, 'qbuffer');"
	"var tmp = new qbuffer();"
	;
     tests["qbuttongroup"] = 
	"assertIsConstructor(qbuttongroup, 'qbuttongroup');"
	"var tmp = new qbuttongroup();"
	;
     tests["qbytearray"] = 
	"assertIsConstructor(qbytearray, 'qbytearray');"
	"var tmp = new qbytearray();"
	;
     tests["qcoreapplication"] = 
	"assertIsConstructor(qcoreapplication, 'qcoreapplication');"
	"var tmp = new qcoreapplication();"
	;
     tests["qcryptographichash"] = 
	"assertIsConstructor(qcryptographichash, 'qcryptographichash');"
	"var tmp = new qcryptographichash();"
	;
     tests["qdatawidgetmapper"] = 
	"assertIsConstructor(qdatawidgetmapper, 'qdatawidgetmapper');"
	"var tmp = new qdatawidgetmapper();"
	;
     tests["qdate"] = 
	"assertIsConstructor(qdate, 'qdate');"
	"var tmp = new qdate();"
	;
     tests["qdialogbuttonbox"] = 
	"assertIsConstructor(qdialogbuttonbox, 'qdialogbuttonbox');"
	"var tmp = new qdialogbuttonbox();"
	;
     tests["qdialogsetup"] = 
	"assertIsConstructor(qdialogsetup, 'qdialogsetup');"
	"var tmp = new qdialogsetup();"
	;
     tests["qdir"] = 
	"assertIsConstructor(qdir, 'qdir');"
	"var tmp = new qdir();"
	;
     tests["qdnsdomainnamerecord"] = 
	"assertIsConstructor(qdnsdomainnamerecord, 'qdnsdomainnamerecord');"
	"var tmp = new qdnsdomainnamerecord();"
	;
     tests["qdnshostaddressrecord"] = 
	"assertIsConstructor(qdnshostaddressrecord, 'qdnshostaddressrecord');"
	"var tmp = new qdnshostaddressrecord();"
	;
     tests["qdnslookup"] = 
	"assertIsConstructor(qdnslookup, 'qdnslookup');"
	"var tmp = new qdnslookup();"
	;
     tests["qdnsmailexchangerecord"] = 
	"assertIsConstructor(qdnsmailexchangerecord, 'qdnsmailexchangerecord');"
	"var tmp = new qdnsmailexchangerecord();"
	;
     tests["qdnsservicerecord"] = 
	"assertIsConstructor(qdnsservicerecord, 'qdnsservicerecord');"
	"var tmp = new qdnsservicerecord();"
	;
     tests["qdnstextrecord"] = 
	"assertIsConstructor(qdnstextrecord, 'qdnstextrecord');"
	"var tmp = new qdnstextrecord();"
	;
     tests["qdockwidget"] = 
	"assertIsConstructor(qdockwidget, 'qdockwidget');"
	"var tmp = new qdockwidget();"
	;
     tests["qdomattr"] = 
	"assertIsConstructor(qdomattr, 'qdomattr');"
	"var tmp = new qdomattr();"
	;
     tests["qdomcdatasection"] = 
	"assertIsConstructor(qdomcdatasection, 'qdomcdatasection');"
	"var tmp = new qdomcdatasection();"
	;
     tests["qdomcharacterdata"] = 
	"assertIsConstructor(qdomcharacterdata, 'qdomcharacterdata');"
	"var tmp = new qdomcharacterdata();"
	;
     tests["qdomcomment"] = 
	"assertIsConstructor(qdomcomment, 'qdomcomment');"
	"var tmp = new qdomcomment();"
	;
     tests["qdomdocumentfragment"] = 
	"assertIsConstructor(qdomdocumentfragment, 'qdomdocumentfragment');"
	"var tmp = new qdomdocumentfragment();"
	;
     tests["qdomdocument"] = 
	"assertIsConstructor(qdomdocument, 'qdomdocument');"
	"var tmp = new qdomdocument();"
	;
     tests["qdomdocumenttype"] = 
	"assertIsConstructor(qdomdocumenttype, 'qdomdocumenttype');"
	"var tmp = new qdomdocumenttype();"
	;
     tests["qdomelement"] = 
	"assertIsConstructor(qdomelement, 'qdomelement');"
	"var tmp = new qdomelement();"
	;
     tests["qdomentity"] = 
	"assertIsConstructor(qdomentity, 'qdomentity');"
	"var tmp = new qdomentity();"
	;
     tests["qdomentityreference"] = 
	"assertIsConstructor(qdomentityreference, 'qdomentityreference');"
	"var tmp = new qdomentityreference();"
	;
     tests["qdomimplementation"] = 
	"assertIsConstructor(qdomimplementation, 'qdomimplementation');"
	"var tmp = new qdomimplementation();"
	;
     tests["qdomnamednodemap"] = 
	"assertIsConstructor(qdomnamednodemap, 'qdomnamednodemap');"
	"var tmp = new qdomnamednodemap();"
	;
     tests["qdomnodelist"] = 
	"assertIsConstructor(qdomnodelist, 'qdomnodelist');"
	"var tmp = new qdomnodelist();"
	;
     tests["qdomnode"] = 
	"assertIsConstructor(qdomnode, 'qdomnode');"
	"var tmp = new qdomnode();"
	;
     tests["qdomnotation"] = 
	"assertIsConstructor(qdomnotation, 'qdomnotation');"
	"var tmp = new qdomnotation();"
	;
     tests["qdomprocessinginstruction"] = 
	"assertIsConstructor(qdomprocessinginstruction, 'qdomprocessinginstruction');"
	"var tmp = new qdomprocessinginstruction();"
	;
     tests["qdomtext"] = 
	"assertIsConstructor(qdomtext, 'qdomtext');"
	"var tmp = new qdomtext();"
	;
     tests["qdoublevalidator"] = 
	"assertIsConstructor(qdoublevalidator, 'qdoublevalidator');"
	"var tmp = new qdoublevalidator();"
	;
        */
     tests["qeventloopproto"] =
        "assertIsConstructor(QEventLoop, 'QEventLoop');"
        "var tmp = new QEventLoop();"
        "['AllEvents', 'WaitForMoreEvents'].forEach(function (e) {"
        "  assertHasProperty(tmp, 'QEventLoop', e);"
        "});"
        "['exec', 'exit', 'isRunning', 'wakeUp'].forEach(function (e) {"
        "  assertIsFunction(tmp[e], e, 'e');"
        "});"
	;
     /*
     tests["qevent"] = 
	"assertIsConstructor(qevent, 'qevent');"
	"var tmp = new qevent();"
	;
     tests["qfileinfo"] = 
	"assertIsConstructor(qfileinfo, 'qfileinfo');"
	"var tmp = new qfileinfo();"
	;
     tests["qfile"] = 
	"assertIsConstructor(qfile, 'qfile');"
	"var tmp = new qfile();"
	;
     tests["qfont"] = 
	"assertIsConstructor(qfont, 'qfont');"
	"var tmp = new qfont();"
	;
     tests["qformlayout"] = 
	"assertIsConstructor(qformlayout, 'qformlayout');"
	"var tmp = new qformlayout();"
	;
     tests["qgridlayout"] = 
	"assertIsConstructor(qgridlayout, 'qgridlayout');"
	"var tmp = new qgridlayout();"
	;
     tests["qhostaddress"] = 
	"assertIsConstructor(qhostaddress, 'qhostaddress');"
	"var tmp = new qhostaddress();"
	;
     tests["qhostinfo"] = 
	"assertIsConstructor(qhostinfo, 'qhostinfo');"
	"var tmp = new qhostinfo();"
	;
     tests["qicon"] = 
	"assertIsConstructor(qicon, 'qicon');"
	"var tmp = new qicon();"
	;
     tests["qintvalidator"] = 
	"assertIsConstructor(qintvalidator, 'qintvalidator');"
	"var tmp = new qintvalidator();"
	;
     tests["qiodevice"] = 
	"assertIsConstructor(qiodevice, 'qiodevice');"
	"var tmp = new qiodevice();"
	;
     tests["qitemdelegate"] = 
	"assertIsConstructor(qitemdelegate, 'qitemdelegate');"
	"var tmp = new qitemdelegate();"
	;
     tests["qjsondocument"] = 
	"assertIsConstructor(qjsondocument, 'qjsondocument');"
	"var tmp = new qjsondocument();"
	;
     tests["qjsonobject"] = 
	"assertIsConstructor(qjsonobject, 'qjsonobject');"
	"var tmp = new qjsonobject();"
	;
     tests["qjsonvalue"] = 
	"assertIsConstructor(qjsonvalue, 'qjsonvalue');"
	"var tmp = new qjsonvalue();"
	;
     tests["qlayoutitem"] = 
	"assertIsConstructor(qlayoutitem, 'qlayoutitem');"
	"var tmp = new qlayoutitem();"
	;
     tests["qlayout"] = 
	"assertIsConstructor(qlayout, 'qlayout');"
	"var tmp = new qlayout();"
	;
     tests["qmainwindow"] = 
	"assertIsConstructor(qmainwindow, 'qmainwindow');"
	"var tmp = new qmainwindow();"
	;
     tests["qmenubar"] = 
	"assertIsConstructor(qmenubar, 'qmenubar');"
	"var tmp = new qmenubar();"
	;
     tests["qmenu"] = 
	"assertIsConstructor(qmenu, 'qmenu');"
	"var tmp = new qmenu();"
	;
     tests["qmessageboxsetup"] = 
	"assertIsConstructor(qmessageboxsetup, 'qmessageboxsetup');"
	"var tmp = new qmessageboxsetup();"
	;
     tests["qmimedatabase"] = 
	"assertIsConstructor(qmimedatabase, 'qmimedatabase');"
	"var tmp = new qmimedatabase();"
	;
     tests["qmimetype"] = 
	"assertIsConstructor(qmimetype, 'qmimetype');"
	"var tmp = new qmimetype();"
	;
     tests["qnetworkaccessmanager"] = 
	"assertIsConstructor(qnetworkaccessmanager, 'qnetworkaccessmanager');"
	"var tmp = new qnetworkaccessmanager();"
	;
     tests["qnetworkinterface"] = 
	"assertIsConstructor(qnetworkinterface, 'qnetworkinterface');"
	"var tmp = new qnetworkinterface();"
	;
     tests["qnetworkreply"] = 
	"assertIsConstructor(qnetworkreply, 'qnetworkreply');"
	"var tmp = new qnetworkreply();"
	;
     tests["qnetworkrequest"] = 
	"assertIsConstructor(qnetworkrequest, 'qnetworkrequest');"
	"var tmp = new qnetworkrequest();"
	;
     tests["qobject"] = 
	"assertIsConstructor(qobject, 'qobject');"
	"var tmp = new qobject();"
	;
     tests["qprinter"] = 
	"assertIsConstructor(qprinter, 'qprinter');"
	"var tmp = new qprinter();"
	;
     tests["qprocessenvironment"] = 
	"assertIsConstructor(qprocessenvironment, 'qprocessenvironment');"
	"var tmp = new qprocessenvironment();"
	;
     tests["qprocess"] = 
	"assertIsConstructor(qprocess, 'qprocess');"
	"var tmp = new qprocess();"
	;
     tests["qpushbutton"] = 
	"assertIsConstructor(qpushbutton, 'qpushbutton');"
	"var tmp = new qpushbutton();"
	;
     tests["qserialportinfo"] = 
	"assertIsConstructor(qserialportinfo, 'qserialportinfo');"
	"var tmp = new qserialportinfo();"
	;
     tests["qserialport"] = 
	"assertIsConstructor(qserialport, 'qserialport');"
	"var tmp = new qserialport();"
	;
     tests["qsizepolicy"] = 
	"assertIsConstructor(qsizepolicy, 'qsizepolicy');"
	"var tmp = new qsizepolicy();"
	;
     tests["qspaceritem"] = 
	"assertIsConstructor(qspaceritem, 'qspaceritem');"
	"var tmp = new qspaceritem();"
	;
     tests["qsqldatabase"] = 
	"assertIsConstructor(qsqldatabase, 'qsqldatabase');"
	"var tmp = new qsqldatabase();"
	;
     tests["qsqldriver"] = 
	"assertIsConstructor(qsqldriver, 'qsqldriver');"
	"var tmp = new qsqldriver();"
	;
     tests["qsqlerror"] = 
	"assertIsConstructor(qsqlerror, 'qsqlerror');"
	"var tmp = new qsqlerror();"
	;
     tests["qsql"] = 
	"assertIsConstructor(qsql, 'qsql');"
	"var tmp = new qsql();"
	;
     tests["qsqlquery"] = 
	"assertIsConstructor(qsqlquery, 'qsqlquery');"
	"var tmp = new qsqlquery();"
	;
     tests["qsqlrecord"] = 
	"assertIsConstructor(qsqlrecord, 'qsqlrecord');"
	"var tmp = new qsqlrecord();"
	;
     tests["qsqltablemodel"] = 
	"assertIsConstructor(qsqltablemodel, 'qsqltablemodel');"
	"var tmp = new qsqltablemodel();"
	;
     tests["qsslcertificateextension"] = 
	"assertIsConstructor(qsslcertificateextension, 'qsslcertificateextension');"
	"var tmp = new qsslcertificateextension();"
	;
     tests["qsslcertificate"] = 
	"assertIsConstructor(qsslcertificate, 'qsslcertificate');"
	"var tmp = new qsslcertificate();"
	;
     tests["qsslcipher"] = 
	"assertIsConstructor(qsslcipher, 'qsslcipher');"
	"var tmp = new qsslcipher();"
	;
     tests["qsslconfiguration"] = 
	"assertIsConstructor(qsslconfiguration, 'qsslconfiguration');"
	"var tmp = new qsslconfiguration();"
	;
     tests["qsslellipticcurve"] = 
	"assertIsConstructor(qsslellipticcurve, 'qsslellipticcurve');"
	"var tmp = new qsslellipticcurve();"
	;
     tests["qsslerror"] = 
	"assertIsConstructor(qsslerror, 'qsslerror');"
	"var tmp = new qsslerror();"
	;
     tests["qsslkey"] = 
	"assertIsConstructor(qsslkey, 'qsslkey');"
	"var tmp = new qsslkey();"
	;
     tests["qsslpresharedkeyauthenticator"] = 
	"assertIsConstructor(qsslpresharedkeyauthenticator, 'qsslpresharedkeyauthenticator');"
	"var tmp = new qsslpresharedkeyauthenticator();"
	;
     tests["qssl"] = 
	"assertIsConstructor(qssl, 'qssl');"
	"var tmp = new qssl();"
	;
     tests["qsslsocket"] = 
	"assertIsConstructor(qsslsocket, 'qsslsocket');"
	"var tmp = new qsslsocket();"
	;
     tests["qstackedwidget"] = 
	"assertIsConstructor(qstackedwidget, 'qstackedwidget');"
	"var tmp = new qstackedwidget();"
	;
     tests["qtabwidget"] = 
	"assertIsConstructor(qtabwidget, 'qtabwidget');"
	"var tmp = new qtabwidget();"
	;
     tests["qtcpserver"] = 
	"assertIsConstructor(qtcpserver, 'qtcpserver');"
	"var tmp = new qtcpserver();"
	;
     tests["qtcpsocket"] = 
	"assertIsConstructor(qtcpsocket, 'qtcpsocket');"
	"var tmp = new qtcpsocket();"
	;
     tests["qtextdocument"] = 
	"assertIsConstructor(qtextdocument, 'qtextdocument');"
	"var tmp = new qtextdocument();"
	;
     tests["qtextedit"] = 
	"assertIsConstructor(qtextedit, 'qtextedit');"
	"var tmp = new qtextedit();"
	;
     tests["qtimer"] = 
	"assertIsConstructor(qtimer, 'qtimer');"
	"var tmp = new qtimer();"
	;
     tests["qtoolbar"] = 
	"assertIsConstructor(qtoolbar, 'qtoolbar');"
	"var tmp = new qtoolbar();"
	;
     tests["qtoolbutton"] = 
	"assertIsConstructor(qtoolbutton, 'qtoolbutton');"
	"var tmp = new qtoolbutton();"
	;
     tests["qtreewidgetitem"] = 
	"assertIsConstructor(qtreewidgetitem, 'qtreewidgetitem');"
	"var tmp = new qtreewidgetitem();"
	;
     tests["qtsetup"] = 
	"assertIsConstructor(qtsetup, 'qtsetup');"
	"var tmp = new qtsetup();"
	;
     tests["qudpsocket"] = 
	"assertIsConstructor(qudpsocket, 'qudpsocket');"
	"var tmp = new qudpsocket();"
	;
     tests["qurl"] = 
	"assertIsConstructor(qurl, 'qurl');"
	"var tmp = new qurl();"
	;
     tests["qurlquery"] = 
	"assertIsConstructor(qurlquery, 'qurlquery');"
	"var tmp = new qurlquery();"
	;
     tests["quuid"] = 
	"assertIsConstructor(quuid, 'quuid');"
	"var tmp = new quuid();"
	;
     tests["qvalidator"] = 
	"assertIsConstructor(qvalidator, 'qvalidator');"
	"var tmp = new qvalidator();"
	;
     tests["qwebchannel"] = 
	"assertIsConstructor(qwebchannel, 'qwebchannel');"
	"var tmp = new qwebchannel();"
	;
#if QT_VERSION < 0x050900
     tests["qwebelementcollection"] = 
	"assertIsConstructor(qwebelementcollection, 'qwebelementcollection');"
	"var tmp = new qwebelementcollection();"
	;
     tests["qwebelement"] = 
	"assertIsConstructor(qwebelement, 'qwebelement');"
	"var tmp = new qwebelement();"
	;
     tests["qwebframe"] = 
	"assertIsConstructor(qwebframe, 'qwebframe');"
	"var tmp = new qwebframe();"
	;
     tests["qwebpage"] = 
	"assertIsConstructor(qwebpage, 'qwebpage');"
	"var tmp = new qwebpage();"
	;
     tests["qwebsecurityorigin"] = 
	"assertIsConstructor(qwebsecurityorigin, 'qwebsecurityorigin');"
	"var tmp = new qwebsecurityorigin();"
	;
#endif
     tests["qwebsettings"] = 
	"assertIsConstructor(qwebsettings, 'qwebsettings');"
	"var tmp = new qwebsettings();"
	;
     tests["qwebsocketcorsauthenticator"] = 
	"assertIsConstructor(qwebsocketcorsauthenticator, 'qwebsocketcorsauthenticator');"
	"var tmp = new qwebsocketcorsauthenticator();"
	;
     tests["qwebsocket"] = 
	"assertIsConstructor(qwebsocket, 'qwebsocket');"
	"var tmp = new qwebsocket();"
	;
     tests["qwebsocketcol"] = 
	"assertIsConstructor(qwebsocketcol, 'qwebsocketcol');"
	"var tmp = new qwebsocketcol();"
	;
     tests["qwebsocketserver"] = 
	"assertIsConstructor(qwebsocketserver, 'qwebsocketserver');"
	"var tmp = new qwebsocketserver();"
	;
     tests["qwebview"] = 
	"assertIsConstructor(qwebview, 'qwebview');"
	"var tmp = new qwebview();"
	;
     tests["qwidget"] = 
	"assertIsConstructor(qwidget, 'qwidget');"
	"var tmp = new qwidget();"
	;
     tests["scriptapi_internal"] = 
	"assertIsConstructor(scriptapi_internal, 'scriptapi_internal');"
	"var tmp = new scriptapi_internal();"
	;
     tests["scriptapitestresults"] = 
	"assertIsConstructor(scriptapitestresults, 'scriptapitestresults');"
	"var tmp = new scriptapitestresults();"
	;
     tests["setupscriptapi"] = 
	"assertIsConstructor(setupscriptapi, 'setupscriptapi');"
	"var tmp = new setupscriptapi();"
	;
     tests["webchanneltransport"] = 
	"assertIsConstructor(webchanneltransport, 'webchanneltransport');"
	"var tmp = new webchanneltransport();"
	;
     tests["xsqlquery"] = 
	"assertIsConstructor(xsqlquery, 'xsqlquery');"
	"var tmp = new xsqlquery();"
	;
     tests["xvariantsetup"] = 
	"assertIsConstructor(xvariantsetup, 'xvariantsetup');"
	"var tmp = new xvariantsetup();"
	;
     tests["xwebsync"] = 
	"assertIsConstructor(xwebsync, 'xwebsync');"
	"var tmp = new xwebsync();"
	;
     tests["xwebsync_p"] = 
	"assertIsConstructor(xwebsync_p, 'xwebsync_p');"
	"var tmp = new xwebsync_p();"
	;
      */
        }
    };

ScriptApiTestResults::ScriptApiTestResults(QScriptEngine *engine,
                                           QWidget *parent)
  : QMainWindow(parent)
{
  setupUi(this);
  _data = new ScriptApiTestResultsPrivate(engine);

  QMapIterator<QString, QString> i(_data->tests);
  while (i.hasNext())
  {
    i.next();
    QLabel    *label = new QLabel(i.key(), this);
    QTextEdit *edit  = new QTextEdit(this);
    edit->setMaximumHeight(20);
    QScriptValue result = engine->evaluate(i.value(), i.key());
    if (engine->hasUncaughtException())
    {
      edit->setTextColor(namedColor("error"));
      edit->setPlainText(result.toString());
    }
    else
    {
      edit->setTextColor(namedColor("altEmphasis"));
      edit->setPlainText("Passed");
    }
    int row = _resultsGrid->rowCount();
    _resultsGrid->addWidget(label, row, 0);
    _resultsGrid->addWidget(edit,  row, 1);
  }
}

ScriptApiTestResults::~ScriptApiTestResults()
{
}
