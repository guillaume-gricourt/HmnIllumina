TARGET=HmnIllumina

DIRINC=./include
DIRSRC=./src
DIROBJ=./obj
DIRLIB=./lib

SRC=$(wildcard $(DIRSRC)/*.cpp)
OBJ=$(patsubst %.cpp,$(DIROBJ)/%.o,$(notdir $(SRC)))

CXX=g++
LIBS = -I include -l interop_lib -L lib/interop-1.1.8/lib64 -I lib/interop-1.1.8/include -I lib/rapidjson-1.1.0
CXXFLAGS = -std=c++11 -Wall -O3 -lz -fopenmp

all: $(TARGET)

$(TARGET):$(OBJ)
	$(CXX) $(OBJ) $(LIBS) $(CXXFLAGS) -o $@

$(DIROBJ)/%.o:$(DIRSRC)/%.cpp
	@[ -d $(DIROBJ) ] || mkdir $(DIROBJ)
	$(CXX) $(LIBS) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(TARGET)
	$(RM) -r $(DIROBJ)

test:
	$(MAKE)
	cp $(TARGET) tests/
	python -m pytest tests
	$(RM) tests/$(TARGET)
