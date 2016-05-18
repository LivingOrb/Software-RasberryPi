#include "GY85.hpp"

#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

extern "C"
{
#include <adxl345.h>
#include <hmc5883l.h>
}

#define CHECK(S) if (S) return false;

const int GY85::MaxAdapters = 2;

GY85::GY85()
	: aFile(0)
	, cFile(0)
	, aX(-261.3f, 273.1f)
	, aY(-258.8f, 265.3f)
	, aZ(-297.2f, 220.4f)
	, cX(-669.9f, 504.8f)
	, cY(-699.7f, 403.1f)
	, cZ(-658.7f, 324.9f)
{}

GY85::~GY85()
{
	close(aFile);
	close(cFile);
}

bool GY85::initialize()
{
	int adapter;
	char filename[20];

	for (adapter = 0; adapter < MaxAdapters; ++adapter)
	{
		snprintf(filename, 20, "/dev/i2c-%d", adapter);
		if (!access(filename, R_OK | W_OK))
			break;
	}

	if (adapter == MaxAdapters)
	{
		fprintf(stderr, "No I2C adapter found\n");
		return false;
	}

	aFile = open(filename, O_RDWR);
	if (!aConfigure())
	{
		fprintf(stderr, "Failed to initialize accelerometer\n");
		return false;
	}

	cFile = open(filename, O_RDWR);
	if (!cConfigure())
	{
		fprintf(stderr, "Failed to initialize compass\n");
		return false;
	}

	ADXL345_ReadData(aFile, &aX.data, &aY.data, &aZ.data);
	HMC5883L_ReadData(cFile, &cX.data, &cY.data, &cZ.data);

	aX.raw();
	aY.raw();
	aZ.raw();
	cX.raw();
	cY.raw();
	cZ.raw();

	return true;
}

void GY85::update(float dt)
{
	ADXL345_ReadData(aFile, &aX.data, &aY.data, &aZ.data);
	HMC5883L_ReadData(cFile, &cX.data, &cY.data, &cZ.data);

	aX.smooth(dt);
	aY.smooth(dt);
	aZ.smooth(dt);
	cX.smooth(dt);
	cY.smooth(dt);
	cZ.smooth(dt);
}

bool GY85::aConfigure()
{
	CHECK(ADXL345_Init(aFile, ADXL345_ID, true));

	struct ADXL345_DataFormat confDataFormat = {};
	confDataFormat.range = ADXL345_RANGE_2G;
	CHECK(ADXL345_ConfigureDataFormat(aFile, &confDataFormat));

	struct ADXL345_Power confPowerControl = {};
	confPowerControl.measurement = true;
	CHECK(ADXL345_ConfigurePower(aFile, &confPowerControl));

	return true;
}

bool GY85::cConfigure()
{
	CHECK(HMC5883L_Init(cFile, HMC5883L_ID, true));

	struct HMC5883L conf = {};
	conf.gain = HMC5883L_GAIN_1090;
	conf.measurementMode = HMC5883L_MEASUREMENTMODE_NORMAL;
	conf.outputRate = HMC5883L_OUTPUTRATE_30;
	conf.samples = HMC5883L_SAMPLES_2;
	CHECK(HMC5883L_Configure(cFile, &conf));

	CHECK(HMC5883L_SetContinuousMeasurement(cFile));

	return true;
}


void GY85::getAccelerometer(glm::vec3 &vec) const
{
	vec[0] = aX.normalized;
	vec[1] = aY.normalized;
	vec[2] = aZ.normalized;
}

void GY85::getCompass(glm::vec3 &vec) const
{
	vec[0] = cX.normalized;
	vec[1] = cY.normalized;
	vec[2] = cZ.normalized;
}
