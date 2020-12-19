
MAVLINK_DIR = $(PWD)/lib/Mavlink/out 
SRC_DIR = $(PWD)/src
INC_DIR = -I$(PWD)/inc -I$(PWD)/lib/RTIMULib2/RTIMULib -I$(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers

ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

ifdef $(DESTDIR)
	$(DESTDIR) := $(DESTDIR)/
endif

SYSTEM_INCLUDE = $(PREFIX)/include
LDFLAGS = -L$(PREFIX)/lib -llifepo4wered -lboost_filesystem -lboost_regex -lboost_system -lboost_program_options -lstdc++fs


openhd_microservice: RTFusion.o RTFusionRTQF.o RTMath.o RTFusionKalman4.o RTIMUAccelCal.o RTIMUHal.o RTIMUMagCal.o RTIMUSettings.o RTIMU.o RTIMUGD20M303DLHC.o RTIMUGD20HM303DLHC.o RTIMUGD20HM303D.o RTIMULSM9DS0.o RTIMULSM9DS1.o RTIMUMPU9150.o RTIMUMPU9250.o RTIMUBMX055.o RTIMUBNO055.o RTIMUNull.o RTPressure.o RTPressureBMP180.o RTPressureLPS25H.o RTPressureMS5611.o RTPressureMS5637.o RTHumidity.o RTHumidityHTS221.o RTHumidityHTU21D.o RTIMUHMC5883LADXL345.o bcm2835.o ina2xx.o microservice.o gpio.o camera.o power.o status.o sensor.o main.o 
	g++ -g -pthread -o openhd_microservice RTFusion.o RTFusionRTQF.o RTMath.o RTFusionKalman4.o RTIMUAccelCal.o RTIMUHal.o RTIMUMagCal.o RTIMUSettings.o RTIMU.o RTIMUGD20M303DLHC.o RTIMUGD20HM303DLHC.o RTIMUGD20HM303D.o RTIMULSM9DS0.o RTIMULSM9DS1.o RTIMUMPU9150.o RTIMUMPU9250.o RTIMUBMX055.o RTIMUBNO055.o RTIMUNull.o RTPressure.o RTPressureBMP180.o RTPressureLPS25H.o RTPressureMS5611.o RTPressureMS5637.o RTHumidity.o RTHumidityHTS221.o RTHumidityHTU21D.o RTIMUHMC5883LADXL345.o bcm2835.o ina2xx.o microservice.o gpio.o camera.o power.o status.o sensor.o main.o $(LDFLAGS)

main.o: $(SRC_DIR)/main.cpp
	g++ -std=c++17 -Wno-psabi -g -c -pthread -I$(SYSTEM_INCLUDE) -I$(MAVLINK_DIR) $(INC_DIR) $(SRC_DIR)/main.cpp

microservice.o: $(SRC_DIR)/microservice.cpp
	g++ -std=c++17 -Wno-psabi -g -c -pthread -I$(SYSTEM_INCLUDE) -I$(MAVLINK_DIR) $(INC_DIR) $(SRC_DIR)/microservice.cpp

camera.o: $(SRC_DIR)/camera.cpp
	g++ -std=c++17 -Wno-psabi -g -c -pthread -I$(SYSTEM_INCLUDE) -I$(MAVLINK_DIR) $(INC_DIR) $(SRC_DIR)/camera.cpp

power.o: $(SRC_DIR)/power.cpp
	g++ -std=c++17 -Wno-psabi -g -c -pthread -I$(SYSTEM_INCLUDE) -I$(MAVLINK_DIR) $(INC_DIR) $(SRC_DIR)/power.cpp

gpio.o: $(SRC_DIR)/gpio.cpp
	g++ -std=c++17 -Wno-psabi -g -c -pthread -I$(SYSTEM_INCLUDE) -I$(MAVLINK_DIR) $(INC_DIR) $(SRC_DIR)/gpio.cpp

status.o: $(SRC_DIR)/status.cpp
	g++ -std=c++17 -Wno-psabi -g -c -pthread -I$(SYSTEM_INCLUDE) -I$(MAVLINK_DIR) $(INC_DIR) $(SRC_DIR)/status.cpp

sensor.o: $(SRC_DIR)/sensor.cpp
	g++ -std=c++17 -Wno-psabi -g -c -pthread -I$(SYSTEM_INCLUDE) -I$(MAVLINK_DIR) $(INC_DIR) $(SRC_DIR)/sensor.cpp

ina2xx.o: $(SRC_DIR)/ina2xx.c
	gcc -g -c -pthread -I$(SYSTEM_INCLUDE) -I$(MAVLINK_DIR) $(INC_DIR) $(SRC_DIR)/ina2xx.c

bcm2835.o: $(SRC_DIR)/bcm2835.c
	gcc -g -c -pthread -I$(SYSTEM_INCLUDE) -I$(MAVLINK_DIR) $(INC_DIR) $(SRC_DIR)/bcm2835.c



RTFusion.o: $(PWD)/lib/RTIMULib2/RTIMULib/RTFusion.cpp
	gcc -g -c -pthread -I$(SYSTEM_INCLUDE) $(INC_DIR) $(PWD)/lib/RTIMULib2/RTIMULib/RTFusion.cpp

RTFusionRTQF.o: $(PWD)/lib/RTIMULib2/RTIMULib/RTFusionRTQF.cpp
	gcc -g -c -pthread -I$(SYSTEM_INCLUDE) $(INC_DIR) $(PWD)/lib/RTIMULib2/RTIMULib/RTFusionRTQF.cpp

RTMath.o: $(PWD)/lib/RTIMULib2/RTIMULib/RTMath.cpp
	gcc -g -c -pthread -I$(SYSTEM_INCLUDE) $(INC_DIR) $(PWD)/lib/RTIMULib2/RTIMULib/RTMath.cpp

RTFusionKalman4.o: $(PWD)/lib/RTIMULib2/RTIMULib/RTFusionKalman4.cpp
	gcc -g -c -pthread -I$(SYSTEM_INCLUDE) $(INC_DIR) $(PWD)/lib/RTIMULib2/RTIMULib/RTFusionKalman4.cpp

RTIMUAccelCal.o: $(PWD)/lib/RTIMULib2/RTIMULib/RTIMUAccelCal.cpp
	gcc -g -c -pthread -I$(SYSTEM_INCLUDE) $(INC_DIR) $(PWD)/lib/RTIMULib2/RTIMULib/RTIMUAccelCal.cpp

RTIMUHal.o: $(PWD)/lib/RTIMULib2/RTIMULib/RTIMUHal.cpp
	gcc -g -c -pthread -I$(SYSTEM_INCLUDE) $(INC_DIR) $(PWD)/lib/RTIMULib2/RTIMULib/RTIMUHal.cpp

RTIMUMagCal.o: $(PWD)/lib/RTIMULib2/RTIMULib/RTIMUMagCal.cpp
	gcc -g -c -pthread -I$(SYSTEM_INCLUDE) $(INC_DIR) $(PWD)/lib/RTIMULib2/RTIMULib/RTIMUMagCal.cpp

RTIMUSettings.o: $(PWD)/lib/RTIMULib2/RTIMULib/RTIMUSettings.cpp
	gcc -g -c -pthread -I$(SYSTEM_INCLUDE) $(INC_DIR) $(PWD)/lib/RTIMULib2/RTIMULib/RTIMUSettings.cpp

RTIMU.o: $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTIMU.cpp
	gcc -g -c -pthread -I$(SYSTEM_INCLUDE) $(INC_DIR) $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTIMU.cpp

RTIMUGD20M303DLHC.o: $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTIMUGD20M303DLHC.cpp
	gcc -g -c -pthread -I$(SYSTEM_INCLUDE) $(INC_DIR) $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTIMUGD20M303DLHC.cpp

RTIMUGD20HM303DLHC.o: $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTIMUGD20HM303DLHC.cpp
	gcc -g -c -pthread -I$(SYSTEM_INCLUDE) $(INC_DIR) $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTIMUGD20HM303DLHC.cpp

RTIMUGD20HM303D.o: $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTIMUGD20HM303D.cpp
	gcc -g -c -pthread -I$(SYSTEM_INCLUDE) $(INC_DIR) $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTIMUGD20HM303D.cpp

RTIMULSM9DS0.o: $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTIMULSM9DS0.cpp
	gcc -g -c -pthread -I$(SYSTEM_INCLUDE) $(INC_DIR) $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTIMULSM9DS0.cpp

RTIMULSM9DS1.o: $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTIMULSM9DS1.cpp
	gcc -g -c -pthread -I$(SYSTEM_INCLUDE) $(INC_DIR) $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTIMULSM9DS1.cpp

RTIMUMPU9150.o: $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTIMUMPU9150.cpp
	gcc -g -c -pthread -I$(SYSTEM_INCLUDE) $(INC_DIR) $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTIMUMPU9150.cpp

RTIMUMPU9250.o: $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTIMUMPU9250.cpp
	gcc -g -c -pthread -I$(SYSTEM_INCLUDE) $(INC_DIR) $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTIMUMPU9250.cpp

RTIMUBMX055.o: $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTIMUBMX055.cpp
	gcc -g -c -pthread -I$(SYSTEM_INCLUDE) $(INC_DIR) $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTIMUBMX055.cpp

RTIMUBNO055.o: $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTIMUBNO055.cpp
	gcc -g -c -pthread -I$(SYSTEM_INCLUDE) $(INC_DIR) $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTIMUBNO055.cpp

RTIMUNull.o: $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTIMUNull.cpp
	gcc -g -c -pthread -I$(SYSTEM_INCLUDE) $(INC_DIR) $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTIMUNull.cpp

RTPressure.o: $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTPressure.cpp
	gcc -g -c -pthread -I$(SYSTEM_INCLUDE) $(INC_DIR) $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTPressure.cpp

RTPressureBMP180.o: $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTPressureBMP180.cpp
	gcc -g -c -pthread -I$(SYSTEM_INCLUDE) $(INC_DIR) $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTPressureBMP180.cpp

RTPressureLPS25H.o: $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTPressureLPS25H.cpp
	gcc -g -c -pthread -I$(SYSTEM_INCLUDE) $(INC_DIR) $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTPressureLPS25H.cpp

RTPressureMS5611.o: $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTPressureMS5611.cpp
	gcc -g -c -pthread -I$(SYSTEM_INCLUDE) $(INC_DIR) $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTPressureMS5611.cpp

RTPressureMS5637.o: $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTPressureMS5637.cpp
	gcc -g -c -pthread -I$(SYSTEM_INCLUDE) $(INC_DIR) $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTPressureMS5637.cpp

RTHumidity.o: $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTHumidity.cpp
	gcc -g -c -pthread -I$(SYSTEM_INCLUDE) $(INC_DIR) $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTHumidity.cpp

RTHumidityHTS221.o: $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTHumidityHTS221.cpp
	gcc -g -c -pthread -I$(SYSTEM_INCLUDE) $(INC_DIR) $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTHumidityHTS221.cpp

RTHumidityHTU21D.o: $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTHumidityHTU21D.cpp
	gcc -g -c -pthread -I$(SYSTEM_INCLUDE) $(INC_DIR) $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTHumidityHTU21D.cpp

RTIMUHMC5883LADXL345.o: $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTIMUHMC5883LADXL345.cpp
	gcc -g -c -pthread -I$(SYSTEM_INCLUDE) $(INC_DIR) $(PWD)/lib/RTIMULib2/RTIMULib/IMUDrivers/RTIMUHMC5883LADXL345.cpp


clean:
	rm -f *.o openhd_microservice

.PHONY: install
install: openhd_microservice
	install -d $(DESTDIR)$(PREFIX)/bin/
	install -d $(DESTDIR)/etc/systemd/system
	install -m 755 openhd_microservice $(DESTDIR)$(PREFIX)/bin/
	install -m 644 openhd_microservice@.service $(DESTDIR)/etc/systemd/system/
	install -d $(DESTDIR)/etc/openhd
	install -m 644 openhd_microservice.conf $(DESTDIR)/etc/openhd/

.PHONY: enable
enable: install
	systemctl enable openhd_microservice@power
	systemctl start openhd_microservice@power

	systemctl enable openhd_microservice@gpio
	systemctl start openhd_microservice@gpio

	systemctl enable openhd_microservice@camera
	systemctl start openhd_microservice@camera

.PHONY: uninstall
uninstall:
	rm -f $(PREFIX)/bin/openhd_microservice
