############################## Source Files of LiteHub Based on TNN #################################
# 1. glob sources files
file(GLOB TNN_CORE_SRCS ${CMAKE_SOURCE_DIR}/source/lite/tnn/core/*.cpp)
file(GLOB TNN_CV_SRCS ${CMAKE_SOURCE_DIR}/source/lite/tnn/cv/*.cpp)
file(GLOB TNN_NLP_SRCS ${CMAKE_SOURCE_DIR}/source/lite/tnn/nlp/*.cpp)
file(GLOB TNN_ASR_SRCS ${CMAKE_SOURCE_DIR}/source/lite/tnn/asr/*.cpp)
# 2. glob headers files
file(GLOB TNN_CORE_HEAD ${CMAKE_SOURCE_DIR}/source/lite/tnn/core/*.h)
file(GLOB TNN_CV_HEAD ${CMAKE_SOURCE_DIR}/source/lite/tnn/cv/*.h)
file(GLOB TNN_NLP_HEAD ${CMAKE_SOURCE_DIR}/source/lite/tnn/nlp/*.h)
file(GLOB TNN_ASR_HEAD ${CMAKE_SOURCE_DIR}/source/lite/tnn/asr/*.h)

set(TNN_SRCS
        ${TNN_CV_SRCS}
        ${TNN_NLP_SRCS}
        ${TNN_ASR_SRCS}
        ${TNN_CORE_SRCS})
