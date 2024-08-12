#ifndef NCCC__NCCCEXPORT_H
#define NCCC__NCCCEXPORT_H

#ifdef NCCC_DLL
#ifdef _WIN32
#define NCCC_EXPORT __declspec(dllexport)
#else
#define NCCC_EXPORT __attribute__ ((visibility ("default")))
#endif
#else /* !NCCC_DLL = Generic static-library */
#ifdef _WIN32
#define NCCC_EXPORT
#else
#define NCCC_EXPORT __attribute__ ((visibility ("default")))
#endif
#endif

#endif
