----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 09/07/2019 02:30:01 PM
-- Design Name: 
-- Module Name: sequence - Sequence
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use ieee.std_logic_unsigned.all;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity sequence is
    Port ( 
        EI: in std_logic;
        inputVec: in std_logic_vector(0 to 7);
        EO, GS: out std_logic;
        A: out std_logic_vector(0 to 2)
    );
end sequence;

architecture Sequence of sequence is

  begin
    process (inputVec, EI) is
    begin
    
      if (EI = '1') then
        A <= "111";

        GS <= '1';
        EO <= '1';
      else 
        if (inputVec(7) = '0') then
          A <= "000";
        elsif (inputVec(6) = '0') then
          A <= "001";
        elsif (inputVec(5) = '0') then
          A <= "010";
        elsif (inputVec(4) = '0') then
          A <= "011";  
        elsif (inputVec(3) = '0') then
          A <= "100";  
        elsif (inputVec(2) = '0') then
          A <= "101";  
        elsif (inputVec(1) = '0') then
          A <= "110";  
        elsif (inputVec(0) = '0') then
          A <= "111"; 
        else
          A <= "111";
        end if;

        if (inputVec = "11111111") then
          GS <= '1';
          EO <= '0';
        else
          GS <= '0';
          EO <= '1';
        end if;
      end if;
    
    end process;
    
end Sequence;
