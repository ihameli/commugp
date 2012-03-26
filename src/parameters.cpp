#include "parameters.h"
#include <sstream>
#include "stdlib.h"
#include "string.h"
#include <fstream>

Parameters parameters;

Parameters::Parameters() {
	bckGnd = "black";
	color = "col";
	percentOfEdges = 0.0;
	minEdges = 1000;
	widthResolution = 800;
	heightResolution = 600;
	fontZoom = 1.0;
	epsilon = 0.0;
	delta = 1.0;
	gamma = 1.3;

epsilon = 0.18;
delta = 1.3;
gamma = 1.5;

	hstart = 0.0;
	hend = 1.0;
	vstart = 0.0;
	vend = 1.0;
	u = 1.0;
	format = "lanet";
	//Options: "" or "java"
	show = "";
	outputFile = "";
	outputRendFile = "";
	outputPngFile = "";
	inputFile = "";
	inputNodeNamesFile = "";
	coresFile = "";
	multigraph = false;
	weighted = false;
	granularity = -1;
	renderer = "povray";
	strengthsIntervals = "equalIntervalSize";
	noCliques = false;
	kConn = false;
	kConnType = "wide";
	connectivity = false;
	innerConnectivity = false;
	onlyGraphic = false;
	noGraphic = false;
	opacity = 0.2;
	isLogFile = false;
	fromLayer = 0;
}

