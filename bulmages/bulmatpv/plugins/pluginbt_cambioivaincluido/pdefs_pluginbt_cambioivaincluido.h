#ifndef PDEFS_PLUGINBT_CAMBIOIVAINCLUIDO_H
#define PDEFS_PLUGINBT_CAMBIOIVAINCLUIDO_H


#ifdef WIN32

#ifdef PLUGINBT_CAMBIOIVAINCLUIDO
#define PLUGINBT_CAMBIOIVAINCLUIDO_EXPORT __declspec(dllexport)
#else
#define PLUGINBT_CAMBIOIVAINCLUIDO_EXPORT __declspec(dllimport)
#endif

#else

#define PLUGINBT_CAMBIOIVAINCLUIDO_EXPORT

#endif


#endif
