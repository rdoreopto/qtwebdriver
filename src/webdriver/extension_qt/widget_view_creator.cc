#include "extension_qt/widget_view_creator.h"

#include "webdriver_session.h"
#include "webdriver_logging.h"

#include "widget_view_util.h"
#include "extension_qt/widget_view_handle.h"

#include <QtCore/QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

namespace webdriver {

QWidgetViewCreator::QWidgetViewCreator() {}

bool QWidgetViewCreator::CreateViewByClassName(const Logger& logger, const std::string& className, ViewHandle** view) const {
	ViewHandle* handle = NULL;	

    if (factory.empty())
        return false;

	if (className.empty() || className == "WidgetView") {
		// get first registered
        CreateViewMethod createMethod = factory.begin()->second;
        handle = new QViewHandle(static_cast<QWidget*>(createMethod()));
	} else {
    	FactoryMap::const_iterator it = factory.find(className);
        if (it != factory.end())
        {
        	CreateViewMethod createMethod = it->second;
            handle = new QViewHandle(static_cast<QWidget*>(createMethod()));
        }
    }

    if (NULL != handle) {
        QWidget* widget = (dynamic_cast<QViewHandle*>(handle))->get();
        std::string objClassName(widget->metaObject()->className());

        if (NULL != widget) {
            widget->show();

            logger.Log(kInfoLogLevel, "QWidgetViewCreator created view("+objClassName+") by class name - "+className);

            *view = handle;
            
            return true;
        } else {
            logger.Log(kSevereLogLevel, "QWidgetViewCreator, smth wrong.");
            handle->Release();
        }
    }

    // view was not created
    return false;
}

bool QWidgetViewCreator::CreateViewForUrl(const Logger& logger, const std::string& url, ViewHandle** view) const {
	if (!QWidgetViewUtil::isUrlSupported(url))
        return false;

    std::string className = QWidgetViewUtil::extractClassName(url);

    return CreateViewByClassName(logger, className, view);
};



} // namespace webdriver