#ifndef PDEFS_PLUGINBF_PRINTERCOCINA_H
#define PDEFS_PLUGINBF_PRINTERCOCINA_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBF_PRINTERCOCINA
#define PLUGINBF_PRINTERCOCINA_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_PRINTERCOCINA_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_PRINTERCOCINA_EXPORT

#endif


#endif

