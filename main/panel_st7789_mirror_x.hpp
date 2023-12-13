#ifndef LGFX_PANEL_ST7789_MIRROR_X_HPP_
#define LGFX_PANEL_ST7789_MIRROR_X_HPP_

#include <LovyanGFX.hpp>
/// @brief ST7789 mirrored for the prism
struct Panel_ST7789_Mirror_X  : public lgfx::Panel_ST7789
{
    std::uint8_t getMadCtl(std::uint8_t r) const override {
      static constexpr std::uint8_t madctl_table[] = {
                                         0,
        MAD_MV|MAD_MX|MAD_MH              ,
               MAD_MX|MAD_MH|MAD_MY|MAD_ML,
        // MAD_MY | MAD_MX added here for proper prism rotation
        MAD_MV|       MAD_MY|MAD_MX|MAD_ML,
                             MAD_MY|MAD_ML,
        MAD_MV                            ,
               MAD_MX|MAD_MH              ,
        MAD_MV|MAD_MX|MAD_MY|MAD_MH|MAD_ML,
      };
      return madctl_table[r];
    }
};

#endif 