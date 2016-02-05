/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2016 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef __QURLPROTO_H__
#define __QURLPROTO_H__

#include <QList>
#include <QObject>
#include <QScriptEngine>
#include <QScriptable>
#include <QString>
#include <QtScript>
#include <QUrl>

#if QT_VERSION < 0x050000
#include <QUrlQuery>
#endif

class QByteArray;
class QSslConfiguration;

Q_DECLARE_METATYPE(QUrl*)
//Q_DECLARE_METATYPE(QUrl) // Already set in qurl.h
#if QT_VERSION < 0x050000
Q_DECLARE_METATYPE(enum QUrl::FormattingOption)
Q_DECLARE_METATYPE(enum QUrl::ParsingMode)
#else
Q_DECLARE_METATYPE(enum QUrl::ComponentFormattingOption)
Q_DECLARE_METATYPE(QUrl::ComponentFormattingOptions)
Q_DECLARE_METATYPE(QUrl::FormattingOptions)
Q_DECLARE_METATYPE(enum QUrl::ParsingMode)
Q_DECLARE_METATYPE(enum QUrl::UrlFormattingOption)
Q_DECLARE_METATYPE(enum QUrl::UserInputResolutionOption)
Q_DECLARE_METATYPE(QUrl::UserInputResolutionOptions)
#endif

void setupQUrlProto(QScriptEngine *engine);
QScriptValue constructQUrl(QScriptContext *context, QScriptEngine *engine);

class QUrlProto : public QObject, public QScriptable
{
  Q_OBJECT

  public:
    QUrlProto(QObject *parent);
    virtual ~QUrlProto();

/*
 * There were major changes to QUrl in Qt 5.x. QUrlQuery was added and sever function moved there.
 * Several function now have parameter defaults. The versioner are split below into one large chunk
 * to keep each versions code together.
 */
// Qt 4.8 version of header:
#if QT_VERSION < 0x050000

    Q_INVOKABLE void                                    addEncodedQueryItem(const QByteArray &key, const QByteArray &value);
    Q_INVOKABLE void                                    addQueryItem(const QString &key, const QString & value);
    Q_INVOKABLE QList<QByteArray>                       allEncodedQueryItemValues(const QByteArray &key) const;
    Q_INVOKABLE QStringList                             allQueryItemValues(const QString &key) const;
    Q_INVOKABLE QString                                 authority() const;
    Q_INVOKABLE void                                    clear();
    Q_INVOKABLE QByteArray                              encodedFragment() const;
    Q_INVOKABLE QByteArray                              encodedHost()     const;
    Q_INVOKABLE QByteArray                              encodedPassword() const;
    Q_INVOKABLE QByteArray                              encodedPath()     const;
    Q_INVOKABLE QByteArray                              encodedQuery()    const;
    Q_INVOKABLE QByteArray                              encodedQueryItemValue(const QByteArray &key) const;
    Q_INVOKABLE QList<QPair<QByteArray, QByteArray> >   encodedQueryItems() const;
    Q_INVOKABLE QByteArray                              encodedUserName() const;
    Q_INVOKABLE QString                                 errorString()     const;
    Q_INVOKABLE QString                                 fragment()       const;
    Q_INVOKABLE bool                                    hasEncodedQueryItem(const QByteArray &key) const;
    Q_INVOKABLE bool                                    hasFragment()    const;
    Q_INVOKABLE bool                                    hasQuery()       const;
    Q_INVOKABLE bool                                    hasQueryItem(const QString &key) const;
    Q_INVOKABLE QString                                 host()           const;
    Q_INVOKABLE bool                                    isEmpty()        const;
    Q_INVOKABLE bool                                    isParentOf(const QUrl &childUrl) const;
    Q_INVOKABLE bool                                    isRelative()     const;
    Q_INVOKABLE bool                                    isValid()        const;
    Q_INVOKABLE QString                                 password()       const;
    Q_INVOKABLE QString                                 path()           const;
    Q_INVOKABLE int                                     port()           const;
    Q_INVOKABLE int                                     port(int defaultPort) const;
    Q_INVOKABLE QString                                 queryItemValue(const QString &key) const;
    Q_INVOKABLE QList<QPair<QString, QString> >         queryItems()  const;
    Q_INVOKABLE char                                    queryPairDelimiter()               const;
    Q_INVOKABLE char                                    queryValueDelimiter()              const;
    Q_INVOKABLE void                                    removeAllEncodedQueryItems(const QByteArray &key);
    Q_INVOKABLE void                                    removeAllQueryItems(const QString &key);
    Q_INVOKABLE void                                    removeEncodedQueryItem(const QByteArray &key);
    Q_INVOKABLE void                                    removeQueryItem(const QString &key);
    Q_INVOKABLE QUrl                                    resolved(const QUrl &relative)     const;
    Q_INVOKABLE QString                                 scheme() const;
    Q_INVOKABLE void                                    setAuthority(const QString &authority);
    Q_INVOKABLE void                                    setEncodedFragment(const QByteArray &fragment);
    Q_INVOKABLE void                                    setEncodedHost(const QByteArray &host);
    Q_INVOKABLE void                                    setEncodedPassword(const QByteArray &password);
    Q_INVOKABLE void                                    setEncodedPath(const QByteArray &path);
    Q_INVOKABLE void                                    setEncodedQuery(const QByteArray &query);
    Q_INVOKABLE void                                    setEncodedQueryItems(const QList<QPair<QByteArray, QByteArray> > &query);
    Q_INVOKABLE void                                    setEncodedUrl(const QByteArray &encodedUrl);
    Q_INVOKABLE void                                    setEncodedUrl(const QByteArray &encodedUrl,
                                                                            QUrl::ParsingMode parsingMode);
    Q_INVOKABLE void                                    setEncodedUserName(const QByteArray &userName);
    Q_INVOKABLE void                                    setFragment(const QString &fragment);
    Q_INVOKABLE void                                    setHost(const QString &host);
    Q_INVOKABLE void                                    setPassword(const QString &password);
    Q_INVOKABLE void                                    setPath(const QString &path);
    Q_INVOKABLE void                                    setPort(int port);
    Q_INVOKABLE void                                    setQueryDelimiters(char valueDelimiter, char pairDelimiter);
    Q_INVOKABLE void                                    setQueryItems(const QList<QPair<QString, QString> > &query);
    Q_INVOKABLE void                                    setQueryItems(const QVariantMap &map);
    Q_INVOKABLE void                                    setScheme(const QString &scheme);
    Q_INVOKABLE void                                    setUrl(const QString &url);
    Q_INVOKABLE void                                    setUrl(const QString &url, QUrl::ParsingMode parsingMode);
    Q_INVOKABLE void                                    setUserInfo(const QString &userInfo);
    Q_INVOKABLE void                                    setUserName(const QString &userName);
    Q_INVOKABLE QByteArray                              toEncoded(QUrl::FormattingOptions options = QUrl::None) const;
    Q_INVOKABLE QString                                 toLocalFile() const;
    Q_INVOKABLE QString                                 userInfo()    const;
    Q_INVOKABLE QString                                 userName()    const;

  public slots:
    Q_INVOKABLE QString                                 toString(QUrl::FormattingOptions options = QUrl::None)  const;

#else
// Qt 5.5.1 version of header:
    Q_INVOKABLE QUrl        adjusted(QUrl::FormattingOptions options) const;
    Q_INVOKABLE QString     authority(QUrl::ComponentFormattingOptions options = QUrl::PrettyDecoded) const;
    Q_INVOKABLE void        clear();
    Q_INVOKABLE QString     errorString() const;
    Q_INVOKABLE QString     fileName(QUrl::ComponentFormattingOptions options = QUrl::FullyDecoded) const;
    Q_INVOKABLE QString     fragment(QUrl::ComponentFormattingOptions options = QUrl::PrettyDecoded) const;
    Q_INVOKABLE bool        hasFragment() const;
    Q_INVOKABLE bool        hasQuery() const;
    Q_INVOKABLE QString     host(QUrl::ComponentFormattingOptions options = QUrl::FullyDecoded) const;
    Q_INVOKABLE bool        isEmpty() const;
    Q_INVOKABLE bool        isLocalFile() const;
    Q_INVOKABLE bool        isParentOf(const QUrl & childUrl) const;
    Q_INVOKABLE bool        isRelative() const;
    Q_INVOKABLE bool        isValid() const;
    Q_INVOKABLE bool        matches(const QUrl & url, QUrl::FormattingOptions options) const;
    Q_INVOKABLE QString     password(QUrl::ComponentFormattingOptions options = QUrl::FullyDecoded) const;
    Q_INVOKABLE QString     path(QUrl::ComponentFormattingOptions options = QUrl::FullyDecoded) const;
    Q_INVOKABLE int         port(int defaultPort = -1) const;
    Q_INVOKABLE QString     query(QUrl::ComponentFormattingOptions options = QUrl::PrettyDecoded) const;
    Q_INVOKABLE QUrl        resolved(const QUrl & relative) const;
    Q_INVOKABLE QString     scheme() const;
    Q_INVOKABLE void        setAuthority(const QString & authority, QUrl::ParsingMode mode = QUrl::TolerantMode);
    Q_INVOKABLE void        setFragment(const QString & fragment, QUrl::ParsingMode mode = QUrl::TolerantMode);
    Q_INVOKABLE void        setHost(const QString & host, QUrl::ParsingMode mode = QUrl::DecodedMode);
    Q_INVOKABLE void        setPassword(const QString & password, QUrl::ParsingMode mode = QUrl::DecodedMode);
    Q_INVOKABLE void        setPath(const QString & path, QUrl::ParsingMode mode = QUrl::DecodedMode);
    Q_INVOKABLE void        setPort(int port);
    Q_INVOKABLE void        setQuery(const QString & query, QUrl::ParsingMode mode = QUrl::TolerantMode);
    Q_INVOKABLE void        setQuery(const QUrlQuery & query);
    Q_INVOKABLE void        setScheme(const QString & scheme);
    Q_INVOKABLE void        setUrl(const QString & url, QUrl::ParsingMode parsingMode = QUrl::TolerantMode);
    Q_INVOKABLE void        setUserInfo(const QString & userInfo, QUrl::ParsingMode mode = QUrl::TolerantMode);
    Q_INVOKABLE void        setUserName(const QString & userName, QUrl::ParsingMode mode = QUrl::DecodedMode);
    Q_INVOKABLE void        swap(QUrl & other);
    Q_INVOKABLE QString     toDisplayString(QUrl::FormattingOptions options = QUrl::FormattingOptions( QUrl::PrettyDecoded )) const;
    Q_INVOKABLE QByteArray  toEncoded(QUrl::FormattingOptions options = QUrl::FullyEncoded) const;
    Q_INVOKABLE QString     toLocalFile() const;
    Q_INVOKABLE QString     toString(QUrl::FormattingOptions options = QUrl::FormattingOptions( QUrl::PrettyDecoded )) const;
    Q_INVOKABLE QString     topLevelDomain(QUrl::ComponentFormattingOptions options = QUrl::FullyDecoded) const;
    Q_INVOKABLE QString     url(QUrl::FormattingOptions options = QUrl::FormattingOptions( QUrl::PrettyDecoded )) const;
    Q_INVOKABLE QString     userInfo(QUrl::ComponentFormattingOptions options = QUrl::PrettyDecoded) const;
    Q_INVOKABLE QString     userName(QUrl::ComponentFormattingOptions options = QUrl::FullyDecoded) const;

#endif

};

#endif
