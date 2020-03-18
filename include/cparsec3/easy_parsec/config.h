/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../base/common.h"

// -----------------------------------------------------------------------
#if !defined(CPARSEC_CONFIG_DATA_SOURCE)
#define CPARSEC_CONFIG_DATA_SOURCE String
#endif

#if !defined(CPARSEC_CONFIG_POSITIONER)
#define CPARSEC_CONFIG_POSITIONER                                        \
  CPARSEC_POSITIONER_DEFAULT(CPARSEC_CONFIG_DATA_SOURCE)
#endif

// -----------------------------------------------------------------------
#define CPARSEC_POSITIONER_DEFAULT(I)                                    \
  EXTRACT_OR_DEFAULT(CAT(CPARSEC_POSITIONER_DEFAULT__, I)(), )

#define CPARSEC_POSITIONER_DEFAULT__String() JUST(Text)

// -----------------------------------------------------------------------
#define CPARSEC_STREAM_TYPE                                              \
  CPARSEC_STREAM_TYPE_DEFAULT(CPARSEC_CONFIG_DATA_SOURCE,                \
                              CPARSEC_CONFIG_POSITIONER)

#define CPARSEC_STREAM_CTOR                                              \
  CPARSEC_STREAM_CTOR_DEFAULT(CPARSEC_CONFIG_DATA_SOURCE,                \
                              CPARSEC_CONFIG_POSITIONER)

#define CPARSEC_STREAM_NEW(...) CPARSEC_STREAM_CTOR(__VA_ARGS__)

// -----------------------------------------------------------------------
#define CPARSEC_STREAM_TYPE_DEFAULT(I, P)                                \
  EXTRACT_OR_DEFAULT(                                                    \
      CAT(CAT(CAT(CPARSEC_STREAM_TYPE_DEFAULT__, I), _), P)(), I)

#define CPARSEC_STREAM_CTOR_DEFAULT(I, P)                                \
  EXTRACT_OR_DEFAULT(                                                    \
      CAT(CAT(CAT(CPARSEC_STREAM_CTOR_DEFAULT__, I), _), P)(),           \
      CPARSEC_IDENTITY)

#define CPARSEC_IDENTITY(...) __VA_ARGS__
#define CPARSEC_STREAM_TYPE_DEFAULT__String_() JUST(String)
#define CPARSEC_STREAM_CTOR_DEFAULT__String_() JUST(CPARSEC_IDENTITY)
#define CPARSEC_STREAM_TYPE_DEFAULT__String_Text() JUST(State(String))
#define CPARSEC_STREAM_CTOR_DEFAULT__String_Text()                       \
  JUST(trait(State(String)).create)
