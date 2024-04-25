library ieee;
use IEEE.std_logic_1164.all;
use ieee.numeric_std.all; --This allows the use of the signed variable types. The complete model using this approach is much more compact and is shown below:

entity peak_detect is
	generic(top : natural := 23);
port (
	clk : in std_logic;
	reset : in std_logic;
	ch1_in : in signed(top downto 0);
	ch2_in : in signed(top downto 0);
	ch3_in : in signed(top downto 0);
	ch4_in : in signed(top downto 0);
	ch5_in : in signed(top downto 0);
	ch6_in : in signed(top downto 0);
	ch7_in : in signed(top downto 0);
	ch8_in : in signed(top downto 0);
	ch9_in : in signed(top downto 0);
	ch10_in : in signed(top downto 0);
	ch11_in : in signed(top downto 0);
	ch12_in : in signed(top downto 0);
	ch13_in : in signed(top downto 0);
	ch14_in : in signed(top downto 0);
	ch15_in : in signed(top downto 0);
	ch16_in : in signed(top downto 0);
	ch1_out : out signed(7 downto 0);
	ch2_out : out signed(7 downto 0);
	ch3_out : out signed(7 downto 0);
	ch4_out : out signed(7 downto 0);
	ch5_out : out signed(7 downto 0);
	ch6_out : out signed(7 downto 0);
	ch7_out : out signed(7 downto 0);
	ch8_out : out signed(7 downto 0);
	ch9_out : out signed(7 downto 0);
	ch10_out : out signed(7 downto 0);
	ch11_out : out signed(7 downto 0);
	ch12_out : out signed(7 downto 0);
	ch13_out : out signed(7 downto 0);
	ch14_out : out signed(7 downto 0);
	ch15_out : out signed(7 downto 0);
	ch16_out : out signed(7 downto 0)
	);
end entity peak_detect;

architecture behavior of peak_detect is

	signal p1 : signed(7 downto 0) := (others => '0');
	signal p2 : signed(7 downto 0) := (others => '0');
	signal p3 : signed(7 downto 0) := (others => '0');
	signal p4 : signed(7 downto 0) := (others => '0');
	signal p5 : signed(7 downto 0) := (others => '0');
	signal p6 : signed(7 downto 0) := (others => '0');
	signal p7 : signed(7 downto 0) := (others => '0');
	signal p8 : signed(7 downto 0) := (others => '0');
	signal p9 : signed(7 downto 0) := (others => '0');
	signal p10 : signed(7 downto 0) := (others => '0');
	signal p11 : signed(7 downto 0) := (others => '0');
	signal p12 : signed(7 downto 0) := (others => '0');
	signal p13 : signed(7 downto 0) := (others => '0');
	signal p14 : signed(7 downto 0) := (others => '0');
	signal p15 : signed(7 downto 0) := (others => '0');
	signal p16 : signed(7 downto 0) := (others => '0');
	

begin
	
	peak : process (clk, reset, ch1_in, ch2_in, ch3_in, ch4_in, ch5_in, ch6_in, ch7_in, ch8_in, ch9_in, ch10_in, ch11_in, ch12_in, ch13_in, ch14_in, ch15_in, ch16_in, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16)
	begin
	  if rising_edge(clk) then
	    if (reset = '1') then
			p1 <= (others => '0');
			p2 <= (others => '0');
			p3 <= (others => '0');
			p4 <= (others => '0');
			p5 <= (others => '0');
			p6 <= (others => '0');
			p7 <= (others => '0');
			p8 <= (others => '0');
			p9 <= (others => '0');
			p10 <= (others => '0');
			p11 <= (others => '0');
			p12 <= (others => '0');
			p13 <= (others => '0');
			p14 <= (others => '0');
			p15 <= (others => '0');
			p16 <= (others => '0');
		end if;		

		if (ch1_in(23 downto 16) > p1) then 
			p1 <= ch1_in(23 downto 16);
		end if;	
		if (ch2_in(23 downto 16) > p2) then 
			p2 <= ch2_in(23 downto 16);
		end if;	
		if (ch3_in(23 downto 16) > p3) then 
			p3 <= ch3_in(23 downto 16);
		end if;	
		if (ch4_in(23 downto 16) > p4) then 
			p4 <= ch4_in(23 downto 16);
		end if;	
		if (ch5_in(23 downto 16) > p5) then 
			p5 <= ch5_in(23 downto 16);
		end if;	
		if (ch6_in(23 downto 16) > p6) then 
			p6 <= ch6_in(23 downto 16);
		end if;	
		if (ch7_in(23 downto 16) > p7) then 
			p7 <= ch7_in(23 downto 16);
		end if;	
		if (ch8_in(23 downto 16) > p8) then 
			p8 <= ch8_in(23 downto 16);
		end if;	
		if (ch9_in(23 downto 16) > p9) then 
			p9 <= ch9_in(23 downto 16);
		end if;	
		if (ch10_in(23 downto 16) > p10) then 
			p10 <= ch10_in(23 downto 16);
		end if;	
		if (ch11_in(23 downto 16) > p11) then 
			p11 <= ch11_in(23 downto 16);
		end if;	
		if (ch12_in(23 downto 16) > p12) then 
			p12 <= ch12_in(23 downto 16);
		end if;	
		if (ch13_in(23 downto 16) > p13) then 
			p13 <= ch13_in(23 downto 16);
		end if;	
		if (ch14_in(23 downto 16) > p14) then 
			p14 <= ch14_in(23 downto 16);
		end if;	
		if (ch15_in(23 downto 16) > p15) then 
			p15 <= ch15_in(23 downto 16);
		end if;	
		if (ch16_in(23 downto 16) > p16) then 
			p16 <= ch16_in(23 downto 16);
		end if;	
    end if;
		
	end process peak;

	ch1_out <= p1;
	ch2_out <= p2;
	ch3_out <= p3;
	ch4_out <= p4;
	ch5_out <= p5;
	ch6_out <= p6;
	ch7_out <= p7;
	ch8_out <= p8;
	ch9_out <= p9;
	ch10_out <= p10;
	ch11_out <= p11;
	ch12_out <= p12;
	ch13_out <= p13;
	ch14_out <= p14;
	ch15_out <= p15;
	ch16_out <= p16;

end architecture behavior;
