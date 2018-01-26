#ifndef GLCHECK_H
#define GLCHECK_H

#include <iostream>

#define BUFFER_OFFSET(offset) ((void *)(offset))

namespace k {

const char* GetOpenGLErrorString(int errId);

void CheckOpenGLError(const char* stmt, const char* fname, int line);

}

#ifndef GL__
#ifdef TRUE // _DEBUG
#define GL__(stmt) do { \
stmt; \
k::CheckOpenGLError(#stmt, __FILE__, __LINE__); \
} while (0);
#else
#define GL__(stmt) stmt
#endif
#endif

#endif // GLCHECK_H
