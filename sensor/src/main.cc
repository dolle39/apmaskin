/* This file contains the entry point for the OrientationFilter application */

#include <iostream>
#include <string>
#include <stdexcept>
#include <utility>

#include "headers/smart_pointer_typedefs.h"

#include "interfaces/imu.h"
#include "interfaces/kalman_model.h"
#include "classes/sensorhandlers/factories/log_reader_factory.h"
#include "classes/sensorhandlers/factories/default_handler_factory.h"
#include "classes/clock/softwareclock.h"
#include "classes/clock/hardwareclock.h"
#include "classes/filter/gyro_input_model.h"
#include "classes/alt_imu.h"

#include "server/server.h"
#include "server/job.h"

#include "sensor_app.h"

//void PrintAndLogMeasurements(const MeasurementBatch& measurementBatch);
void SendTest(ConnectionManager &connectionManager);

int main(int argc, char **argv)
{
    /* Create default factory if compiling for target, else log reader factory. */
    try
    {
        #ifdef _MSC_VER
            std::string logFileName{"logs/measurementslog_D2014-12-09_T18-49_180_degrees_rotation_and_back.txt"};
            if (argc > 1)
            {
                logFileName = argv[1];
            }
            SoftwareClock clock;
            SensorHandlerFactoryPtr factory{new LogReaderFactory{clock, logFileName}};
        #else
            HardwareClock clock;
            SensorHandlerFactoryPtr factory{new DefaultHandlerFactory{clock}};
        #endif

        IMUPtr imu{new AltIMU{factory}};
        KalmanModelPtr model{new GyroInputModel};
        SensorApp sensorApp{std::move(imu), std::move(model), clock};

        asio::io_service ioService;
        Server server(ioService, SensorApp::serverPort);
        ConnectionManager &connectionManager = server.GetConnectionManager();

        std::function<void()> updateFunction{std::bind(&SensorApp::Update, &sensorApp)};
        Job updateJob{ioService, updateFunction, std::chrono::milliseconds{50}};

        std::function<void()> sendTestFunction = std::bind(SendTest, std::ref(connectionManager));
        Job testSendJob{ ioService, sendTestFunction, std::chrono::milliseconds{1000}};

        ioService.run();
    }
    catch (const std::exception& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cout << "An unknown error occured." << std::endl;
        return 1;
    }

    return 0;
}


void SendTest(ConnectionManager &connectionManager)
{
    Message msg;
    msg.SetMsgType(1);
    msg.SetBodyLength(3);
    msg.EncodeHeader();
    msg.WriteChar('H');
    msg.WriteChar('e');
    msg.WriteChar('j');
    connectionManager.SendToAll(msg);

    Message msg2;
    msg2.SetMsgType(2);
    msg2.SetBodyLength(sizeof(F32) * 3);
    msg2.EncodeHeader();
    msg2.WriteFloat(1.0f);
    msg2.WriteFloat(10.0f);
    msg2.WriteFloat(15.0f);
    connectionManager.SendToAll(msg2);
}
/*
void PrintAndLogMeasurements(const MeasurementBatch& measurementBatch)
{
    #ifndef _MSC_VER
        static LoggerVisitor loggerVisitor{ "measurementslog.txt"};
        for (const auto& measurement : measurementBatch)
        {
            measurement->Accept(loggerVisitor);
        }
    #endif

    for (const auto& measurement : measurementBatch)
    {
        Logger::Log(LogLevel::Debug) << measurement->ToString();
    }
}
*/