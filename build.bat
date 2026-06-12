@echo off
rem Hardcoded compile commands — one line per source file

cl /nologo /EHsc windows\exercise-1-securing-sections\unnamed\privileged.cpp /link /out:privileged-unnamed.exe && del privileged.obj
@REM cl /nologo /EHsc windows\exercise-1-securing-sections\unnamed\privileged-patched.cpp /link /out:privileged-patched.exe && del privileged-patched.obj
cl /nologo /EHsc windows\exercise-1-securing-sections\unnamed\lesser-privileged.cpp /link /out:lesser-privileged-unnamed.exe && del lesser-privileged.obj

@REM cl /nologo /EHsc windows\exercise-1-securing-sections\named\privileged.cpp /link advapi32.lib /out:windows\exercise-1-securing-sections\named\privileged.exe && del privileged.obj
@REM cl /nologo /EHsc windows\exercise-1-securing-sections\named\lesser-privileged.cpp /link /out:windows\exercise-1-securing-sections\named\lesser-privileged.exe && del lesser-privileged.obj

echo Done.
exit /b 0