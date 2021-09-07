#ifndef DBUTIL_GLOBAL_H
#define DBUTIL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DBUTIL_LIBRARY)
#  define DBUTILSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DBUTILSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // DBUTIL_GLOBAL_H
