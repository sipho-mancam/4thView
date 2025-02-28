#include "track_preprocessor.hpp"

CoordinateSpaceTransform::CoordinateSpaceTransform()
{
}

std::tuple<double, double> CoordinateSpaceTransform::screen2Catersian_n(std::tuple<double, double> coordinates)
{
	const auto [x, y] = coordinates;
	double x_c = x - 0.5;
	double y_c = 0.5 - y;
	return { x_c, y_c };
}

std::vector<json> CoordinateSpaceTransform::convertBatch(std::vector<json> tracks)
{
	for (json& track : tracks)
	{
		std::tuple<double, double> coordinates = track["coordinates"];
		track["coordinates"] = CoordinateSpaceTransform::screen2Catersian_n(coordinates);
	}
	return tracks;
}

json CoordinateSpaceTransform::convertFromFrameData(json frameData)
{
	if (!frameData.contains("tracks"))
		return frameData;

	std::vector<json> tracks = frameData.at("tracks");
	tracks = convertBatch(tracks);
	frameData["tracks"] = tracks;
	return frameData;
}
