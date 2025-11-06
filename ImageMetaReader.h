#pragma once
#include <string>
#include <exiv2/exiv2.hpp>

struct ImageMetaInfo
{
    std::string captureTime;

    double wgsLat = 0.0;
    double wgsLon = 0.0;

    double gcjLat = 0.0;
    double gcjLon = 0.0;

    std::string amapUrl;
};

class ImageMetaReader
{
public:
    bool load(const std::string& filePath);
    ImageMetaInfo getInfo() const { return info_; }

private:
    ImageMetaInfo info_;

    bool parseExif(Exiv2::Image::UniquePtr& image);
    bool getGpsValue(const Exiv2::ExifData& exifData, const char* key, double& value);

    void wgs84ToGcj02(double lat, double lon, double& outLat, double& outLon);
    double transformLat(double x, double y);
    double transformLon(double x, double y);
};
