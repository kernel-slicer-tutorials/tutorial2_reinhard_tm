cmake_minimum_required (VERSION 3.8)
project (test)

# get rid of annoying MSVC warnings.
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
set (CMAKE_CXX_STANDARD 17)
option(USE_ISPC   "Enable ISPC code" OFF)
option(USE_VULKAN "Enable Vulkan code" OFF)

include_directories(".")
if(WIN32)
  link_directories(${ADDITIONAL_LIBRARY_DIRS})
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS ${CMAKE_CURRENT_SOURCE_DIR})

find_package(OpenMP)
find_package(Vulkan)
include_directories(${Vulkan_INCLUDE_DIR})

include_directories("external/vkutils" 
                    "external/volk" 
                    "external/LiteMath"
                    "external/tinyexr"
                    "external/tinyexr/deps/miniz")

link_directories("external/volk")

add_compile_definitions(USE_VOLK)
if(USE_ISPC)
  add_compile_definitions(USE_ISPC)
endif()

#uncomment this to detect broken memory problems via gcc sanitizers
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize-address-use-after-scope -fno-omit-frame-pointer -fsanitize=leak -fsanitize=undefined -fsanitize=bounds-strict")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wno-attributes")

set(MAIN_SOURCE main.cpp 
                reinhard.cpp
                external/LiteMath/Image2d.cpp
                imageutils.cpp)

set(ISPC_SOURCE reinhard_ispc.cpp
                reinhard_kernels.o
                ../LiteMathAux/ispc_tasksys.cpp)

set(VKGEN_SOURCE reinhard_generated.cpp
                 reinhard_generated_init.cpp
                 reinhard_generated_ds.cpp)

set(VKUTILS_SOURCE external/vkutils/vk_utils.cpp
                   external/vkutils/vk_copy.cpp
                   external/vkutils/vk_buffers.cpp
                   external/vkutils/vk_images.cpp
                   external/vkutils/vk_context.cpp
                   external/vkutils/vk_alloc_simple.cpp
                   external/vkutils/vk_pipeline.cpp
                   external/vkutils/vk_descriptor_sets.cpp)    

add_library(miniz external/tinyexr/deps/miniz/miniz.c)

if(USE_ISPC)

  add_executable(testapp ${MAIN_SOURCE} 
                         ${VKGEN_SOURCE} 
                         ${VKUTILS_SOURCE} 
                         ${ISPC_SOURCE})
  target_link_libraries(testapp LINK_PUBLIC miniz ${Vulkan_LIBRARY} ${FREEIMAGE_LIBRARIES} OpenMP::OpenMP_CXX volk dl)        

elseif(USE_VULKAN)

  add_subdirectory(external/volk)
  add_compile_definitions(USE_VOLK)
  add_compile_definitions(USE_VULKAN)

  add_executable(testapp ${MAIN_SOURCE} 
                         ${VKGEN_SOURCE} 
                         ${VKUTILS_SOURCE})

  target_link_libraries(testapp LINK_PUBLIC miniz ${Vulkan_LIBRARY} ${FREEIMAGE_LIBRARIES} OpenMP::OpenMP_CXX volk dl)            

else()
  
  add_executable(testapp ${MAIN_SOURCE})
  target_link_libraries(testapp LINK_PUBLIC miniz ${Vulkan_LIBRARY} ${FREEIMAGE_LIBRARIES} OpenMP::OpenMP_CXX)  

endif()

set_target_properties(testapp PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
