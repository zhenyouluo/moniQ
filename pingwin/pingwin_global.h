#ifndef PINGWIN_GLOBAL_H
#define PINGWIN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PINGWIN_LIBRARY)
#  define PINGWINSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PINGWINSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PINGWIN_GLOBAL_H
