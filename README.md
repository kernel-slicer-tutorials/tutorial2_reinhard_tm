# tutorial2_reinhard_tm
Simple implemetation of Tone Mapping with parellel reduction inside

# Build (CPU):

1. clone this repo:
   * git clone https://github.com/kernel-slicer-tutorials/tutorial2_reinhard_tm.git
   * cd tutorial2_reinhard_tm
2. clone submodules:
   * git submodule init && git submodule update
3. use Cmake to build project:
   * mkdir cmake-build-release && cd cmake-build-release
   * cmake -DCMAKE_BUILD_TYPE=Release ..
   * make -j 8

# Build (GPU):

1. clone and build kernel_slicer (https://github.com/Ray-Tracing-Systems/kernel_slicer) in some directory
2. run kernel_slicer via provided scripts:
  * bash setup.sh my/path/kernel_slicer/cmake-build-release/kslicer my/path/to/kernel_slicer (once)
  * bash run_kslicer.sh

3. build shaders:
   * cd example_tracer/shaders_generated && bash build.sh
   * or use record 'Build Shaders (GLSL, example_tracer)' in vs code config 'tasks.json': just press 'Ctr+Shift+B'
4. use Cmake to build project with 'USE_VULKAN' flag == 'ON':
   * mkdir cmake-build-release && cd cmake-build-release
   * cmake -DCMAKE_BUILD_TYPE=Release -DUSE_VULKAN=ON ..
   * make -j 8
   * Note that the program will seek for 'example_tracer/shaders_generated/kernel2D_RayMarch.comp.spv'
   * Therefore you have to run it from LiteRF root folder (what is done by default in VS Code config) or copy appropriate SPIR-V file to get 'cmake-build-release/example_tracer/shaders_generated/kernel2D_RayMarch.comp.spv'