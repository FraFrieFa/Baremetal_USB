with port_SAMD51G19A; use port_SAMD51G19A;

package body USB is

   procedure Usb_Init is
   begin
      Setup_Gpio;
      Setup_Clock;
      Setup_Usb;
      Setup_Interrupts;
      Usb_Attach;
   end Usb_Init;

end USB;
