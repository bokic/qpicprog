#ifndef EXPORT_H
#define EXPORT_H

#if (defined WIN32)&&(defined LIB_EXPORT)
#define MY_EXPORT __declspec(dllexport)
#else
#define MY_EXPORT
#endif

#endif // EXPORT_H
