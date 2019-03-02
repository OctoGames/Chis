REM Ejecutar este bat con la Consola de Windows
REM La carpeta de CMake debe estar en Dependencies
REM Este script debe ejecutarse desde el  directorio del ejecutable de CMake

cmake -S ..\..\ogre-1.11.5 -B ..\..\ogre-1.11.5\Build -G "Visual Studio 15 2017" -A Win32