include $(TOPDIR)/rules.mk
PKG_NAME:=download_package
PKG_VERSION:=1.0.0
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/download_package
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=download_package ------------ HC_RD_vuhongtu
	DEPENDS:= +libjson-c +libstdcpp
endef

define Package/download_package/description
 If you can't figure out what this program does,
 you're probably brain-dead and need immediate
 medical attention.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/download_package/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/download_package $(1)/bin/
endef

$(eval $(call BuildPackage,download_package))
