----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 10/01/2019 12:00:47 PM
-- Design Name: 
-- Module Name: d_flip_flop_test - Behavioral
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

entity d_flip_flop_test is
--  Port ( );
end d_flip_flop_test;

architecture Behavioral of d_flip_flop_test is
    component d_flip_flop
    port(
         D : in  std_logic;
         C : in  std_logic;
         R : in  std_logic;
         S : in  std_logic;
         Q : out  std_logic;
         NOT_Q: out std_logic
        ); 
    END COMPONENT;
    
    signal D : std_logic := '0';
    signal C : std_logic := '0';
    signal R : std_logic := '0';
    signal S : std_logic := '0';
    signal Q : std_logic;
    signal NOT_Q : std_logic;
    
    constant clk_period : time := 20 ns;

    begin
    ddf: d_flip_flop port map (
        D => D,
        C => C,
        R => R,
        S => S,
        Q => Q,
        NOT_Q => NOT_Q
    );

    clk_process: process
    begin       
        C <= '0';
        wait for clk_period / 2;
        C <= '1';
        wait for clk_period / 2;
    end process;
    
    test_process_async: process
    begin
        S <= '0';
        R <= '1';
        wait for 600 ns;
        
        R <= '0';
        S <= '1';
        wait for 600 ns;
        
        R <= '0';
        S <= '0';
        wait for 600 ns;
    end process;
    
    test_process_d: process
        variable temp : std_logic := '0';
    begin
        wait for 100 ns;
        
        for i in 0 to 10 loop
            D <= temp;
            temp := not(temp);
            
             wait for 50 ns;
        end loop;
    end process;

end Behavioral;
