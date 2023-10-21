with Machine_Code; use Machine_Code;
with Registers;    use Registers;

package body Util is
   procedure Wait (cycles : u32) is
   begin
      for J in 1 .. cycles loop
         Asm ("NOP", Volatile => True);
      end loop;
   end Wait;

   procedure Reset_To_Bootloader is
   begin
      RESET_ADDR := 16#f016_69ef#;
      AIRCR      := 16#5fa_0004# or (AIRCR and 16#700#);
   end Reset_To_Bootloader;

end Util;
