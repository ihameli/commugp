#ifndef _PARAMETERS_H_
#define _PARAMETERS_H_

#include <iostream>

class Parameters {
  private:
	std::string bckGnd;
	std::string color;
	double percentOfEdges;
	int minEdges;
	int widthResolution;
	int heightResolution;
	double fontZoom;
	double epsilon;
	double delta;
	double gamma;
	double hstart;
	double hend;
	double vstart;
	double vend;
	double u;
	//Options: "lanet" or "nwb"
	std::string format;
	//Options: "" or "java"
	std::string show;
	std::string outputFile;
	std::string outputRendFile;
	std::string outputPngFile;
	std::string inputFile;
	std::string inputNodeNamesFile;
	std::string coresFile;
	bool multigraph;
	bool weighted;
	int granularity;
	std::string renderer;
	std::string strengthsIntervals;
	bool noCliques;
	bool kConn;
	bool connectivity;
	bool innerConnectivity;
	std::string kConnType;
	bool onlyGraphic;
	bool noGraphic;
	double opacity;
	bool isLogFile;
	int fromLayer;

  public:
	Parameters();
	std::string getBckGnd() { return bckGnd; }
	std::string getColor() { return color; }
	double getPercentOfEdges() { return percentOfEdges; }
	int getMinEdges() { return minEdges; }
	int getWidthResolution() { return widthResolution; }
	int getHeightResolution() { return heightResolution; }
	int getFontZoom() { return fontZoom; }
	double getEpsilon() { return epsilon; }
	double getDelta() { return delta; }
	double getGamma() { return gamma; }
	double getHStart() { return hstart; }
	double getHEnd() { return hend; }
	double getVStart() { return vstart; }
	double getVEnd() { return vend; }
	double getU() { return u; }
	std::string getFormat() { return format; }
	std::string getShow() { return show; }
	std::string getOutputFile() { return outputRendFile; }
	std::string getOutputRendFile() { return outputRendFile; }
	std::string getOutputPngFile() { return outputPngFile; }
	std::string getInputFile() { return inputFile; }
	std::string getInputNodeNamesFile() { return inputNodeNamesFile; }
	std::string getCoresFile() { return coresFile; }
	bool getMultigraph() { return multigraph; }
	bool getWeighted() { return weighted; }
	int getGranularity() { return granularity; }
	std::string getRenderer() { return renderer; }
	std::string getStrengthsIntervals() { return strengthsIntervals; }
	bool getNoCliques() { return noCliques; }
	bool getKConn() { return kConn; }
	bool getConnectivity() { return connectivity; }
	bool getInnerConnectivity() { return innerConnectivity; }
	std::string getKConnType() { return kConnType; }
	bool getOnlyGraphic() { return onlyGraphic; }
	bool getNoGraphic() { return noGraphic; }
	double getOpacity() { return opacity; }
	bool getIsLogFile() { return isLogFile; }
	inline int getFromLayer() { return fromLayer; }

};

//The parameters object
extern Parameters parameters;

#endif

