----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 10/01/2019 05:09:38 PM
-- Design Name: 
-- Module Name: main_test - Behavioral
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
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use IEEE.STD_LOGIC_TEXTIO.ALL;
use STD.TEXTIO.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity main_test is
--  Port ( );
end main_test;

architecture Behavioral of main_test is
    component main
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
    end component;
    
    signal SER, SRCLK, RCLK: std_logic := '0';
    signal NOT_SRCLR, NOT_RCLR: std_logic := '1';
    signal QA, QB, QC, QD, QE, QF, QG, QH, QHl: std_logic := '0';
    
    file output : text open read_mode is "output.txt";
    file test_input : text open read_mode is "input.txt";
    
    constant clk_period : time := 16 ns;
    constant passed : string(1 to 6) := "passed";
    constant error : string(1 to 11) := "TEST FAILED";
    
begin
    main_component: main port map (
        NOT_RCLR => NOT_RCLR,
        RCLK => RCLK,
        NOT_SRCLR => NOT_SRCLR,
        SRCLK => SRCLK,
        SER => SER,
        QA => QA,
        QB => QB,
        QC => QC,
        QD => QD,
        QE => QE,
        QF => QF,
        QG => QG,
        QH => QH,
        QHl => QHl
    );

    test_process: process
        variable curr_line, output_line, line_to_write : line;
        variable vector_value : std_logic_vector(4 downto 0);
        variable output_value : std_logic_vector(8 downto 0);
     
    begin
        while not endfile(test_input) loop
            --read input variable
            readline(test_input, curr_line);
            read(curr_line, vector_value);
            
            --read output variable
            readline(output, output_line);
            read(output_line, output_value);
            
            NOT_RCLR <= vector_value(4);
            NOT_SRCLR <= vector_value(3);
            SER <= vector_value(0);
            SRCLK<= vector_value(1);
            wait for 2 ns;
            RCLK <= vector_value(2);
            
            if (
                QA = output_value(8) and
                QB = output_value(7) and
                QC = output_value(6) and
                QD = output_value(5) and
                QE = output_value(4) and
                QF = output_value(3) and
                QG = output_value(2) and
                QH = output_value(1) and
                QHl = output_value(0)
            ) then
                report passed;
            else
                report error severity failure;
            end if;
            
            wait for 2 ns;
        end loop; 
        
        wait for 1000 ns;
    end process;

end Behavioral;
