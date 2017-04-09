@echo off
rem ////設定////
set Folder=ShaderCompiler
set Pass=%CD%\%Folder%\
set ShaderModel=vs_3_0

rem ////出力先のフォルダー(相対パス)////
set Release=ShaderFiles

rem ////フォルダ判定////
if EXIST "%Folder%\" ( 
	rem ////実行////
	if EXIST "%Release%\" (
		%Pass%ShaderCompiler.exe /Fo%CD%\%Release%\%~n1.vso /T%ShaderModel% %1
	) else (
		mkdir %CD%\%Release%
		%Pass%ShaderCompiler.exe /Fo%CD%\%Release%\%~n1.pso /T%ShaderModel% %1
	)
) else (
	echo error: "%Folder%"フォルダーがありません。 
)
pause