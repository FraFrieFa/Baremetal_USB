with Registers; use Registers;
with Types; use Types;

package body port_samd51g19a is

   procedure setup_gpio is
   begin
      PA_PMUX12   := 16#77#;
      PA_PINCFG24 := 1;
      PA_PINCFG25 := 1;
   end setup_gpio;

   procedure setup_clock is
   begin
      OSCCTRL_DFLLCTRLB := 0;
      OSCCTRL_DFLLCTRLA := 16#82#;
      GCLK_GENCTRL0 := 16#106#;

      -- Set USB channel to GENCTRL0
      GCLK_PCHCTRL10 := 16#40#;
   end setup_clock;

   procedure setup_usb is
   begin
      -- Enable USB in MCLK
      MCLK_AHBMASK := MCLK_AHBMASK or 16#400#;
      MCLK_APBBMASK := MCLK_APBBMASK or 1;
      -- Enable USB in Device Mode
      USB_CTRLA := 2;
   end setup_usb;

   procedure setup_interrupts is
   begin
      NVIC_ISER2 := NVIC_ISER2 or 16#4#;
      USB_INTENSET := USB_INTENSET or 8;
   end setup_interrupts;

   procedure usb_attach is
   begin
      USB_CTRLB := USB_CTRLB and not 1;
   end usb_attach;

end port_samd51g19a;
