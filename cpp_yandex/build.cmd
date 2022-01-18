rem @echo off
rem echo "INIT MSVS Environment"
cd "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\" 
call vcvars64.bat
cd %~dp0

if exist build (
    echo "DELETE build directory"
    rd /s /q build
)

echo "CREATE build directory"
md build

cd build

echo "call cmake"
cmake ^
    -G "Visual Studio 16 2019" ^
	-DBOOST_ROOT=C:\Users\Andrey\Documents\post_msu_prac\boost_1_69_0_win ^
	-DBOOST_INCLUDEDIR=C:\Users\Andrey\Documents\post_msu_prac\boost_1_69_0_win ^
	-DBOOST_LIBRARYDIR=C:\Users\Andrey\Documents\post_msu_prac\boost_1_69_0_win\stage\lib ^
    rem -DCMAKE_PREFIX_PATH=C:\Users\Andrey\Documents\post_msu_prac\poco-1.10.1-all\poco-1.10.1-all\cmake-build ^
    -A x64 ..

cmake --build . --config Release -- -m:12
cmake --build . --config Debug -- -m:12
