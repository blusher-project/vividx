INCLUDE_DIR_FLAG=-I../../ -I../../include
LIB_DIR_FLAGS=-L../../out/Debug -lvividx -lfreetype -lz -lpng16 -ljpeg
TESTS_CXX_FILES=../../tests/Test.cpp ../../tools/flags/CommandLineFlags.cpp

default:
	$(CXX) -DSK_R32_SHIFT=16 $(INCLUDE_DIR_FLAG) $(TESTS_CXX_FILES) RectTest.cpp $(LIB_DIR_FLAGS) ../../build/libvividx-partial.a -o RectTest
