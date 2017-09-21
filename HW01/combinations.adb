-- Combination of numbers;
-- Similar as C#, C++ code;
-- To compile this code, simply using: gcc -c combinations.adb
-- To bind the program: gnatbind combinations; gnatlink combinations; gnatmake combinations.adb
-- To run the code, simply run: gnatmake ./combinations k n;

with GNAT.Command_Line; use GNAT.Command_Line;
with Gnat.Io; use Gnat.Io;
procedure Combinations is

   Max: Integer := 100;
   type Node;                          
   type Node_Ptr is access Node;

   type Node(L: Integer) is record
      Data: String(1..L);
      Next: Node_Ptr;
   end record;
                 
   Scan_Ptr: Node_Ptr;

   Itr: Integer;
   Len: Integer;

   K_value: Integer;
   N_value: Integer;

   -- This combination function is a recursive function to recursively call 
   -- itself, and then examine the returned list, several "if" statement is
   -- listed below:

   -- if:     the return list is empty, return a empty list, or if the 
   --      combination number is larger the range of (start, end);
   -- elseif: the combination number is "1", just return all the number into
   --     string as a list;
   -- elseif: the combination just equals to the range between the "start" 
   --     and "end", then just simply return all the number in a single
   --     string; 
   -- else:   add the additional number (if fits the critera) on each list;
   
   function Combination_Three(Num, S, E: Integer) return Node_ptr is
   Tmp_List_Head: Node_Ptr;
   List_Head: Node_Ptr;
   Tmp_Node: Node_Ptr;
   Front: Integer;
   Back: Integer;
   Current: Integer;
   Str: String(1..100);

   Begin
      if Num > (E - S + 1) then
         return null;
      elsif Num = 1 then
         Current := E;
         Front := 1;
         Back := Integer'Image(Current)'Length;
         Str(Front..back) := Integer'Image(Current);
         Tmp_List_Head := new Node'(Back, Str(Front..Back), null);
         
         while Current > S loop
            Current := Current - 1;
            Back := Integer'Image(Current)'Length;
            Str(Front..Back) := Integer'Image(Current);
            Tmp_Node := new Node'(Back, Str(Front..Back), Tmp_List_Head);
            Tmp_List_Head := Tmp_Node;
         end loop;

         return Tmp_List_Head;
      elsif Num = (E - S + 1) then
         Current := S;
         Front := 1;
         Back := Integer'Image(Current)'Length;
         
         Str(Front..back) := Integer'Image(Current);
         
         while Current < E loop
            Current := Current + 1;
            Front := Back + 1;
            Back := Front + Integer'Image(Current)'Length - 1;

            Str(Front..Back) := Integer'Image(Current);
         end loop;
         
         Tmp_List_Head := new Node'(Back, Str(1..Back), null);
         return Tmp_List_Head;
      else
         List_Head := null;
         Itr := S + 1;
         while Itr <= E loop
            Scan_Ptr :=  Combination_Three(Num - 1, Itr, E);
            loop
               exit when Scan_Ptr = null;
               Len := Integer'Image(Itr - 1)'Length;
               Str(1..Len) := Integer'Image(Itr -1);
               Front := 1 + Len;
               Back := Len + Scan_Ptr.Data'Length;
               Str(Front..Back) := Scan_Ptr.Data;
               Tmp_Node := new Node'(Back, Str(1..Back), List_Head);
               List_Head := Tmp_Node;

               Scan_Ptr := Scan_Ptr.Next;
            end loop;
            Itr := Itr + 1;
         end loop;
         return List_Head;
      end if;
   end;

begin

   K_value := Integer'Value(Get_Argument);
   N_value := Integer'Value(Get_Argument);

   -- make a list to store the combination in string;
   Scan_Ptr := Combination_Three(K_value, 1, N_value);
   -- print all the combination into concole;
   loop
      exit when Scan_Ptr = null;
      Put(Scan_Ptr.Data);
      Put(" ");
      Scan_Ptr := Scan_Ptr.Next;
      New_Line;
   end loop;


end Combinations;
