@echo off
rem ////�ݒ�////
set Folder=ShaderCompiler
set Pass=%CD%\%Folder%\
set ShaderModel=vs_3_0

rem ////�o�͐�̃t�H���_�[(���΃p�X)////
set Release=ShaderFiles

rem ////�t�H���_����////
if EXIST "%Folder%\" ( 
	rem ////���s////
	if EXIST "%Release%\" (
		%Pass%ShaderCompiler.exe /Fo%CD%\%Release%\%~n1.vso /T%ShaderModel% %1
	) else (
		mkdir %CD%\%Release%
		%Pass%ShaderCompiler.exe /Fo%CD%\%Release%\%~n1.pso /T%ShaderModel% %1
	)
) else (
	echo error: "%Folder%"�t�H���_�[������܂���B 
)
pause