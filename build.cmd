@echo off

cl /nologo /c test.c base64.c
link /NOLOGO test.obj base64.obj
del test.obj base64.obj
