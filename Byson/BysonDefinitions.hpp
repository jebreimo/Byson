//****************************************************************************
// Copyright © 2016, Jan Erik Breimo.
// Created 2014-04-24 by JEB
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#ifdef _WIN32
  #ifdef BYSON_EXPORTS
    #define BYSON_API __declspec(dllexport)
  #else
    #define BYSON_API __declspec(dllimport)
    #ifdef _DEBUG
      #pragma comment (lib, "YubJsonD.lib")
    #else
      #pragma comment (lib, "Byson.lib")
    #endif
  #endif
#else
  #define BYSON_API
#endif
