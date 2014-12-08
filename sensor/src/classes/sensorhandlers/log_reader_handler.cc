#include "log_reader_handler.h"
#include "classes/logger.h"
#include "../measurements.h"
#include "../clock/softwareclock.h"

#include <vector>
#include <stdexcept>

static std::vector<std::string> SplitString(const std::string &input, char delim)
{
    std::vector<std::string> elems;
    std::string item;

    std::stringstream inputStringStream{ input };
    while (std::getline(inputStringStream, item, delim))
    {
        if (!item.empty())
        {
            elems.push_back(item);
        }
    }

    return elems;
}

LogReader::LogReader(const SoftwareClock &clock_, const std::string& logFile) : clock(clock_)
{
    std::ifstream logStream{ logFile };
    if (!logStream.is_open())
    {
        throw std::runtime_error("Could not open log file: " + logFile);
    }

    std::string inputLine;
    while (std::getline(logStream, inputLine))
    {
        Logger::Log(LogLevel::Debug) << inputLine;

        try
        {
            measurementList.push_back(CreateMeasurement(inputLine));
        }
        catch (std::exception &e)
        {
            Logger::Log(LogLevel::Warning) << "Could not parse measurement";
            Logger::Log(LogLevel::Warning) << e.what();
        }
    }
}

MeasurementBatch LogReader::GetMeasurements() const
{
    Logger::Log(LogLevel::Info) << "LogReader::GetMeasurements()";
    MeasurementBatch measurementsThisFrame;

    const U32 currentTimeStamp = clock.GetTimeStampInMicroSecs();
    Logger::Log(LogLevel::Info) << "Reading until: " << currentTimeStamp;

    while (!measurementList.empty())
    {
        if (measurementList.front()->GetTimeStamp() < currentTimeStamp)
        {
            measurementsThisFrame.push_back(std::move(measurementList.front()));
            measurementList.erase(measurementList.begin());
        }
        else
        {
            break;
        }
    }

    return measurementsThisFrame;
}

bool LogReader::HasAvailableMeasurements() const
{
    Logger::Log(LogLevel::Info) << "LogReader::HasAvailableMeasurements()";
    return false;
}

MeasurementPtr LogReader::CreateMeasurement(const std::string &inputLine) const
{
    const std::vector<std::string> tokenizedString = SplitString(inputLine, ',');
    const U32 timeStamp{ std::stoul(tokenizedString.at(0)) };
    const std::string measurementStr{ tokenizedString.at(1) };

    if (measurementStr == "ACC")
    {
        return MeasurementPtr{ new AccelerometerMeasurement{ timeStamp,
            std::stof(tokenizedString.at(2)),
            std::stof(tokenizedString.at(3)),
            std::stof(tokenizedString.at(4)) } };
    }
    else if (measurementStr == "MAG")
    {
        return MeasurementPtr{ new CompassMeasurement{ timeStamp,
            std::stof(tokenizedString.at(2)),
            std::stof(tokenizedString.at(3)),
            std::stof(tokenizedString.at(4)) } };
    }
    else if (measurementStr == "GYRO")
    {
        return MeasurementPtr{ new GyroscopeMeasurement{ timeStamp,
            std::stof(tokenizedString.at(2)),
            std::stof(tokenizedString.at(3)),
            std::stof(tokenizedString.at(4)) } };
    }
    else if (measurementStr == "BARO")
    {
        return MeasurementPtr{ new PressureMeasurement{ timeStamp,
            std::stof(tokenizedString.at(2)) } };
    }
    else if (measurementStr == "TEMP")
    {
        return MeasurementPtr{ new TemperatureMeasurement{ timeStamp,
            std::stof(tokenizedString.at(2)) } };
    }
    else
    {
        throw std::runtime_error{ "Unknown measurement type: " + measurementStr };
    }
}
