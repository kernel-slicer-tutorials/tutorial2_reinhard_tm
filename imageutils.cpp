#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

#if defined(_WIN32)
    #ifndef NOMINMAX
    #define NOMINMAX
    #endif
#endif

#define TINYEXR_IMPLEMENTATION
#include "tinyexr.h"

std::vector<float> LoadImage4fFromEXR(const char* infilename, int* pW, int* pH) 
{
  std::vector<float> result;
  float* out; // width * height * RGBA
  int width  = 0;
  int height = 0;
  const char* err = nullptr; 

  int ret = LoadEXR(&out, &width, &height, infilename, &err);
  if (ret != TINYEXR_SUCCESS) 
  {
    if (err) 
    {
      fprintf(stderr, "[LoadImage4fFromEXR] : %s\n", err);
      std::cerr << "[LoadImage4fFromEXR] : " << err << std::endl;
      delete err;
    }
  }
  else 
  {
    result.resize(width * height*4);
    *pW = uint32_t(width);
    *pH = uint32_t(height);
    memcpy(result.data(), out, width*height*sizeof(float)*4);
    free(out);
  }
  
  return result;
}
