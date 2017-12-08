/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2017 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "scriptapi_internal.h"
#include "qeventproto.h"

#include <QDebug>
#include <QString>

#define DEBUG false

QScriptValue constructQEvent(QScriptContext *context,
                            QScriptEngine  *engine)
{
  QEvent *obj = 0;

  if (DEBUG)
  {
    qDebug("constructQEvent() entered");
    for (int i = 0; i < context->argumentCount(); i++)
      qDebug() << "context->argument(" << i << ") = " << context->argument(i).toVariant();
  }

  if (context->argumentCount() == 1)
    obj = new QEvent(qscriptvalue_cast<QEvent::Type>(context->argument(0)));
  else
    obj = new QEvent(QEvent::None);

  return engine->toScriptValue(obj);
}

void setupQEventProto(QScriptEngine *engine)
{
  QScriptValue proto = engine->newQObject(new QEventProto(engine));
  engine->setDefaultPrototype(qMetaTypeId<QEvent*>(), proto);

  QScriptValue constructor = engine->newFunction(constructQEvent, proto);
  engine->globalObject().setProperty("QEvent", constructor);

  proto.setProperty("None",                             QScriptValue(engine, QEvent::None),                             ENUMPROPFLAGS);
  proto.setProperty("ActionAdded",                      QScriptValue(engine, QEvent::ActionAdded),                      ENUMPROPFLAGS);
  proto.setProperty("ActionChanged",                    QScriptValue(engine, QEvent::ActionChanged),                    ENUMPROPFLAGS);
  proto.setProperty("ActivationChange",                 QScriptValue(engine, QEvent::ActivationChange),                 ENUMPROPFLAGS);
  proto.setProperty("ApplicationActivate",              QScriptValue(engine, QEvent::ApplicationActivate),              ENUMPROPFLAGS);
  proto.setProperty("ApplicationActivated",             QScriptValue(engine, QEvent::ApplicationActivated),             ENUMPROPFLAGS);
  proto.setProperty("ApplicationDeactivate",            QScriptValue(engine, QEvent::ApplicationDeactivate),            ENUMPROPFLAGS);
  proto.setProperty("ApplicationFontChange",            QScriptValue(engine, QEvent::ApplicationFontChange),            ENUMPROPFLAGS);
  proto.setProperty("ApplicationLayoutDirectionChange", QScriptValue(engine, QEvent::ApplicationLayoutDirectionChange), ENUMPROPFLAGS);
  proto.setProperty("ApplicationPaletteChange",         QScriptValue(engine, QEvent::ApplicationPaletteChange),         ENUMPROPFLAGS);
  proto.setProperty("ApplicationStateChange",           QScriptValue(engine, QEvent::ApplicationStateChange),           ENUMPROPFLAGS);
  proto.setProperty("ApplicationWindowIconChange",      QScriptValue(engine, QEvent::ApplicationWindowIconChange),      ENUMPROPFLAGS);
  proto.setProperty("ChildAdded",                       QScriptValue(engine, QEvent::ChildAdded),                       ENUMPROPFLAGS);
  proto.setProperty("ChildPolished",                    QScriptValue(engine, QEvent::ChildPolished),                    ENUMPROPFLAGS);
  proto.setProperty("ChildRemoved",                     QScriptValue(engine, QEvent::ChildRemoved),                     ENUMPROPFLAGS);
  proto.setProperty("Clipboard",                        QScriptValue(engine, QEvent::Clipboard),                        ENUMPROPFLAGS);
  proto.setProperty("Close",                            QScriptValue(engine, QEvent::Close),                            ENUMPROPFLAGS);
  proto.setProperty("CloseSoftwareInputPanel",          QScriptValue(engine, QEvent::CloseSoftwareInputPanel),          ENUMPROPFLAGS);
  proto.setProperty("ContentsRectChange",               QScriptValue(engine, QEvent::ContentsRectChange),               ENUMPROPFLAGS);
  proto.setProperty("ContextMenu",                      QScriptValue(engine, QEvent::ContextMenu),                      ENUMPROPFLAGS);
  proto.setProperty("CursorChange",                     QScriptValue(engine, QEvent::CursorChange),                     ENUMPROPFLAGS);
  proto.setProperty("DeferredDelete",                   QScriptValue(engine, QEvent::DeferredDelete),                   ENUMPROPFLAGS);
  proto.setProperty("DragEnter",                        QScriptValue(engine, QEvent::DragEnter),                        ENUMPROPFLAGS);
  proto.setProperty("DragLeave",                        QScriptValue(engine, QEvent::DragLeave),                        ENUMPROPFLAGS);
  proto.setProperty("DragMove",                         QScriptValue(engine, QEvent::DragMove),                         ENUMPROPFLAGS);
  proto.setProperty("Drop",                             QScriptValue(engine, QEvent::Drop),                             ENUMPROPFLAGS);
  proto.setProperty("DynamicPropertyChange",            QScriptValue(engine, QEvent::DynamicPropertyChange),            ENUMPROPFLAGS);
  proto.setProperty("EnabledChange",                    QScriptValue(engine, QEvent::EnabledChange),                    ENUMPROPFLAGS);
  proto.setProperty("Enter",                            QScriptValue(engine, QEvent::Enter),                            ENUMPROPFLAGS);
#ifdef QT_KEYPAD_NAVIGATION
  proto.setProperty("EnterEditFocus",                   QScriptValue(engine, QEvent::EnterEditFocus),                   ENUMPROPFLAGS);
#endif
  proto.setProperty("EnterWhatsThisMode",               QScriptValue(engine, QEvent::EnterWhatsThisMode),               ENUMPROPFLAGS);
  proto.setProperty("Expose",                           QScriptValue(engine, QEvent::Expose),                           ENUMPROPFLAGS);
  proto.setProperty("FileOpen",                         QScriptValue(engine, QEvent::FileOpen),                         ENUMPROPFLAGS);
  proto.setProperty("FocusIn",                          QScriptValue(engine, QEvent::FocusIn),                          ENUMPROPFLAGS);
  proto.setProperty("FocusOut",                         QScriptValue(engine, QEvent::FocusOut),                         ENUMPROPFLAGS);
  proto.setProperty("FocusAboutToChange",               QScriptValue(engine, QEvent::FocusAboutToChange),               ENUMPROPFLAGS);
  proto.setProperty("FontChange",                       QScriptValue(engine, QEvent::FontChange),                       ENUMPROPFLAGS);
  proto.setProperty("Gesture",                          QScriptValue(engine, QEvent::Gesture),                          ENUMPROPFLAGS);
  proto.setProperty("GestureOverride",                  QScriptValue(engine, QEvent::GestureOverride),                  ENUMPROPFLAGS);
  proto.setProperty("GrabKeyboard",                     QScriptValue(engine, QEvent::GrabKeyboard),                     ENUMPROPFLAGS);
  proto.setProperty("GrabMouse",                        QScriptValue(engine, QEvent::GrabMouse),                        ENUMPROPFLAGS);
  proto.setProperty("GraphicsSceneContextMenu",         QScriptValue(engine, QEvent::GraphicsSceneContextMenu),         ENUMPROPFLAGS);
  proto.setProperty("GraphicsSceneDragEnter",           QScriptValue(engine, QEvent::GraphicsSceneDragEnter),           ENUMPROPFLAGS);
  proto.setProperty("GraphicsSceneDragLeave",           QScriptValue(engine, QEvent::GraphicsSceneDragLeave),           ENUMPROPFLAGS);
  proto.setProperty("GraphicsSceneDragMove",            QScriptValue(engine, QEvent::GraphicsSceneDragMove),            ENUMPROPFLAGS);
  proto.setProperty("GraphicsSceneDrop",                QScriptValue(engine, QEvent::GraphicsSceneDrop),                ENUMPROPFLAGS);
  proto.setProperty("GraphicsSceneHelp",                QScriptValue(engine, QEvent::GraphicsSceneHelp),                ENUMPROPFLAGS);
  proto.setProperty("GraphicsSceneHoverEnter",          QScriptValue(engine, QEvent::GraphicsSceneHoverEnter),          ENUMPROPFLAGS);
  proto.setProperty("GraphicsSceneHoverLeave",          QScriptValue(engine, QEvent::GraphicsSceneHoverLeave),          ENUMPROPFLAGS);
  proto.setProperty("GraphicsSceneHoverMove",           QScriptValue(engine, QEvent::GraphicsSceneHoverMove),           ENUMPROPFLAGS);
  proto.setProperty("GraphicsSceneMouseDoubleClick",    QScriptValue(engine, QEvent::GraphicsSceneMouseDoubleClick),    ENUMPROPFLAGS);
  proto.setProperty("GraphicsSceneMouseMove",           QScriptValue(engine, QEvent::GraphicsSceneMouseMove),           ENUMPROPFLAGS);
  proto.setProperty("GraphicsSceneMousePress",          QScriptValue(engine, QEvent::GraphicsSceneMousePress),          ENUMPROPFLAGS);
  proto.setProperty("GraphicsSceneMouseRelease",        QScriptValue(engine, QEvent::GraphicsSceneMouseRelease),        ENUMPROPFLAGS);
  proto.setProperty("GraphicsSceneMove",                QScriptValue(engine, QEvent::GraphicsSceneMove),                ENUMPROPFLAGS);
  proto.setProperty("GraphicsSceneResize",              QScriptValue(engine, QEvent::GraphicsSceneResize),              ENUMPROPFLAGS);
  proto.setProperty("GraphicsSceneWheel",               QScriptValue(engine, QEvent::GraphicsSceneWheel),               ENUMPROPFLAGS);
  proto.setProperty("Hide",                             QScriptValue(engine, QEvent::Hide),                             ENUMPROPFLAGS);
  proto.setProperty("HideToParent",                     QScriptValue(engine, QEvent::HideToParent),                     ENUMPROPFLAGS);
  proto.setProperty("HoverEnter",                       QScriptValue(engine, QEvent::HoverEnter),                       ENUMPROPFLAGS);
  proto.setProperty("HoverLeave",                       QScriptValue(engine, QEvent::HoverLeave),                       ENUMPROPFLAGS);
  proto.setProperty("HoverMove",                        QScriptValue(engine, QEvent::HoverMove),                        ENUMPROPFLAGS);
  proto.setProperty("IconDrag",                         QScriptValue(engine, QEvent::IconDrag),                         ENUMPROPFLAGS);
  proto.setProperty("IconTextChange",                   QScriptValue(engine, QEvent::IconTextChange),                   ENUMPROPFLAGS);
  proto.setProperty("InputMethod",                      QScriptValue(engine, QEvent::InputMethod),                      ENUMPROPFLAGS);
  proto.setProperty("InputMethodQuery",                 QScriptValue(engine, QEvent::InputMethodQuery),                 ENUMPROPFLAGS);
  proto.setProperty("KeyboardLayoutChange",             QScriptValue(engine, QEvent::KeyboardLayoutChange),             ENUMPROPFLAGS);
  proto.setProperty("KeyPress",                         QScriptValue(engine, QEvent::KeyPress),                         ENUMPROPFLAGS);
  proto.setProperty("KeyRelease",                       QScriptValue(engine, QEvent::KeyRelease),                       ENUMPROPFLAGS);
  proto.setProperty("LanguageChange",                   QScriptValue(engine, QEvent::LanguageChange),                   ENUMPROPFLAGS);
  proto.setProperty("LayoutDirectionChange",            QScriptValue(engine, QEvent::LayoutDirectionChange),            ENUMPROPFLAGS);
  proto.setProperty("LayoutRequest",                    QScriptValue(engine, QEvent::LayoutRequest),                    ENUMPROPFLAGS);
  proto.setProperty("Leave",                            QScriptValue(engine, QEvent::Leave),                            ENUMPROPFLAGS);
#ifdef QT_KEYPAD_NAVIGATION
  proto.setProperty("LeaveEditFocus",                   QScriptValue(engine, QEvent::LeaveEditFocus),                   ENUMPROPFLAGS);
#endif
  proto.setProperty("LeaveWhatsThisMode",               QScriptValue(engine, QEvent::LeaveWhatsThisMode),               ENUMPROPFLAGS);
  proto.setProperty("LocaleChange",                     QScriptValue(engine, QEvent::LocaleChange),                     ENUMPROPFLAGS);
  proto.setProperty("NonClientAreaMouseButtonDblClick", QScriptValue(engine, QEvent::NonClientAreaMouseButtonDblClick), ENUMPROPFLAGS);
  proto.setProperty("NonClientAreaMouseButtonPress",    QScriptValue(engine, QEvent::NonClientAreaMouseButtonPress),    ENUMPROPFLAGS);
  proto.setProperty("NonClientAreaMouseButtonRelease",  QScriptValue(engine, QEvent::NonClientAreaMouseButtonRelease),  ENUMPROPFLAGS);
  proto.setProperty("NonClientAreaMouseMove",           QScriptValue(engine, QEvent::NonClientAreaMouseMove),           ENUMPROPFLAGS);
  proto.setProperty("MacSizeChange",                    QScriptValue(engine, QEvent::MacSizeChange),                    ENUMPROPFLAGS);
  proto.setProperty("MetaCall",                         QScriptValue(engine, QEvent::MetaCall),                         ENUMPROPFLAGS);
  proto.setProperty("ModifiedChange",                   QScriptValue(engine, QEvent::ModifiedChange),                   ENUMPROPFLAGS);
  proto.setProperty("MouseButtonDblClick",              QScriptValue(engine, QEvent::MouseButtonDblClick),              ENUMPROPFLAGS);
  proto.setProperty("MouseButtonPress",                 QScriptValue(engine, QEvent::MouseButtonPress),                 ENUMPROPFLAGS);
  proto.setProperty("MouseButtonRelease",               QScriptValue(engine, QEvent::MouseButtonRelease),               ENUMPROPFLAGS);
  proto.setProperty("MouseMove",                        QScriptValue(engine, QEvent::MouseMove),                        ENUMPROPFLAGS);
  proto.setProperty("MouseTrackingChange",              QScriptValue(engine, QEvent::MouseTrackingChange),              ENUMPROPFLAGS);
  proto.setProperty("Move",                             QScriptValue(engine, QEvent::Move),                             ENUMPROPFLAGS);
  proto.setProperty("NativeGesture",                    QScriptValue(engine, QEvent::NativeGesture),                    ENUMPROPFLAGS);
  proto.setProperty("OrientationChange",                QScriptValue(engine, QEvent::OrientationChange),                ENUMPROPFLAGS);
  proto.setProperty("Paint",                            QScriptValue(engine, QEvent::Paint),                            ENUMPROPFLAGS);
  proto.setProperty("PaletteChange",                    QScriptValue(engine, QEvent::PaletteChange),                    ENUMPROPFLAGS);
  proto.setProperty("ParentAboutToChange",              QScriptValue(engine, QEvent::ParentAboutToChange),              ENUMPROPFLAGS);
  proto.setProperty("ParentChange",                     QScriptValue(engine, QEvent::ParentChange),                     ENUMPROPFLAGS);
  proto.setProperty("PlatformPanel",                    QScriptValue(engine, QEvent::PlatformPanel),                    ENUMPROPFLAGS);
  proto.setProperty("PlatformSurface",                  QScriptValue(engine, QEvent::PlatformSurface),                  ENUMPROPFLAGS);
  proto.setProperty("Polish",                           QScriptValue(engine, QEvent::Polish),                           ENUMPROPFLAGS);
  proto.setProperty("PolishRequest",                    QScriptValue(engine, QEvent::PolishRequest),                    ENUMPROPFLAGS);
  proto.setProperty("QueryWhatsThis",                   QScriptValue(engine, QEvent::QueryWhatsThis),                   ENUMPROPFLAGS);
  proto.setProperty("ReadOnlyChange",                   QScriptValue(engine, QEvent::ReadOnlyChange),                   ENUMPROPFLAGS);
  proto.setProperty("RequestSoftwareInputPanel",        QScriptValue(engine, QEvent::RequestSoftwareInputPanel),        ENUMPROPFLAGS);
  proto.setProperty("Resize",                           QScriptValue(engine, QEvent::Resize),                           ENUMPROPFLAGS);
  proto.setProperty("ScrollPrepare",                    QScriptValue(engine, QEvent::ScrollPrepare),                    ENUMPROPFLAGS);
  proto.setProperty("Scroll",                           QScriptValue(engine, QEvent::Scroll),                           ENUMPROPFLAGS);
  proto.setProperty("Shortcut",                         QScriptValue(engine, QEvent::Shortcut),                         ENUMPROPFLAGS);
  proto.setProperty("ShortcutOverride",                 QScriptValue(engine, QEvent::ShortcutOverride),                 ENUMPROPFLAGS);
  proto.setProperty("Show",                             QScriptValue(engine, QEvent::Show),                             ENUMPROPFLAGS);
  proto.setProperty("ShowToParent",                     QScriptValue(engine, QEvent::ShowToParent),                     ENUMPROPFLAGS);
  proto.setProperty("SockAct",                          QScriptValue(engine, QEvent::SockAct),                          ENUMPROPFLAGS);
  proto.setProperty("StateMachineSignal",               QScriptValue(engine, QEvent::StateMachineSignal),               ENUMPROPFLAGS);
  proto.setProperty("StateMachineWrapped",              QScriptValue(engine, QEvent::StateMachineWrapped),              ENUMPROPFLAGS);
  proto.setProperty("StatusTip",                        QScriptValue(engine, QEvent::StatusTip),                        ENUMPROPFLAGS);
  proto.setProperty("StyleChange",                      QScriptValue(engine, QEvent::StyleChange),                      ENUMPROPFLAGS);
  proto.setProperty("TabletMove",                       QScriptValue(engine, QEvent::TabletMove),                       ENUMPROPFLAGS);
  proto.setProperty("TabletPress",                      QScriptValue(engine, QEvent::TabletPress),                      ENUMPROPFLAGS);
  proto.setProperty("TabletRelease",                    QScriptValue(engine, QEvent::TabletRelease),                    ENUMPROPFLAGS);
  proto.setProperty("TabletEnterProximity",             QScriptValue(engine, QEvent::TabletEnterProximity),             ENUMPROPFLAGS);
  proto.setProperty("TabletLeaveProximity",             QScriptValue(engine, QEvent::TabletLeaveProximity),             ENUMPROPFLAGS);
#if QT_VERSION >= 0x050900
  proto.setProperty("TabletTrackingChange",             QScriptValue(engine, QEvent::TabletTrackingChange),             ENUMPROPFLAGS);
#endif
  proto.setProperty("ThreadChange",                     QScriptValue(engine, QEvent::ThreadChange),                     ENUMPROPFLAGS);
  proto.setProperty("Timer",                            QScriptValue(engine, QEvent::Timer),                            ENUMPROPFLAGS);
  proto.setProperty("ToolBarChange",                    QScriptValue(engine, QEvent::ToolBarChange),                    ENUMPROPFLAGS);
  proto.setProperty("ToolTip",                          QScriptValue(engine, QEvent::ToolTip),                          ENUMPROPFLAGS);
  proto.setProperty("ToolTipChange",                    QScriptValue(engine, QEvent::ToolTipChange),                    ENUMPROPFLAGS);
  proto.setProperty("TouchBegin",                       QScriptValue(engine, QEvent::TouchBegin),                       ENUMPROPFLAGS);
  proto.setProperty("TouchCancel",                      QScriptValue(engine, QEvent::TouchCancel),                      ENUMPROPFLAGS);
  proto.setProperty("TouchEnd",                         QScriptValue(engine, QEvent::TouchEnd),                         ENUMPROPFLAGS);
  proto.setProperty("TouchUpdate",                      QScriptValue(engine, QEvent::TouchUpdate),                      ENUMPROPFLAGS);
  proto.setProperty("UngrabKeyboard",                   QScriptValue(engine, QEvent::UngrabKeyboard),                   ENUMPROPFLAGS);
  proto.setProperty("UngrabMouse",                      QScriptValue(engine, QEvent::UngrabMouse),                      ENUMPROPFLAGS);
  proto.setProperty("UpdateLater",                      QScriptValue(engine, QEvent::UpdateLater),                      ENUMPROPFLAGS);
  proto.setProperty("UpdateRequest",                    QScriptValue(engine, QEvent::UpdateRequest),                    ENUMPROPFLAGS);
  proto.setProperty("WhatsThis",                        QScriptValue(engine, QEvent::WhatsThis),                        ENUMPROPFLAGS);
  proto.setProperty("WhatsThisClicked",                 QScriptValue(engine, QEvent::WhatsThisClicked),                 ENUMPROPFLAGS);
  proto.setProperty("Wheel",                            QScriptValue(engine, QEvent::Wheel),                            ENUMPROPFLAGS);
  proto.setProperty("WinEventAct",                      QScriptValue(engine, QEvent::WinEventAct),                      ENUMPROPFLAGS);
  proto.setProperty("WindowActivate",                   QScriptValue(engine, QEvent::WindowActivate),                   ENUMPROPFLAGS);
  proto.setProperty("WindowBlocked",                    QScriptValue(engine, QEvent::WindowBlocked),                    ENUMPROPFLAGS);
  proto.setProperty("WindowDeactivate",                 QScriptValue(engine, QEvent::WindowDeactivate),                 ENUMPROPFLAGS);
  proto.setProperty("WindowIconChange",                 QScriptValue(engine, QEvent::WindowIconChange),                 ENUMPROPFLAGS);
  proto.setProperty("WindowStateChange",                QScriptValue(engine, QEvent::WindowStateChange),                ENUMPROPFLAGS);
  proto.setProperty("WindowTitleChange",                QScriptValue(engine, QEvent::WindowTitleChange),                ENUMPROPFLAGS);
  proto.setProperty("WindowUnblocked",                  QScriptValue(engine, QEvent::WindowUnblocked),                  ENUMPROPFLAGS);
  proto.setProperty("WinIdChange",                      QScriptValue(engine, QEvent::WinIdChange),                      ENUMPROPFLAGS);
  proto.setProperty("ZOrderChange",                     QScriptValue(engine, QEvent::ZOrderChange),                     ENUMPROPFLAGS);

  proto.setProperty("User", QScriptValue(engine, QEvent::User), ENUMPROPFLAGS);
  proto.setProperty("MaxUser", QScriptValue(engine, QEvent::MaxUser), ENUMPROPFLAGS);

}

QEventProto::QEventProto(QObject *parent)
    : QObject(parent)
{
}

void QEventProto::accept()
{
  QEvent *item = qscriptvalue_cast<QEvent*>(thisObject());
  if (item)
    item->accept();
}

void QEventProto::ignore()
{
  QEvent *item = qscriptvalue_cast<QEvent*>(thisObject());
  if (item)
    item->ignore();
}

bool QEventProto::isAccepted() const
{
  QEvent *item = qscriptvalue_cast<QEvent*>(thisObject());
  if (item)
    return item->isAccepted();
  return false;
}

void QEventProto::setAccepted(bool accepted)
{
  QEvent *item = qscriptvalue_cast<QEvent*>(thisObject());
  if (item)
    item->setAccepted(accepted);
}

bool QEventProto::spontaneous() const
{
  QEvent *item = qscriptvalue_cast<QEvent*>(thisObject());
  if (item)
    return item->spontaneous();
  return false;
}

