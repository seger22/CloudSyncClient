#ifndef INOTIFY_GLOBAL_H
#define INOTIFY_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(INOTIFY_LIBRARY)
#  define INOTIFYSHARED_EXPORT Q_DECL_EXPORT
#else
#  define INOTIFYSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // INOTIFY_GLOBAL_H
