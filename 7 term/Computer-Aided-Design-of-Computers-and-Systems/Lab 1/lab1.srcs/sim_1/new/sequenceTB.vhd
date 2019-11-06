----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 09/18/2019 02:25:32 PM
-- Design Name: 
-- Module Name: sequenceTB - Behavioral
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


library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;
use ieee.std_logic_textio.all;
use ieee.std_logic_arith.all;
library std;
use std.textio.all; 

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity sequenceTB is
--  Port ( );
end sequenceTB;

architecture Behavioral of sequenceTB is  
  component sequence
    port(
      EI: in std_logic;
      inputVec: in std_logic_vector(0 to 7);
      EO, GS: out std_logic;
      A: out std_logic_vector(0 to 2)
    );
  end component;

  signal EI: std_logic;
  signal inputVec: std_logic_vector(0 to 7);
  signal EO, GS: std_logic;
  signal A: std_logic_vector(2 downto 0);

  begin
  
  mapping: sequence port map (
    EI => EI,
    inputVec => inputVec,
    EO => EO,
    GS => GS,
    A => A
  );

  test_case: process
  begin
    for i in std_logic range '0' to '1' loop
        for j in 0 to 255 loop
          EI <= i;
          inputVec <= std_logic_vector(conv_unsigned(j, 8));
          wait for 10 ns;
        end loop;
    end loop;
  end process;

end Behavioral;

