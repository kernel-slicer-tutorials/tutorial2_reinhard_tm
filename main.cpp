#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <cstdint>
#include <cassert>
#include <string>

#include "reinhard.h"
#include "Image2d.h"

std::vector<float> LoadImage4fFromEXR(const char* infilename, int* pW, int* pH);
bool LiteImage::SaveBMP(const char* fname, const unsigned int* pixels, int w, int h);

#ifdef USE_VULKAN
#include "vk_context.h"
std::shared_ptr<ReinhardTM> CreateReinhardTM_Generated(vk_utils::VulkanContext a_ctx, size_t a_maxThreadsGenerated); 
#endif
#ifdef USE_ISPC
std::shared_ptr<ReinhardTM> CreateReinhardTM_ISPC();
#endif

int main(int argc, const char** argv)
{
  int w = 0, h = 0;
  std::vector<float> hdrData = LoadImage4fFromEXR("images/tief_etz_1k.exr", &w, &h); // you can download test images at https://polyhaven.com/hdris
  
  if(w == 0 || h == 0)
  {
    std::cout << "can't open input file 'images/tief_etz_1k.exr' " << std::endl;
    return 0;
  }

  std::vector<uint> ldrData(w*h);
  std::string option = (argc <= 1) ? "" : (argv[1]);
  
  bool onGPU  = (option == "--gpu");
  bool isISPC = (option == "--ispc");

  std::shared_ptr<ReinhardTM> pImpl = nullptr;
  #ifdef USE_VULKAN
  if(onGPU)
  {
    auto ctx = vk_utils::globalContextGet(true, 0);
    pImpl = CreateReinhardTM_Generated(ctx, w*h);
  }
  else
  #endif
  #ifdef USE_ISPC
  else if(isISPC)
    pImpl = CreateReinhardTM_ISPC();
  else
  #endif
    pImpl = std::make_shared<ReinhardTM>();

  pImpl->CommitDeviceData();
  pImpl->Run(w, h, hdrData.data(), ldrData.data()); // (const float4*)
  
  // just flip data by Y axis
  //
  std::vector<unsigned> flipedYData(w*h);
  for (unsigned y = 0; y < h; y++)
  {
    const unsigned offset1 = (h - y - 1)*w; 
    const unsigned offset2 = y*w;
    for(unsigned x=0; x<w; x++)
      flipedYData[offset2+x] = ldrData[offset1 + x];
  }

  if(onGPU)
    LiteImage::SaveBMP("zout_gpu.bmp", flipedYData.data(), w, h);
  else if(isISPC)
    LiteImage::SaveBMP("zout_ispc.bmp", flipedYData.data(), w, h);
  else
    LiteImage::SaveBMP("zout_cpu.bmp", flipedYData.data(), w, h);

  std::cout << "whitePoint = " << pImpl->getWhitePoint() << std::endl;

  float timings[4] = {0,0,0,0};
  pImpl->GetExecutionTime("Run", timings);
  std::cout << "Run(exec) = " << timings[0]              << " ms " << std::endl;
  std::cout << "Run(copy) = " << timings[1] + timings[2] << " ms " << std::endl;
  std::cout << "Run(ovrh) = " << timings[3]              << " ms " << std::endl;

  return 0;
}
