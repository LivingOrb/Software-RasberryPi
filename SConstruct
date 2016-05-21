import os

common = Library('lib/common', Glob('common/*.cpp'),
	CXXFLAGS='-std=c++11',
	CPPPATH='.')

game = Program('bin/game', Glob('game/*.cpp') + [common],
	CXXFLAGS='-std=c++11',
	CPPPATH='.',
	LIBS=['adxl345', 'dl', 'hmc5883l', 'lua', 'ws2811'])

rotationCalibration = Program('bin/rotation-calibration', Glob('rotation-calibration/*.cpp') + [common],
	CXXFLAGS='-std=c++11',
	CPPPATH='.',
	LIBS=['adxl345', 'hmc5883l', 'pthread', 'ws2811'])

Default([game, rotationCalibration])
