with Registers; use Registers;
with Types;     use Types;

package body port_samd51g19a is

   procedure Setup_Gpio is
   begin
      PA_PMUX12   := 16#77#;
      PA_PINCFG24 := 1;
      PA_PINCFG25 := 1;
   end Setup_Gpio;

   procedure Setup_Clock is
   begin
      OSCCTRL_DFLLCTRLB := 0;
      OSCCTRL_DFLLCTRLA := 16#82#;
      GCLK_GENCTRL0     := 16#106#;

      -- Set USB channel to GENCTRL0
      GCLK_PCHCTRL10 := 16#40#;
   end Setup_Clock;

   procedure Setup_Usb is
   begin
      -- Enable USB in MCLK
      MCLK_AHBMASK  := MCLK_AHBMASK or 16#400#;
      MCLK_APBBMASK := MCLK_APBBMASK or 1;
      -- Enable USB in Device Mode
      USB_CTRLA     := 2;
   end Setup_Usb;

   procedure Setup_Interrupts is
   begin
      NVIC_ISER2   := NVIC_ISER2 or 16#4#;
      USB_INTENSET := USB_INTENSET or 8;
   end Setup_Interrupts;

   procedure Usb_Attach is
   begin
      USB_CTRLB := USB_CTRLB and not 1;
   end Usb_Attach;

end port_samd51g19a;
