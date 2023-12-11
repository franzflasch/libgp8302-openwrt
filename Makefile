include $(TOPDIR)/rules.mk

PKG_NAME:=libgp8302
PKG_VERSION:=1.0
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

CMAKE_INSTALL:=1

define Package/libgp8302
  SECTION:=base
  CATEGORY:=Libraries
  TITLE:=libgp8302 library
  DEPENDS:=+libi2c-user
endef

define Package/libgp8302/description
	libgp8302 library
endef

define Package/libgp8302/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/libgp8302.so $(1)/usr/lib
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/libgp8302
	$(CP) $(PKG_BUILD_DIR)/*.h $(1)/usr/include/libgp8302/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/libgp8302.so* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,libgp8302))