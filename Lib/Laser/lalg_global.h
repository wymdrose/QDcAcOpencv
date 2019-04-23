#ifndef LALG_GLOBAL_H
#define LALG_GLOBAL_H

#include <QtCore/qglobal.h>

#pragma comment(lib, "Lib/Laser/LALG.lib")

#ifdef LALG_LIB
# define LALG_EXPORT Q_DECL_EXPORT
#else
# define LALG_EXPORT Q_DECL_IMPORT
#endif

#endif // LALG_GLOBAL_H
