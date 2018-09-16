/* Copyright (C) 2017-2018 Michal Kosciesza <michal@mkiol.net>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifdef SAILFISH
#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif
#include <QGuiApplication>
#include <QtQml>
#include <QQmlEngine>
#include <QQuickView>
#include <QQmlContext>
#include <sailfishapp.h>
#include "iconprovider.h"
#endif

#ifdef DESKTOP
#include <QApplication>
#include "mainwindow.h"
#endif

#include <QLocale>
#include <QTranslator>
#include <QTextCodec>
#include <iostream>
#include <libupnpp/control/discovery.hxx>
#include <libupnpp/control/description.hxx>
#include "utils.h"
#include "directory.h"
#include "devicemodel.h"
#include "renderingcontrol.h"
#include "avtransport.h"
#include "contentserver.h"
#include "filemetadata.h"
#include "settings.h"
#include "deviceinfo.h"
#include "playlistmodel.h"
#include "dbusapp.h"
#include "albummodel.h"
#include "artistmodel.h"
#include "playlistfilemodel.h"
#include "trackmodel.h"
#include "services.h"
#include "info.h"

int main(int argc, char *argv[])
{
#ifdef SAILFISH
    auto app = SailfishApp::application(argc, argv);
    auto view = SailfishApp::createView();
    auto context = view->rootContext();
    auto engine = view->engine();

    qmlRegisterType<DeviceModel>("harbour.jupii.DeviceModel", 1, 0, "DeviceModel");
    qmlRegisterType<RenderingControl>("harbour.jupii.RenderingControl", 1, 0, "RenderingControl");
    qmlRegisterType<AVTransport>("harbour.jupii.AVTransport", 1, 0, "AVTransport");
    qmlRegisterType<DeviceInfo>("harbour.jupii.DeviceInfo", 1, 0, "DeviceInfo");
    qmlRegisterType<PlayListModel>("harbour.jupii.PlayListModel", 1, 0, "PlayListModel");
    qmlRegisterType<AlbumModel>("harbour.jupii.AlbumModel", 1, 0, "AlbumModel");
    qmlRegisterType<ArtistModel>("harbour.jupii.ArtistModel", 1, 0, "ArtistModel");
    qmlRegisterType<PlaylistFileModel>("harbour.jupii.PlaylistFileModel", 1, 0, "PlaylistFileModel");
    qmlRegisterType<TrackModel>("harbour.jupii.TrackModel", 1, 0, "TrackModel");

    engine->addImageProvider(QLatin1String("icons"), new IconProvider);

    context->setContextProperty("APP_NAME", Jupii::APP_NAME);
    context->setContextProperty("APP_VERSION", Jupii::APP_VERSION);
    context->setContextProperty("COPYRIGHT_YEAR", Jupii::COPYRIGHT_YEAR);
    context->setContextProperty("AUTHOR", Jupii::AUTHOR);
    context->setContextProperty("SUPPORT_EMAIL", Jupii::SUPPORT_EMAIL);
    context->setContextProperty("PAGE", Jupii::PAGE);
    context->setContextProperty("LICENSE", Jupii::LICENSE);
    context->setContextProperty("LICENSE_URL", Jupii::LICENSE_URL);
#endif

#ifdef DESKTOP
    auto app = new QApplication(argc, argv);
    app->setOrganizationName(app->applicationName());
#endif

    app->setApplicationDisplayName(Jupii::APP_NAME);
    app->setApplicationVersion(Jupii::APP_VERSION);

    qRegisterMetaType<Service::ErrorType>("ErrorType");

    /*QTranslator translator;
    const QString locale = QLocale::system().name();
    //const QString locale = "es";
    const QString transDir = SailfishApp::pathTo("translations").toLocalFile();
    if(translator.load("harbour-jupii-" + locale, transDir)) {
        app->installTranslator(&translator);
    } else {
        qWarning() << "Couldn't load translation for" << locale;
        if (translator.load("harbour-jupii", transDir)) {
            app->installTranslator(&translator);
        } else {
            qWarning() << "Couldn't load default translation";
        }
    }*/

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    auto utils = Utils::instance();
    auto settings = Settings::instance();
    auto dir = Directory::instance();
    auto cserver = ContentServer::instance();
    auto services = Services::instance();
    DbusProxy dbusProxy;

#ifdef SAILFISH
    context->setContextProperty("utils", utils);
    context->setContextProperty("settings", settings);
    context->setContextProperty("directory", dir);
    context->setContextProperty("cserver", cserver);
    context->setContextProperty("dbus", &dbusProxy);
    context->setContextProperty("rc", services->renderingControl.get());
    context->setContextProperty("av", services->avTransport.get());

    view->setSource(SailfishApp::pathTo("qml/main.qml"));
    view->show();
#endif

#ifdef DESKTOP
    MainWindow devicesWindow;
    devicesWindow.show();
#endif
    return app->exec();
}