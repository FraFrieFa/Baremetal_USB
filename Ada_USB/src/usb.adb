with port_SAMD51G19A; use port_SAMD51G19A;

package body USB is

   procedure usb_init is
   begin
      setup_gpio;
      setup_clock;
      setup_usb;
      setup_interrupts;
      usb_attach;
   end usb_init;

end USB;
