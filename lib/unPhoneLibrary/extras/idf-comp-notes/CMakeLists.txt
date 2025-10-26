# CMakeLists.txt

idf_component_register(
    SRCS unPhone.cpp unPhoneFactoryMode.cpp unPhoneLoRa.cpp unPhoneUI0.cpp
    INCLUDE_DIRS "."
)
#   REQUIRES arduino
#   REQUIRES arduino-esp32

