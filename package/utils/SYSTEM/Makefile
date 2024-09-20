include $(TOPDIR)/rules.mk

PKG_NAME:=SYSTEM
PKG_VERSION:=1.2.38
PKG_RELEASE:=2

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/SYSTEM
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=SYSTEM
	DEPENDS:= +libcurl +libuci +libmosquitto-ssl +libmosquitto +libjson-c +libstdcpp +libsqlite3
endef

define Package/SYSTEM/description
 If you can't figure out what this program does,
 you're probably brain-dead and need immediate
 medical attention.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/SYSTEM/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/SYSTEM $(1)/bin/
endef

$(eval $(call BuildPackage,SYSTEM))

