#include "ImageMetaReader.h"
#include <cmath>
#include <iostream>
#include <cstring>
#include <Exiv2/exiv2.hpp>

namespace {
    constexpr double PI = 3.14159265358979323846264338327950288;
    constexpr double A = 6378245.0;
    constexpr double EE = 0.00669342162296594323; 

    inline bool outOfChina(double lat, double lon) {
        return (lon < 72.004 || lon > 137.8347 || lat < 0.8293 || lat > 55.8271);
    }

    inline double transformLat(double x, double y) {
        double ret = -100.0 + 2.0 * x + 3.0 * y + 0.2 * y * y
            + 0.1 * x * y + 0.2 * sqrt(std::abs(x));
        ret += (20.0 * sin(6.0 * x * PI) + 20.0 * sin(2.0 * x * PI)) * 2.0 / 3.0;
        ret += (20.0 * sin(y * PI) + 40.0 * sin(y / 3.0 * PI)) * 2.0 / 3.0;
        ret += (160.0 * sin(y / 12.0 * PI) + 320.0 * sin(y * PI / 30.0)) * 2.0 / 3.0;
        return ret;
    }

    inline double transformLon(double x, double y) {
        double ret = 300.0 + x + 2.0 * y + 0.1 * x * x
            + 0.1 * x * y + 0.1 * sqrt(std::abs(x));
        ret += (20.0 * sin(6.0 * x * PI) + 20.0 * sin(2.0 * x * PI)) * 2.0 / 3.0;
        ret += (20.0 * sin(x * PI) + 40.0 * sin(x / 3.0 * PI)) * 2.0 / 3.0;
        ret += (150.0 * sin(x / 12.0 * PI) + 300.0 * sin(x / 30.0 * PI)) * 2.0 / 3.0;
        return ret;
    }
} // namespace

bool ImageMetaReader::load(const std::string& filePath)
{
    try {
        Exiv2::Image::UniquePtr image = Exiv2::ImageFactory::open(filePath);
        if (!image) return false;
        image->readMetadata();
        return parseExif(image);
    }
    catch (...) {
        return false;
    }
}

bool ImageMetaReader::parseExif(Exiv2::Image::UniquePtr& image)
{
    auto& exifData = image->exifData();

    auto it = exifData.findKey(Exiv2::ExifKey("Exif.Photo.DateTimeOriginal"));
    if (it != exifData.end())
        info_.captureTime = it->toString();

    bool latOK = getGpsValue(exifData, "Exif.GPSInfo.GPSLatitude", info_.wgsLat);
    bool lonOK = getGpsValue(exifData, "Exif.GPSInfo.GPSLongitude", info_.wgsLon);

    if (!latOK || !lonOK) return false;

    wgs84ToGcj02(info_.wgsLat, info_.wgsLon, info_.gcjLat, info_.gcjLon);

    char buf[256];
    snprintf(buf, sizeof(buf),
        "https://www.amap.com/regeo?lng=%.8f&lat=%.8f",
        info_.gcjLon, info_.gcjLat);
    info_.amapUrl = buf;

    return true;
}

bool ImageMetaReader::getGpsValue(const Exiv2::ExifData& exifData, const char* key, double& value)
{
    auto it = exifData.findKey(Exiv2::ExifKey(key));
    if (it == exifData.end()) return false;

    try {
        Exiv2::Rational r1 = it->toRational(0);
        Exiv2::Rational r2 = it->toRational(1);
        Exiv2::Rational r3 = it->toRational(2);

        long double deg = static_cast<long double>(r1.first) / r1.second;
        long double min = static_cast<long double>(r2.first) / r2.second;
        long double sec = static_cast<long double>(r3.first) / r3.second;

        value = static_cast<double>(deg + min / 60.0L + sec / 3600.0L);
        return true;
    }
    catch (...) {
        return false;
    }
}

void ImageMetaReader::wgs84ToGcj02(double lat, double lon, double& outLat, double& outLon)
{
    if (outOfChina(lat, lon)) {
        outLat = lat;
        outLon = lon;
        return;
    }

    double dLat = transformLat(lon - 105.0, lat - 35.0);
    double dLon = transformLon(lon - 105.0, lat - 35.0);

    double radLat = lat / 180.0 * PI;
    double magic = sin(radLat);
    magic = 1 - EE * magic * magic;
    double sqrtMagic = sqrt(magic);

    outLat = lat + (dLat * 180.0) / ((A * (1 - EE)) / (magic * sqrtMagic) * PI);
    outLon = lon + (dLon * 180.0) / (A / sqrtMagic * cos(radLat) * PI);
}
