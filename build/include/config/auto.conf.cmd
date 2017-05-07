deps_config := \
	/home/gombe/bin/esp/esp-idf/components/aws_iot/Kconfig \
	/home/gombe/bin/esp/esp-idf/components/bt/Kconfig \
	/home/gombe/bin/esp/esp-idf/components/esp32/Kconfig \
	/home/gombe/bin/esp/esp-idf/components/ethernet/Kconfig \
	/home/gombe/bin/esp/esp-idf/components/fatfs/Kconfig \
	/home/gombe/bin/esp/esp-idf/components/freertos/Kconfig \
	/home/gombe/bin/esp/esp-idf/components/log/Kconfig \
	/home/gombe/bin/esp/esp-idf/components/lwip/Kconfig \
	/home/gombe/bin/esp/esp-idf/components/mbedtls/Kconfig \
	/home/gombe/bin/esp/esp-idf/components/openssl/Kconfig \
	/home/gombe/bin/esp/esp-idf/components/spi_flash/Kconfig \
	/home/gombe/bin/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/gombe/bin/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/gombe/bin/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/gombe/bin/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
