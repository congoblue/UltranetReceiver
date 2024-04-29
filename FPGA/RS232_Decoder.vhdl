-------------------------------------------------------------------------------
-- RS232 Command decoder
-- https://github.com/xn--nding-jua/UltranetReceiver
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use IEEE.NUMERIC_STD.ALL;

entity RS232_Decoder is 
	port
	(
		clk				: in std_logic;
	
		RX_DataReady	: in std_logic;
		RX_Data			: in std_logic_vector(7 downto 0);
		
		ch1_peak : in signed(7 downto 0);
		ch2_peak : in signed(7 downto 0);
		ch3_peak : in signed(7 downto 0);
		ch4_peak : in signed(7 downto 0);
		ch5_peak : in signed(7 downto 0);
		ch6_peak : in signed(7 downto 0);
		ch7_peak : in signed(7 downto 0);
		ch8_peak : in signed(7 downto 0);
		ch9_peak : in signed(7 downto 0);
		ch10_peak : in signed(7 downto 0);
		ch11_peak : in signed(7 downto 0);
		ch12_peak : in signed(7 downto 0);
		ch13_peak : in signed(7 downto 0);
		ch14_peak : in signed(7 downto 0);
		ch15_peak : in signed(7 downto 0);
		ch16_peak : in signed(7 downto 0);

		ultranet_valid	: in std_logic;

		TX_TxBusy	: in std_logic;
		TX_DataReady	: out std_logic;
		TX_Data			: out std_logic_vector(7 downto 0);
		
		main_volume_l	: out std_logic_vector(23 downto 0);
		main_volume_r	: out std_logic_vector(23 downto 0);

		ch1_volume_l	: out std_logic_vector(23 downto 0);
		ch1_volume_r	: out std_logic_vector(23 downto 0);

		ch2_volume_l	: out std_logic_vector(23 downto 0);
		ch2_volume_r	: out std_logic_vector(23 downto 0);

		ch3_volume_l	: out std_logic_vector(23 downto 0);
		ch3_volume_r	: out std_logic_vector(23 downto 0);

		ch4_volume_l	: out std_logic_vector(23 downto 0);
		ch4_volume_r	: out std_logic_vector(23 downto 0);

		ch5_volume_l	: out std_logic_vector(23 downto 0);
		ch5_volume_r	: out std_logic_vector(23 downto 0);

		ch6_volume_l	: out std_logic_vector(23 downto 0);
		ch6_volume_r	: out std_logic_vector(23 downto 0);

		ch7_volume_l	: out std_logic_vector(23 downto 0);
		ch7_volume_r	: out std_logic_vector(23 downto 0);

		ch8_volume_l	: out std_logic_vector(23 downto 0);
		ch8_volume_r	: out std_logic_vector(23 downto 0);

		ch9_volume_l	: out std_logic_vector(23 downto 0);
		ch9_volume_r	: out std_logic_vector(23 downto 0);

		ch10_volume_l	: out std_logic_vector(23 downto 0);
		ch10_volume_r	: out std_logic_vector(23 downto 0);

		ch11_volume_l	: out std_logic_vector(23 downto 0);
		ch11_volume_r	: out std_logic_vector(23 downto 0);

		ch12_volume_l	: out std_logic_vector(23 downto 0);
		ch12_volume_r	: out std_logic_vector(23 downto 0);

		ch13_volume_l	: out std_logic_vector(23 downto 0);
		ch13_volume_r	: out std_logic_vector(23 downto 0);

		ch14_volume_l	: out std_logic_vector(23 downto 0);
		ch14_volume_r	: out std_logic_vector(23 downto 0);

		ch15_volume_l	: out std_logic_vector(23 downto 0);
		ch15_volume_r	: out std_logic_vector(23 downto 0);

		ch16_volume_l	: out std_logic_vector(23 downto 0);
		ch16_volume_r	: out std_logic_vector(23 downto 0);
		
		peak_reset	: out std_logic
		
	);
end entity;

architecture Behavioral of RS232_Decoder is
	signal pos_edge			: std_logic;
	signal zRX_DataReady		: std_logic;
	signal ErrorCheckWord	: unsigned(15 downto 0);
	signal PayloadSum			: unsigned(15 downto 0);
	signal zTX_TxBusy		: std_logic;
	signal tx_ready			: std_logic;
	signal tx_start			: std_logic;
	signal tx_send			: std_logic;

begin
	detect_edge : process(clk)
	begin
		if rising_edge(clk) then
			zRX_DataReady <= RX_DataReady;
			if RX_DataReady = '1' and zRX_DataReady = '0' then
				pos_edge <= '1';
			else
				pos_edge <= '0';
			end if;
			zTX_TxBusy <= TX_TxBusy; --make a flag for tx busy going low
			if TX_TxBusy = '0' and zTX_TxBusy = '1' then
				tx_ready <= '1';
			else
				tx_ready <= '0';
			end if;

		end if;
	end process;

	process (clk)
		variable b1 : std_logic_vector(7 downto 0);	-- 0xAA
		variable b2 : std_logic_vector(7 downto 0);	-- C
		variable b3 : std_logic_vector(7 downto 0);	-- V1 = payload 0..127
		variable tx_count : unsigned(7 downto 0);
		variable tx_pulse : unsigned(7 downto 0);
		
		
	begin
		if (rising_edge(clk)) then
			if pos_edge = '1' then
				-- a byte has been received from the uart
				b1 := b2;
				b2 := b3;
				b3 := RX_Data;
				
				-- State-Machine zum Empfangen von RS232-Daten
				if (unsigned(b1)=16#AA#) then 
						
						if (unsigned(b2)=0) then
							-- Command = 0 (Main L)
							main_volume_l(6 downto 0) <= b3(6 downto 0); --map the 0..127 value to be 0..254
						elsif (unsigned(b2)=1) then
							ch1_volume_l(6 downto 0) <= b3(6 downto 0);
						elsif (unsigned(b2)=2) then
							ch2_volume_l(6 downto 0) <= b3(6 downto 0);
						elsif (unsigned(b2)=3) then
							ch3_volume_l(6 downto 0) <= b3(6 downto 0);
						elsif (unsigned(b2)=4) then
							ch4_volume_l(6 downto 0) <= b3(6 downto 0);
						elsif (unsigned(b2)=5) then
							ch5_volume_l(6 downto 0) <= b3(6 downto 0);
						elsif (unsigned(b2)=6) then
							ch6_volume_l(6 downto 0) <= b3(6 downto 0);
						elsif (unsigned(b2)=7) then
							ch7_volume_l(6 downto 0) <= b3(6 downto 0);
						elsif (unsigned(b2)=8) then
							ch8_volume_l(6 downto 0) <= b3(6 downto 0);
						elsif (unsigned(b2)=9) then
							ch9_volume_l(6 downto 0) <= b3(6 downto 0);
						elsif (unsigned(b2)=10) then
							ch10_volume_l(6 downto 0) <= b3(6 downto 0);
						elsif (unsigned(b2)=11) then
							ch11_volume_l(6 downto 0) <= b3(6 downto 0);
						elsif (unsigned(b2)=12) then
							ch12_volume_l(6 downto 0) <= b3(6 downto 0);
						elsif (unsigned(b2)=13) then
							ch13_volume_l(6 downto 0) <= b3(6 downto 0);
						elsif (unsigned(b2)=14) then
							ch14_volume_l(6 downto 0) <= b3(6 downto 0);
						elsif (unsigned(b2)=15) then
							ch15_volume_l(6 downto 0) <= b3(6 downto 0);
						elsif (unsigned(b2)=16) then
							ch16_volume_l(6 downto 0) <= b3(6 downto 0);
						elsif (unsigned(b2)=17) then
							-- Command = 17 (Main R)
							main_volume_r(6 downto 0) <= b3(6 downto 0);
						elsif (unsigned(b2)=18) then
							ch1_volume_r(6 downto 0) <= b3(6 downto 0);
						elsif (unsigned(b2)=19) then
							ch2_volume_r(6 downto 0) <= b3(6 downto 0);
						elsif (unsigned(b2)=20) then
							ch3_volume_r(6 downto 0) <= b3(6 downto 0);
						elsif (unsigned(b2)=21) then
							ch4_volume_r(6 downto 0) <= b3(6 downto 0);
						elsif (unsigned(b2)=22) then
							ch5_volume_r(6 downto 0) <= b3(6 downto 0);
						elsif (unsigned(b2)=23) then
							ch6_volume_r(6 downto 0) <= b3(6 downto 0);
						elsif (unsigned(b2)=24) then
							ch7_volume_r(6 downto 0) <= b3(6 downto 0);
						elsif (unsigned(b2)=25) then
							ch8_volume_r(6 downto 0) <= b3(6 downto 0);
						elsif (unsigned(b2)=26) then
							ch9_volume_r(6 downto 0) <= b3(6 downto 0);
						elsif (unsigned(b2)=27) then
							ch10_volume_r(6 downto 0) <= b3(6 downto 0);
						elsif (unsigned(b2)=28) then
							ch11_volume_r(6 downto 0) <= b3(6 downto 0);
						elsif (unsigned(b2)=29) then
							ch12_volume_r(6 downto 0) <= b3(6 downto 0);
						elsif (unsigned(b2)=30) then
							ch13_volume_r(6 downto 0) <= b3(6 downto 0);
						elsif (unsigned(b2)=31) then
							ch14_volume_r(6 downto 0) <= b3(6 downto 0);
						elsif (unsigned(b2)=32) then
							ch15_volume_r(6 downto 0) <= b3(6 downto 0);
						elsif (unsigned(b2)=33) then
							ch16_volume_r(6 downto 0) <= b3(6 downto 0);
						elsif (unsigned(b2)=127) then
							tx_start <= '1'; --send metering packet							
						end if;
					--end if;
				end if;
			end if;
			
			if tx_start = '1' then --metering packet has been requested
				tx_count := (others => '0');
				tx_start <= '0';
				Tx_Data <= std_logic_vector( to_unsigned( 16#AA#, 8));
				tx_send <= '1'; --flag to cause the trigger signal high
				tx_count := tx_count + 1;
			end if;
			
			if (tx_ready = '1') and (tx_count<19) then
			   Tx_Data(7) <= '0';
				if tx_count = 1 then
					Tx_Data(6 downto 0) <= std_logic_vector(ch1_peak(6 downto 0));
					tx_send <= '1';
				elsif tx_count = 2 then
					Tx_Data(6 downto 0) <= std_logic_vector(ch2_peak(6 downto 0));
					tx_send <= '1';
				elsif tx_count = 3 then
					Tx_Data(6 downto 0) <= std_logic_vector(ch3_peak(6 downto 0));
					tx_send <= '1';
				elsif tx_count = 4 then
					Tx_Data(6 downto 0) <= std_logic_vector(ch4_peak(6 downto 0));
					tx_send <= '1';
				elsif tx_count = 5 then
					Tx_Data(6 downto 0) <= std_logic_vector(ch5_peak(6 downto 0));
					tx_send <= '1';
				elsif tx_count = 6 then
					Tx_Data(6 downto 0) <= std_logic_vector(ch6_peak(6 downto 0));
					tx_send <= '1';
				elsif tx_count = 7 then
					Tx_Data(6 downto 0) <= std_logic_vector(ch7_peak(6 downto 0));
					tx_send <= '1';
				elsif tx_count = 8 then
					Tx_Data(6 downto 0) <= std_logic_vector(ch8_peak(6 downto 0));
					tx_send <= '1';
				elsif tx_count = 9 then
					Tx_Data(6 downto 0) <= std_logic_vector(ch9_peak(6 downto 0));
					tx_send <= '1';
				elsif tx_count = 10 then
					Tx_Data(6 downto 0) <= std_logic_vector(ch10_peak(6 downto 0));
					tx_send <= '1';
				elsif tx_count = 11 then
					Tx_Data(6 downto 0) <= std_logic_vector(ch11_peak(6 downto 0));
					tx_send <= '1';
				elsif tx_count = 12 then
					Tx_Data(6 downto 0) <= std_logic_vector(ch12_peak(6 downto 0));
					tx_send <= '1';
				elsif tx_count = 13 then
					Tx_Data(6 downto 0) <= std_logic_vector(ch13_peak(6 downto 0));
					tx_send <= '1';
				elsif tx_count = 14 then
					Tx_Data(6 downto 0) <= std_logic_vector(ch14_peak(6 downto 0));
					tx_send <= '1';
				elsif tx_count = 15 then
					Tx_Data(6 downto 0) <= std_logic_vector(ch15_peak(6 downto 0));
					tx_send <= '1';
				elsif tx_count = 16 then
					Tx_Data(6 downto 0) <= std_logic_vector(ch16_peak(6 downto 0));
					tx_send <= '1';
				elsif tx_count = 17 then --last byte = ultranet valid signal, 0x55 for good 0x33 for bad
					if ultranet_valid = '1' then
						Tx_Data <= std_logic_vector( to_unsigned( 16#55#, 8));
					else
						Tx_Data <= std_logic_vector( to_unsigned( 16#33#, 8));
					end if;
					tx_send <= '1';
					peak_reset <= '1';
				elsif tx_count = 18 then
					peak_reset <= '0';
				end if;
				tx_count := tx_count + 1;
			end if;
			
			if tx_send = '1' then --put the dataready line high for 1 tick
			   tx_send <= '0';
				TX_DataReady <= '1';
				tx_pulse := to_unsigned(200, 8);
			else
				if (tx_pulse > 0) then
					tx_pulse := tx_pulse - 1;
				else
					TX_DataReady <= '0'; --then take it low after 200 ticks
				end if;
			end if;

			
			
		end if;
	end process;
end Behavioral;