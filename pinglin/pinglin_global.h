#ifndef PINGLIN_GLOBAL_H
#define PINGLIN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PINGLIN_LIBRARY)
#  define PINGLINSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PINGLINSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PINGLIN_GLOBAL_H
