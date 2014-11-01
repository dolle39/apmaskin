/*
* Definition of a default concrete factory for the abstract factory pattern.
* This factory returns different concrete implementations I2Chandler interface.
* The factory is intended to be used by the IMU-class for dependency injection.
*/

#include "default_handler_factory.h"
#include "acc_and_mag_handler.h"
#include "gyroscope_handler.h"
#include "barometer_handler.h"

std::unique_ptr<I2CHandler> DefaultHandlerFactory::MakeAccAndMagHandler() const
{
	return std::unique_ptr<I2CHandler>{new AccAndMagHandler{}};
}

std::unique_ptr<I2CHandler> DefaultHandlerFactory::MakeGyroscopeHandler() const
{
	return std::unique_ptr<I2CHandler>{new GyroscopeHandler{}};
}

std::unique_ptr<I2CHandler> DefaultHandlerFactory::MakeBarometerHandler() const
{
	return std::unique_ptr<I2CHandler>{new BarometerHandler{}};
}
