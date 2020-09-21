all: project1 run

project1: main.cpp
	g++ -std=c++11 main.cpp -o main -I/usr/local/include/opencv4 -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs

run: project1
	./main
clean:
	rm main