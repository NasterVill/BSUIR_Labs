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
    
    constant clk_period : time := 16 ns;
    
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

    clk_process: process
    begin       
        SRCLK <= '0';
        wait for clk_period / 2;
        SRCLK <= '1';
        wait for clk_period / 2;
    end process;

    rclk_process: process
    begin    
        wait for 2 ns;   
        RCLK <= '0';
        wait for clk_period / 2;
        RCLK <= '1';
        wait for clk_period / 2;
    end process;

    
    test_process: process
    begin
        SER <= '1';
        wait for 200 ns;
        
        NOT_SRCLR <= '0';
        wait for 20 ns;
        NOT_SRCLR <= '1';
        wait for 200 ns;
        
        NOT_RCLR <= '0';
        wait for 20 ns;
        NOT_RCLR <= '1';
        wait for 200 ns;
       
        for i in 7 downto 0 loop 
            SER <= '0';
            wait for 20 ns;
            SER <= '1';
            wait for 20 ns;
        end loop;

    end process;

end Behavioral;
