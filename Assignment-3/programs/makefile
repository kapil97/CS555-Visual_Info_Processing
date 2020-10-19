all: project3 run

project3:
	g++ -std=c++11 main.cpp -o main -I/usr/local/include/opencv4 -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs

run:
	./main	

clean:
	rm main