all: project1

project1:
	g++ -std=c++11 prog1.cpp -o prog1 -I/usr/local/include/opencv4 -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs
	g++ -std=c++11 prog2.cpp -o prog2 -I/usr/local/include/opencv4 -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs
clean:
	rm prog1 prog2