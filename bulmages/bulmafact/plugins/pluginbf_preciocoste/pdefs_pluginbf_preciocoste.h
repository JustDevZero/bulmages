#ifndef PDEFS_PLUGINBF_PRECIOCOSTE_H
#define PDEFS_PLUGINBF_PRECIOCOSTE_H


#ifdef Q_OS_WIN32

#ifdef PLUGINBF_PRECIOCOSTE
#define PLUGINBF_PRECIOCOSTE_EXPORT __declspec(dllexport)
#else
#define PLUGINBF_PRECIOCOSTE_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBF_PRECIOCOSTE_EXPORT

#endif


#endif

