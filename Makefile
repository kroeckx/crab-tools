all: crab_import

crab_import: crab_import.C
	g++ -g -Wall crab_import.C -o crab_import -lpq -lshp

