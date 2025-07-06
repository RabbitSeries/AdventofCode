@echo off
setlocal

if "%~1"=="" (
    echo Please provide working directory.
    echo Usage: %~nx0 workingPath
    goto :end
)

@REM No blank characters within.
set workingPath=%~1
cd %workingPath%
echo Performed project clean at
echo %workingPath%
@REM extension specified files
for /r "%workingPath%" %%p in (*.exe) do del /q "%%p"
for /r "%workingPath%" %%p in (*.dll) do del /q "%%p"
for /r "%workingPath%" %%p in (*.o)   do del /q "%%p"
for /r "%workingPath%" %%p in (*.obj) do del /q "%%p"
for /r "%workingPath%" %%p in (*.pdb) do del /q "%%p"
for /r "%workingPath%" %%p in (*.ilk) do del /q "%%p"
for /r "%workingPath%" %%p in (*.out) do del /q "%%p"
for /r "%workingPath%" %%p in (*.ilk) do del /q "%%p"
echo Deleted *.exe, *.dll, *.o, *.out, *.obj, *.pdb, *.ilk in %workingPath% recursively.
@REM directories /s removes all subdirectories.
@REM if exist "%workingPath%\.vs" rd /s /q "%workingPath%\.vs" && echo Remoed directory %workingPath%\.vs
if exist "%workingPath%\build" rd /s /q "%workingPath%\build" && echo Remoed directory %workingPath%\build
@REM if exist "%workingPath%\obj" rd /s /q "%workingPath%\obj" && echo Remoed directory %workingPath%\.obj
@REM if exist "%workingPath%\bin" rd /s /q "%workingPath%\bin" && echo Remoed directory %workingPath%\.bin
@REM 
for /r "%workingPath%\AssemblyOutput" %%p in (*.*) do del /q "%%p"
:end
endlocal