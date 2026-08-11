include $(TOPDIR)/rules.mk

PKG_NAME:=smh
PKG_VERSION:=2.0.0
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)
export DEFINES = -D__OPENWRT__=1

include $(INCLUDE_DIR)/package.mk

define Package/smh
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=smh -- Rang Dong smarthome app
	DEPENDS:= +libuci +libmosquittopp +lsqlite3 +libcurl
endef

define Package/smh/description
 If you can't figure out what this program does,
 you're probably brain-dead and need immediate
 medical attention.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/smh/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/smh $(1)/bin/
	$(CP) ./files/smh.sqlite $(1)/
	$(CP) ./files/etc $(1)/
endef

$(eval $(call BuildPackage,smh))

