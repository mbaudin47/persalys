#!/bin/sh

set -x -e

uid=$1
gid=$2

cd /tmp

mkdir -p build && cd build
MOD_PREFIX=$PWD/install
LDFLAGS="-no-pie" CXXFLAGS="-Wall -Wextra -Werror -D_GLIBCXX_ASSERTIONS -fno-pie -fno-pic -g -fno-inline" ${ARCH}-w64-mingw32-cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_SHARED_LIBS=OFF \
  -DUSE_SPHINX=OFF -DUSE_SALOME=OFF \
  -DCMAKE_INSTALL_PREFIX=${MOD_PREFIX} \
  -DPYTHON_INCLUDE_DIR=${MINGW_PREFIX}/include/python${PYMAJMIN} \
  -DPYTHON_LIBRARY=${MINGW_PREFIX}/lib/libpython${PYMAJMIN}.dll.a \
  -DPYTHON_EXECUTABLE=/usr/bin/${ARCH}-w64-mingw32-python${PYMAJMIN}-bin \
  -DPYTHON_SITE_PACKAGES=Lib/site-packages \
  -DSWIG_COMPILE_FLAGS="-Wno-error=unused-parameter" \
  -DCMAKE_UNITY_BUILD=ON -DCMAKE_UNITY_BUILD_BATCH_SIZE=32 /io
make install
# ${ARCH}-w64-mingw32-strip --strip-unneeded ${MOD_PREFIX}/bin/*.dll ${MOD_PREFIX}/Lib/site-packages/persalys/*.pyd
cp ${MINGW_PREFIX}/bin/*.dll ${MOD_PREFIX}/bin
# make tests
# WINEPATH="${MINGW_PREFIX}/bin;${MOD_PREFIX}/bin" xvfb-run -s "-screen 0 1024x768x24" ctest --output-on-failure --timeout 100 -j8

# module installer
cp -r ${MINGW_PREFIX}/lib/qt/plugins/{platforms,imageformats} ${MOD_PREFIX}/lib
cp -rv ${MINGW_PREFIX}/bin/paraview*/plugins/BagPlotViewsAndFilters ${MOD_PREFIX}/bin
cp /usr/${ARCH}-w64-mingw32/etc/openturns/openturns.conf ${MOD_PREFIX}/Lib/site-packages/persalys/
VERSION=`cat /io/VERSION`
cp /io/distro/windows/* .
unzip persalys-doc.zip -d .
makensis -DMODULE_PREFIX=${MOD_PREFIX} -DMODULE_VERSION=${VERSION} -DOPENTURNS_VERSION=1.14 -DPYBASEVER=${PYMAJMIN:0:1}.${PYMAJMIN:1:1} -DPYBASEVER_NODOT=${PYMAJMIN} -DARCH=${ARCH} installer.nsi

# bundle installer
cd /tmp
PYVER=`${ARCH}-w64-mingw32-python${PYMAJMIN}-bin -V|sed "s|.*Python \([0-9\.]*\).*|\1|g"`
curl -fSsLO https://www.python.org/ftp/python/${PYVER}/python-${PYVER}-embed-amd64.zip
curl -fsSLO https://bootstrap.pypa.io/get-pip.py && sudo ${ARCH}-w64-mingw32-python${PYMAJMIN}-bin get-pip.py
mkdir -p python_root/Lib/site-packages && cd python_root
unzip /tmp/python-${PYVER}-embed-amd64.zip
echo 'Lib\\site-packages' >> python${PYMAJMIN}._pth
curl -fSsL https://anaconda.org/conda-forge/vs2015_runtime/14.0.25420/download/win-64/vs2015_runtime-14.0.25420-0.tar.bz2 | tar xj
cp -r /usr/${ARCH}-w64-mingw32/Lib/site-packages/openturns Lib/site-packages
cp /usr/${ARCH}-w64-mingw32/bin/*.dll Lib/site-packages/openturns
cp /usr/${ARCH}-w64-mingw32/etc/openturns/openturns.conf Lib/site-packages/openturns
rm Lib/site-packages/openturns/{libvtk,libboost,LLVM,Qt,python}*.dll
cp -rv /usr/${ARCH}-w64-mingw32/Lib/site-packages/otmorris Lib/site-packages
cp -rv /usr/${ARCH}-w64-mingw32/Lib/site-packages/{pip*,setuptools*,wheel*,pkg_resources,easy_install.py} Lib/site-packages/
mkdir Scripts && echo -e 'import sys\nfrom pip import main\nsys.exit(main())\n' > Scripts/pip.py && echo -e 'python %~dp0pip.py %*' > Scripts/pip.bat
cd /tmp/build
makensis -DMODULE_PREFIX=${MOD_PREFIX} -DMODULE_VERSION=${VERSION} -DPYTHON_PREFIX=/tmp/python_root -DPYBASEVER=${PYMAJMIN:0:1}.${PYMAJMIN:1:1} -DARCH=${ARCH} installer_bundle.nsi

# copy to host with same permission
if test -n "${uid}" -a -n "${gid}"
then
  sudo cp persalys*.exe /io
  sudo chown ${uid}:${gid} /io/persalys*.exe
fi
