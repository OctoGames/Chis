mkdir Exports
for %%f in (*.mesh) do (
    ..\..\..\Dependencies\ogre-1.11.5\Build\bin\release\OgreMeshUpgrader.exe "%%~nf.mesh" "Exports\%%~nf.mesh"
)