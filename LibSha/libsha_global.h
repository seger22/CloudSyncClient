#ifndef LIBSHA_GLOBAL_H
#define LIBSHA_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIBSHA_LIBRARY)
#  define LIBSHASHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIBSHASHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LIBSHA_GLOBAL_H
