#include "settingsplugin.h"

// #include "resourcescontroller.h"
// #include "resourcelistmodel.h"
// #include "maildir_resource.h"
#include "settings.h"

#include <QtQml>

void SettingsPlugin::registerTypes (const char *uri)
{
    Q_ASSERT(uri == QLatin1String("org.kube.framework.settings"));

    // qmlRegisterType<ResourceListModel>();
    // qmlRegisterType<ResourcesController>(uri, 1, 0, "Resources");
    // qmlRegisterType<MaildirResouceController>(uri, 1, 0, "Maildir");
    qmlRegisterType<Kube::Settings>(uri, 1, 0, "Settings");
}
