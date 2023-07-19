all:
	g++ -o VR.exe *.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -Wall -std=c++17
