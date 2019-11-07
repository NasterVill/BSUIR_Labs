----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 10/01/2019 11:36:00 AM
-- Design Name: 
-- Module Name: main - Behavioral
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

entity main is
    port (
        NOT_RCLR: in std_logic;
        RCLK: in std_logic;
        NOT_SRCLR: in std_logic;
        SRCLK: in std_logic;
        SER: in std_logic;
        QA: out std_logic;
        QB: out std_logic;
        QC: out std_logic;
        QD: out std_logic;
        QE: out std_logic;
        QF: out std_logic;
        QG: out std_logic;
        QH: out std_logic;
        QHl: out std_logic
    );
end main;

architecture Behavioral of main is
    component d_flip_flop
        port(
            NOT_R: in std_logic;
            NOT_S: in std_logic;
            D: in std_logic;
            C: in std_logic;
            Q: out std_logic;
            NOT_Q: out std_logic
        );
    end component;
    
    signal VCC: std_logic := '1';
    signal first_line_output, second_line_output: std_logic_vector(7 downto 0);
   
    begin
        first_flip_flop: d_flip_flop port map (
            NOT_R => NOT_SRCLR,
            NOT_S => VCC,
            D => SER,
            C => SRCLK,
            Q => first_line_output(0)
        );
        
        first_line: for i in 0 to 6 generate
            flip_flop: d_flip_flop port map (
                NOT_R => NOT_SRCLR,
                NOT_S => VCC,
                D => first_line_output(i),
                C => SRCLK,
                Q => first_line_output(i + 1)
            );
        end generate first_line;
        
        second_line: for i in 0 to 7 generate
            flip_flop: d_flip_flop port map (
                NOT_R => NOT_RCLR,
                NOT_S => VCC,
                D => first_line_output(i),
                C => RCLK,
                Q => second_line_output(i)
            );
        end generate second_line;
                     
        QA <= second_line_output(0);
        QB <= second_line_output(1);
        QC <= second_line_output(2);
        QD <= second_line_output(3);
        QE <= second_line_output(4);
        QF <= second_line_output(5);
        QG <= second_line_output(6);
        QH <= second_line_output(7);
        QHl <= first_line_output(7);

end Behavioral;