with Machine_Code; use Machine_Code;

package body Util is
   procedure Wait (cycles : u32) is
   begin
      for J in 1 .. cycles loop
         Asm ("NOP", Volatile => True);
      end loop;
   end Wait;
end Util;
