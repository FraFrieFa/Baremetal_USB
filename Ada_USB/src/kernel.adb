with Registers; use Registers;
with Util;      use Util;
with USB;       use USB;

procedure Kernel is

begin

   PA_PORT := 16#40_0000#;

   Usb_Init;

   for I in 1 .. 10 loop
      Wait (1_000_000);
      PA_OUT := 16#40_0000#;
      Wait (1_000_000);
      PA_OUT := 0;
   end loop;

   Reset_To_Bootloader;

end Kernel;
