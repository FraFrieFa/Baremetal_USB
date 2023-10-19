with Types; use Types;

package Registers is
   pragma Preelaborate;

   MCLK_AHBMASK : u32 with Import, Volatile, Link_Name => "OSCCTRL_EVCTRL";
   MCLK_APBBMASK : u32 with Import, Volatile, Link_Name => "OSCCTRL_EVCTRL";

   OSCCTRL_EVCTRL : u8 with Import, Volatile, Link_Name => "OSCCTRL_EVCTRL";
   OSCCTRL_DFLLCTRLA : u8 with Import, Volatile, Link_Name => "OSCCTRL_DFLLCTRLA";
   OSCCTRL_DFLLCTRLB : u8 with Import, Volatile, Link_Name => "OSCCTRL_DFLLCTRLB";
   
   GCLK_CTRLA : u8 with Import, Volatile, Link_Name => "GCLK_CTRLA";
   GCLK_GENCTRL0 : u32 with Import, Volatile, Link_Name => "GCLK_GENCTRL0";
   GCLK_GENCTRL1 : u32 with Import, Volatile, Link_Name => "GCLK_GENCTRL1";
   GCLK_PCHCTRL0 : u32 with Import, Volatile, Link_Name => "GCLK_PCHCTRL0";
   GCLK_PCHCTRL10 : u32 with Import, Volatile, Link_Name => "GCLK_PCHCTRL10";

   USB_CTRLA : u8 with Import, Volatile, Link_Name => "USB_CTRLA";
   USB_CTRLB : u16 with Import, Volatile, Link_Name => "USB_CTRLB";
   USB_DADD : u8 with Import, Volatile, Link_Name => "USB_DADD";

   USB_INTENSET : u8 with Import, Volatile, Link_Name => "USB_INTENSET";

   PA_PORT : u32 with Import, Volatile, Link_Name => "PA_DIR";
   PA_OUT : u32 with Import, Volatile, Link_Name => "PA_OUT";
   PA_PMUX12 : u8 with Import, Volatile, Link_Name => "PA_PMUX12";
   PA_PINCFG24 : u8 with Import, Volatile, Link_Name => "PA_PINCFG24";
   PA_PINCFG25 : u8 with Import, Volatile, Link_Name => "PA_PINCFG25";

   NVIC_ISER2 : u32 with Import, Volatile, Link_Name => "NVIC_ISER2";

end Registers;
