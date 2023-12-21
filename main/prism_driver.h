#ifndef PRISM_DRIVER_H
#define PRISM_DRIVER_H

#include <LovyanGFX.hpp>
#include "panel_st7789_mirror_x.h"
#define TFT_WIDTH   240
#define TFT_HEIGHT  320

#define TFT_MOSI    10
#define TFT_MISO    -1
#define TFT_SCLK    8
#define TFT_DC      4 
#define TFT_CS      2
#define TFT_RST     5  
#define TFT_BL      3

class LGFX : public lgfx::LGFX_Device
{
    Panel_ST7789_Mirror_X _panel_instance;
    lgfx::Bus_SPI _bus_instance; 
    lgfx::Light_PWM     _light_instance;

public:
  LGFX(void)
  {
    {
      auto cfg = _bus_instance.config();
      cfg.spi_host = SPI2_HOST; 
      cfg.spi_mode = 0;
      cfg.dma_channel = SPI_DMA_CH_AUTO;
      cfg.freq_write = 40000000;
      cfg.freq_read = 16000000;
      cfg.spi_3wire = true;
      cfg.use_lock = true;
      
      cfg.pin_sclk = TFT_SCLK;
      cfg.pin_mosi = TFT_MOSI; 
      cfg.pin_miso = TFT_MISO;
      cfg.pin_dc = TFT_DC;  

      _bus_instance.config(cfg);             
      _panel_instance.setBus(&_bus_instance);
    }

    {
      auto cfg = _panel_instance.config(); 
      cfg.pin_cs = TFT_CS;   
      cfg.pin_rst = TFT_RST;  
      cfg.pin_busy = -1;     

      cfg.offset_x         =     0;  
      cfg.offset_y         =     0;  
      cfg.dummy_read_pixel =     8;   
      cfg.dummy_read_bits  =     1; 
      cfg.panel_width = TFT_WIDTH;
      cfg.panel_height = TFT_HEIGHT;
      cfg.invert = true;
      cfg.rgb_order = false;
      cfg.dlen_16bit = false;
      cfg.bus_shared = false;
  
      _panel_instance.config(cfg);
    }

    {
      auto cfg = _light_instance.config();      
      cfg.pin_bl = TFT_BL;            
      cfg.invert = false;           
      cfg.freq   = 44100;    
      cfg.pwm_channel = 0;        
      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);  
    }
    setPanel(&_panel_instance);
  }
};

#endif 
