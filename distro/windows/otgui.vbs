dim fso: set fso = CreateObject("Scripting.FileSystemObject")
dim currentDirectory: currentDirectory = fso.GetParentFolderName(WScript.ScriptFullName)
Set wshShell = CreateObject("WScript.Shell")
Set wshEnv = wshShell.Environment("Process")

rem override default Qt5Agg backend (on conda) to avoid conflicts with newer MinGW Qt5 libs
wshEnv("MPLBACKEND") = "TkAgg"

rem add Python directory in Environment variable PATH
wshEnv("PATH") = wshEnv("PATH") & ";" & fso.GetAbsolutePathName(currentDirectory + "\..\..\..")

rem add documentation path in Environment variable OTGUI_HTML_DOCUMENTATION_PATH
wshEnv("OTGUI_HTML_DOCUMENTATION_PATH") = currentDirectory + "\doc\html\"

rem need quotes if contains spaces (C:\Program Files)
wshShell.run Chr(34) & fso.BuildPath(currentDirectory, "otgui.exe") & Chr(34)
