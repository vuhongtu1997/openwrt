include $(TOPDIR)/rules.mk

PKG_NAME:=CONFIG_MQTT_PASSWD
PKG_VERSION:=1.0.0
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/CONFIG_MQTT_PASSWD
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=CONFIG_MQTT_PASSWD
	DEPENDS:= +libstdcpp +libopenssl
endef

define Package/CONFIG_MQTT_PASSWD/description
 If you can't figure out what this program does,
 you're probably brain-dead and need immediate
 medical attention.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/CONFIG_MQTT_PASSWD/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/CONFIG_MQTT_PASSWD $(1)/bin/
endef

$(eval $(call BuildPackage,CONFIG_MQTT_PASSWD))

