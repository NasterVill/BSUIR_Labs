----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 10/01/2019 11:36:00 AM
-- Design Name: 
-- Module Name: d_flip_flop - Behavioral
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

entity d_flip_flop is
    Port ( 
        NOT_R: in std_logic;
        NOT_S: in std_logic;
        D: in std_logic;
        C: in std_logic;
        Q: out std_logic;
        NOT_Q: out std_logic
    );
end d_flip_flop;

architecture Behavioral of d_flip_flop is
    component jk_flip_flop
        port(
           NOT_R: in std_logic;
           NOT_S: in std_logic;
           J: in std_logic;
           K: in std_logic;
           C: in std_logic;
           Q: out std_logic;
           NOT_Q: out std_logic
        );
    end component;
    
    signal jk_NOT_R, jk_NOT_S, jk_J, jk_K , jk_C, jk_Q, jk_NOT_Q: std_logic;
    
    begin   
        inner_flip_flop: jk_flip_flop port map (
            NOT_R => jk_NOT_R,
            NOT_S => jk_NOT_S,
            J => jk_J,
            K => jk_K,
            C => jk_C,
            Q => jk_Q,
            NOT_Q => jk_NOT_Q
        );
     
        jk_NOT_R <= NOT_R;
        jk_NOT_S <= NOT_S;
     
        jk_J <= D;
        jk_K <= not(D);
        
        jk_C <= C;
        
        Q <= jk_Q;
        NOT_Q <= jk_NOT_Q;                  
end Behavioral;
