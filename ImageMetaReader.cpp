#include "ImageMetaReader.h"
#include <cmath>
#include <iostream>

#include<cstring>

bool ImageMetaReader::load(const std::string& filePath)
{
    try
    {

        Exiv2::Image::UniquePtr image = Exiv2::ImageFactory::open(filePath);
        if (!image) return false;

        image->readMetadata();

        return parseExif(image);
    }
    catch (...)
    {
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

    Exiv2::Rational r1 = it->toRational(0);
    Exiv2::Rational r2 = it->toRational(1);
    Exiv2::Rational r3 = it->toRational(2);

    value = r1.first / (double)r1.second
        + r2.first / (double)r2.second / 60.0
        + r3.first / (double)r3.second / 3600.0;

    return true;
}

const double PI = 3.14159265358979323846;
const double A = 6378245.0;
const double EE = 0.00669342162296594323;

void ImageMetaReader::wgs84ToGcj02(double lat, double lon, double& outLat, double& outLon)
{
    double dLat = transformLat(lon - 105.0, lat - 35.0);
    double dLon = transformLon(lon - 105.0, lat - 35.0);

    double radLat = lat / 180.0 * PI;
    double magic = sin(radLat);
    magic = 1 - EE * magic * magic;
    double sqrtMagic = sqrt(magic);

    outLat = lat + (dLat * 180.0) / ((A * (1 - EE)) / (magic * sqrtMagic) * PI);
    outLon = lon + (dLon * 180.0) / (A / sqrtMagic * cos(radLat) * PI);
}

double ImageMetaReader::transformLat(double x, double y)
{
    return -100 + 2*x + 3*y + 0.2*y*y + 0.1*x*y + 0.2*sqrt(abs(x));
}

double ImageMetaReader::transformLon(double x, double y)
{
    return 300 + x + 2*y + 0.1*x*x + 0.1*x*y + 0.1*sqrt(abs(x));
}
