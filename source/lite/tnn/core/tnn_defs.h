//
// Created by DefTruth on 2021/10/17.
//

#ifndef LITE_AI_TOOLKIT_TNN_CORE_TNN_DEFS_H
#define LITE_AI_TOOLKIT_TNN_CORE_TNN_DEFS_H

#include "lite/config.h"
#include "lite/lite.ai.defs.h"

#ifdef ENABLE_DEBUG_STRING
# define LITETNN_DEBUG 1
#else
# define LITETNN_DEBUG 0
#endif

#ifdef LITE_WIN32
#  ifndef NOMINMAX
#    define NOMINMAX
#  endif
#endif

#endif //LITE_AI_TOOLKIT_TNN_CORE_TNN_DEFS_H
