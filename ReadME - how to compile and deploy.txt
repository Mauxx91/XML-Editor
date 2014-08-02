To run go to : deployed version and open MultiplatformFileAnalyzer.exe
if it doesn't work install vcredist_x86.exe contained in the same directory

Requirements for compile:

	1)Qt 5.3.1 for Windows 32-bit (VS 2012, OpenGL, 553 MB): downloadable from here http://qt-project.org/downloads under other "Other downloads"
		you can directly download with this: http://download.qt-project.org/official_releases/qt/5.3/5.3.1/qt-opensource-windows-x86-msvc2012_opengl-5.3.1.exe
		When you are installing Qt it's advised to select the install Qt Creator too (under tools branch
	
	2)Visual studio 2012 compiler.

Compile and execute:

	It's possible to do that in two different ways:
	
	1) Open the .pro file and edit with Qt Creator
		a) use build button (in order to make the other way possible you almost have to run qmake for the release )
	
	2) Using command prompt:
		a) open a command prompt
		b) set the visual studio 2012 variables by:
			*1) drag and drop "C:\Program Files (x86)\Microsoft Visual Studio 11.0\Common7\Tools\vsvars32.bat" and press start two times
				(now the command nmake and qmake will be available)
		c) go to the DataLastTimeAnalyzer forder
		d) use this command "qmake -config release 'absolute path to the .pro file'" or debug
		e) use this command "nmake" (use "nmake clean" to clean)
		d) the complied it can be found in C:\Qt\Qt5.3.1\5.3\msvc2012_opengl under the release or debug directory.
		
To deploy:

	1) Make sure that VCINSTALLDIR environment variable is setted with the dir of the installation of Visual Studio 2012 (x86)
	
	2) Make sure that Path environment variable is setted with the bin of Qt. The standard location is C:\Qt\Qt5.3.1\5.3\msvc2012_opengl\bin

	3)Open a command prompt and run "C:\Qt\Qt5.3.1\5.3\msvc2012_opengl\bin\windeployqt.exe pathOfADirectoryOfReleaseExe"
	
		a) substitute pathOfADirectoryOfReleaseExe with a empty directory with only a exe of a release build
		
	4) Distribute all that directory which contains all the stuff needed to run the application, included the exe of the visual studio runtime