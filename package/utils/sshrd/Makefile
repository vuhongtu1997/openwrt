include $(TOPDIR)/rules.mk

PKG_NAME:=sshrd
PKG_VERSION:=1.0.1
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

export DEFINES = -D__OPENWRT__=1
# export DEFINES = -DUSE_UCI=1

define Package/sshrd
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=sshrd -- Rang Dong ssh 
	DEPENDS:= +libuci +libmosquitto-ssl +libmosquittopp
endef

define Package/sshrd/description
 If you can't figure out what this program does,
 you're probably brain-dead and need immediate
 medical attention.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/sshrd/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/sshrd $(1)/bin/
	$(CP) ./files/etc $(1)/
endef

$(eval $(call BuildPackage,sshrd))

