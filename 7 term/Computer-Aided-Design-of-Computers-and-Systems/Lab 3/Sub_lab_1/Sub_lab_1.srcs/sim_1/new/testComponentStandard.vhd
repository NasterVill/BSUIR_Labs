library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use IEEE.STD_LOGIC_TEXTIO.ALL;
use STD.TEXTIO.ALL;

entity testComponentStandard is
end testComponentStandard;

architecture Behavioral of testComponentStandard is
    component parallelThree
        port(
          EI: in std_logic;
          inputVec: in std_logic_vector(0 to 7);
          EO, GS: out std_logic;
          A: out std_logic_vector(2 downto 0)
        );
    end component;

    component sequence
        port(
          EI: in std_logic;
          inputVec: in std_logic_vector(0 to 7);
          EO, GS: out std_logic;
          A: out std_logic_vector(2 downto 0)
        );
    end component;
    
    signal EI: std_logic;
    signal inputVec: std_logic_vector(0 to 7);
    signal EO_p, GS_p: std_logic;
    signal A_p: std_logic_vector(2 downto 0);
    signal EO_s, GS_s: std_logic;
    signal A_s: std_logic_vector(2 downto 0);
    signal A0_p: std_logic;
    signal A1_p: std_logic;
    signal A2_p: std_logic;
    signal A0_s: std_logic;
    signal A1_s: std_logic;
    signal A2_s: std_logic;
    signal I0: std_logic;
    signal I1: std_logic;
    signal I2: std_logic;
    signal I3: std_logic;
    signal I4: std_logic;
    signal I5: std_logic;
    signal I6: std_logic;
    signal I7: std_logic;


    file test_input : text open read_mode is "input.txt";
    
    constant passed : string(1 to 6) := "passed";
    constant error : string(1 to 11) := "TEST FAILED";
    
    begin
        instance_p: parallelThree port map(
            EI => EI,
            inputVec => inputVec,
            EO => EO_p,
            GS => GS_p,
            A => A_p
        );
        instance_s: sequence port map(
            EI => EI,
            inputVec => inputVec,
            EO => EO_s,
            GS => GS_s,
            A => A_s
        );
        
        A0_p <= A_p(0);
        A1_p <= A_p(1);
        A2_p <= A_p(2);
        
        A0_s <= A_s(0);
        A1_s <= A_s(1);
        A2_s <= A_s(2);

        I0 <= inputVec(0);
        I1 <= inputVec(1);
        I2 <= inputVec(2);
        I3 <= inputVec(3);
        I4 <= inputVec(4);
        I5 <= inputVec(5);
        I6 <= inputVec(6);
        I7 <= inputVec(7);
    
    process
        variable curr_line : line;
        variable vector_value : std_logic_vector(8 downto 0);
                  
        begin
        
        while not endfile(test_input) loop
            --read input variable
            readline(test_input, curr_line);
            read(curr_line, vector_value);
            
            for i in 0 to 7 loop
                inputVec(i) <= vector_value(i);
            end loop;
            
            EI <= vector_value(8);
            
            wait for 1 ns;
            
            if (A_p = A_s) and (EO_p = EO_s) and (GS_p = GS_s) then
                report passed;
           else
               report error severity failure;
            end if;
        end loop; 
        wait;    
    end process;  
end Behavioral;
