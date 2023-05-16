commit?=Unspecified Commit
PROGRAM = main.cpp
OUTPUT = UntitledGam
SOURCES = src/*.cpp src/OpenCL/*.cpp src/rendering/*.cpp src/rendering/fontLoading/*.cpp src/ui/*.cpp src/scenes/*.cpp my.res
LIBRARIES = -L${OPENCL_LIB} -lWin32\OpenCL -lgdi32 -static-libgcc -static-libstdc++
INCLUDES = -I${OPENCL_INCLUDE}
CFLAGS = -std=c++11 -msse2 -O2

all:
	windres my.rc -O coff -o my.res
	g++ $(CFLAGS) $(PROGRAM) $(SOURCES) -o $(OUTPUT) $(INCLUDES) $(LIBRARIES)

github:
	windres my.rc -O coff -o my.res
	g++ $(CFLAGS) $(PROGRAM) $(SOURCES) -o $(OUTPUT) $(INCLUDES) $(LIBRARIES)
	git add *
	git commit -m "$(commit)"
	git push -u origin main --force

github-nocompile:
	git add *
	git commit -m "$(commit)"
	git push -u origin main --force

restore:
	git clone https://github.com/dygo454/DGE.git ../temp
	cmd /C xcopy /E /H /Y /C ..\temp\.git .git
	cmd /C RD /S /Q ..\temp
	git reset --hard
