
MAVLINK_DIR = $(PWD)/lib/mavlink_generated/include/mavlink/v2.0 
SRC_DIR = $(PWD)/src
INC_DIR = -I$(PWD)/inc

ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

ifdef $(DESTDIR)
	$(DESTDIR) := $(DESTDIR)/
endif

SYSTEM_INCLUDE = $(PREFIX)/include
LDFLAGS = -L$(PREFIX)/lib -llifepo4wered -lboost_filesystem -lboost_regex -lboost_system -lboost_program_options -lstdc++fs


openhd_microservice: bcm2835.o ina2xx.o microservice.o gpio.o camera.o power.o status.o sensor.o main.o 
	g++ -g -pthread -o openhd_microservice bcm2835.o ina2xx.o microservice.o gpio.o camera.o power.o status.o sensor.o main.o $(LDFLAGS)

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
