#!/bin/sh

# installs paraview in ./pv-linux-install
# sudo apt install qttools5-dev lzma-dev libxt-dev libqt5x11extras5-dev qtxmlpatterns5-dev-tools 

set -e

export QT_SELECT=qt5


git clone https://gitlab.kitware.com/paraview/paraview.git || echo "ok" 
cd paraview
git checkout v5.5.1
git submodule init
git submodule update

mkdir -p build && cd build

cmake \
  -DBUILD_SHARED_LIBS=ON \
  -DCMAKE_BUILD_TYPE=Release \
  -DVTK_USE_64BIT_IDS=OFF \
  -DVTK_NO_PYTHON_THREADS=OFF \
  -DVTK_PYTHON_FULL_THREADSAFE=ON \
  -DPARAVIEW_ENABLE_PYTHON=ON \
  -DPARAVIEW_ENABLE_EMBEDDED_DOCUMENTATION=OFF \
  -DBUILD_DOCUMENTATION=OFF \
  -DVTK_USE_OGGTHEORA_ENCODER=ON \
  -DVTK_USE_SYSTEM_OGGTHEORA=ON \
  -DVTK_USE_SYSTEM_FREETYPE=ON \
  -DVTK_USE_SYSTEM_GL2PS=OFF \
  -DVTK_USE_SYSTEM_JPEG=ON \
  -DVTK_USE_SYSTEM_NETCDF=ON \
  -DVTK_USE_SYSTEM_GLEW=ON \
  -DVTK_USE_SYSTEM_PNG=ON \
  -DVTK_USE_SYSTEM_TIFF=ON \
  -DVTK_USE_SYSTEM_ZLIB=ON \
  -DVTK_USE_SYSTEM_LIBXML2=ON \
  -DVTK_USE_SYSTEM_EXPAT=ON \
  -DVTK_USE_SYSTEM_HDF5=ON \
  -DPARAVIEW_ENABLE_CATALYST:BOOL=ON \
  -DPARAVIEW_BUILD_QT_GUI:BOOL=ON \
  -DPARAVIEW_QT_VERSION=5 \
  -DVTK_BUILD_QT_DESIGNER_PLUGIN:BOOL=OFF \
  -DPARAVIEW_ENABLE_MATPLOTLIB:BOOL=ON \
  -DCMAKE_INSTALL_RPATH=$PWD/../../pv-linux-install/lib/paraview \
  -DCMAKE_INSTALL_RPATH_USE_LINK_PATH=OFF  \
  -DCMAKE_SKIP_RPATH=OFF \
  -DVTK_CUSTOM_LIBRARY_SUFFIX="" \
  -DCMAKE_INSTALL_PREFIX=$PWD/../../pv-linux-install \
  -DVTK_INSTALL_LIBRARY_DIR="lib/paraview" \
  -DVTK_INSTALL_ARCHIVE_DIR="lib/paraview" \
  -DVTK_INSTALL_INCLUDE_DIR="include/paraview" \
  -DVTK_INSTALL_DATA_DIR="share/paraview" \
  -DVTK_INSTALL_DOC_DIR="share/doc/paraview" \
  -DVTK_INSTALL_PACKAGE_DIR="lib/cmake/paraview" \
  -DVTK_USE_SYSTEM_PROTOBUF=OFF \
  -DPARAVIEW_USE_VTKM=OFF \
  -DPARAVIEW_INSTALL_DEVELOPMENT_FILES:BOOL=ON \
  ..
make install -j8
cd ../..
