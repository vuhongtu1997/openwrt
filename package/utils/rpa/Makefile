include $(TOPDIR)/rules.mk

PKG_NAME:=rpa
PKG_VERSION:=2.0.0
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)
export DEFINES = -D__OPENWRT__=1

include $(INCLUDE_DIR)/package.mk

define Package/rpa
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=rpa -- Rang Dong smarthome app
	DEPENDS:= +libuci +libmosquittopp +lsqlite3 +libcurl
endef

define Package/rpa/description
 If you can't figure out what this program does,
 you're probably brain-dead and need immediate
 medical attention.
endef

define Build/Compile
    $(MAKE) -C $(PKG_BUILD_DIR) \
        CXX="$(TARGET_CXX)" \
        CXXFLAGS="$(TARGET_CXXFLAGS)" \
        LDFLAGS="$(TARGET_LDFLAGS)" \
        rpa
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/rpa/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/rpa $(1)/bin/
endef

$(eval $(call BuildPackage,rpa))

