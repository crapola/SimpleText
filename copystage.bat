@echo off
set dest="D:\Users\Utilisateur\GitStage\SimpleText\"
REM /D=more recent /E=files&folders
xcopy "." %dest% /D /E /EXCLUDE:exclude.txt