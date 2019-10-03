----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 10/01/2019 11:36:00 AM
-- Design Name: 
-- Module Name: jk_flip_flop - Behavioral
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

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity jk_flip_flop is
    Port (
        NOT_R: in std_logic;
        NOT_S: in std_logic;
        J: in std_logic;
        K: in std_logic;
        C: in std_logic;
        Q: out std_logic;
        NOT_Q: out std_logic
    );
end jk_flip_flop;

architecture Behavioral of jk_flip_flop is
   signal temp: std_logic := '0';
   
begin
    process (C, NOT_R, NOT_S)
    begin
        if NOT_R='0' then   
            temp <= '0';
        elsif NOT_S='0' then   
            temp <= '1';
        elsif rising_edge(C) then                 
            if (J='0' and K='0') then
                temp <= temp;
            elsif (J='0' and K='1') then
                temp <= '0';
            elsif (J='1' and K='0') then
                temp <= '1';
            elsif (J='1' and K='1') then
                temp <= not (temp);
            end if;
        end if;
    end process;
    
    Q <= temp;
    NOT_Q <= not(temp);
end Behavioral;
