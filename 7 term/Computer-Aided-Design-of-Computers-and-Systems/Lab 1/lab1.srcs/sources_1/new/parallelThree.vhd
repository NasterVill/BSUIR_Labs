----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 09/18/2019 12:57:04 PM
-- Design Name: 
-- Module Name: parallelThree - ParallelThree
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

entity parallelThree is
    Port ( 
        EI: in std_logic;
        inputVec: in std_logic_vector(0 to 7);
        EO, GS: out std_logic;
        A: out std_logic_vector(0 to 2)
    );  
end parallelThree;

architecture ParallelThree of parallelThree is
  signal tmpNotAndResult: std_logic;
  
begin
  tmpNotAndResult <= not (
    inputVec(0) and
    inputVec(1) and
    inputVec(2) and
    inputVec(3) and
    inputVec(4) and
    inputVec(5) and
    inputVec(6) and
    inputVec(7) and
    (not EI)
  );

  EO <= tmpNotAndResult;

  -- GS <= (not (not EI)) or (not tmpNotAndResult);
  GS <= not ((not EI) and tmpNotAndResult);

  A(0) <= not (
    ((not inputVec(1)) and inputVec(2) and inputVec(4) and inputVec(6) and (not EI)) or
    ((not inputVec(3)) and inputVec(4) and inputVec(6) and (not EI)) or
    ((not inputVec(5)) and inputVec(6) and (not EI)) or
    ((not inputVec(7)) and (not EI))
  );

  A(1) <= not (
    ((not inputVec(2)) and inputVec(4) and inputVec(5) and (not EI)) or
    ((not inputVec(3)) and inputVec(4) and inputVec(5) and (not EI)) or
    ((not inputVec(6)) and (not EI)) or
    ((not inputVec(7)) and (not EI))
  );

  A(2) <= not (
    ((not inputVec(4)) and (not EI)) or
    ((not inputVec(5)) and (not EI)) or
    ((not inputVec(6)) and (not EI)) or
    ((not inputVec(7)) and (not EI))
  );

end ParallelThree;
