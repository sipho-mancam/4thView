#pragma once
#include <tuple>
#include <vector>
#include <nlohmann/json.hpp>


using json = nlohmann::json;

class CoordinateSpaceTransform
{
public:
	CoordinateSpaceTransform();

	static std::tuple<double,double> screen2Catersian_n(std::tuple<double, double> coordinates);

	static std::vector<json> convertBatch(std::vector<json> tracks);
	static json convertFromFrameData(json);
	
};