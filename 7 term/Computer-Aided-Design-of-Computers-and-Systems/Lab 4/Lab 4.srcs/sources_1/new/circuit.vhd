----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 01.11.2019 00:24:51
-- Design Name: 
-- Module Name: schema - Behavioral
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

entity circuit is
    Port ( 
        ledsmain: out std_logic_vector(3 downto 0);
        ledsboard: out std_logic_vector(3 downto 0);
        pushbuttons: in std_logic_vector(4 downto 0);
        dipswitch: in std_logic_vector(3 downto 0);
        --clock diff pair
        sysclk_p: in std_logic;
        sysclk_n: in std_logic
    );
end circuit;

architecture Behavioral of circuit is
    component ShiftRegisterWithBuffer is
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
  
    component ibufds
        port (
            i, ib : in std_logic; 
            o : out std_logic
        );
    end component;
    
    component DIVIDER is
        port ( 
            CLK_IN : in STD_LOGIC;
            CLK_OUT : out STD_LOGIC
        );
    end component;

    signal CLOCK, NOT_CLOCK, innerQH, innerQHl : std_logic;
    signal CLK_NO_DIV : std_logic;
begin
    NOT_CLOCK <= not(CLOCK);

    ShiftRegisterWithBuffer1: ShiftRegisterWithBuffer port map(
        NOT_RCLR => not(pushbuttons(0)),
        RCLK => NOT_CLOCK,
        NOT_SRCLR => not(pushbuttons(1)),
        SRCLK => CLOCK,
        SER => dipswitch(0),
        QA => ledsmain(0),
        QB => ledsmain(1),
        QC => ledsmain(2),
        QD => ledsmain(3),
        QE => ledsboard(0),
        QF => ledsboard(1),
        QG => ledsboard(2),
        QH => innerQH,
        QHl => innerQHl
    );
    
    ledsboard(3) <= ((innerQH and not(dipswitch(0))) or (innerQHl and dipswitch(0)));
    
    buffds: ibufds port map (
        i => sysclk_p, 
        ib => sysclk_n, 
        o => CLK_NO_DIV
    );
    
    div: DIVIDER port map (
        CLK_IN => CLK_NO_DIV, 
        CLK_OUT => CLOCK
    );
    
end Behavioral;
