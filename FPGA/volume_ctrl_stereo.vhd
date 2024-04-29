library ieee;
use IEEE.std_logic_1164.all;
use ieee.numeric_std.all; --This allows the use of the signed variable types. The complete model using this approach is much more compact and is shown below:

entity volume_ctrl_stereo is
	generic(top : natural := 23);
port (
	signal_in 		: in signed(top downto 0);
	volume_l			: in unsigned(top downto 0); -- value is between 0 and 256
	volume_r			: in unsigned(top downto 0); -- value is between 0 and 256
	signal_out_l	: out std_logic_vector(top downto 0);
	signal_out_r	: out std_logic_vector(top downto 0)
	);
end entity volume_ctrl_stereo;

architecture behavior of volume_ctrl_stereo is

	type t_log is array (0 to 127) of integer;

	signal signal_x_volume_l : signed((2*top)+1 downto 0); -- we need double the bits for the multiplication
	signal signal_x_volume_r : signed((2*top)+1 downto 0); -- we need double the bits for the multiplication

	constant volume_log	: t_log := (
	
	0,
3,
7,
11,
15,
20,
25,
30,
35,
41,
47,
53,
60,
67,
74,
82,
90,
99,
108,
118,
129,
139,
151,
163,
176,
189,
204,
219,
235,
251,
269,
288,
308,
328,
351,
374,
398,
424,
452,
481,
511,
544,
578,
614,
652,
692,
734,
779,
826,
876,
929,
984,
1043,
1105,
1170,
1239,
1312,
1389,
1471,
1557,
1647,
1743,
1844,
1951,
2064,
2183,
2309,
2441,
2581,
2729,
2886,
3051,
3225,
3409,
3603,
3808,
4025,
4254,
4495,
4750,
5019,
5303,
5604,
5920,
6255,
6608,
6982,
7376,
7792,
8231,
8695,
9184,
9701,
10247,
10824,
11433,
12075,
12754,
13471,
14228,
15027,
15870,
16761,
17702,
18695,
19744,
20851,
22020,
23255,
24559,
25935,
27388,
28923,
30544,
32255,
34062,
35969,
37984,
40111,
42357,
44728,
47232,
49876,
52668,
55616,
58728,
62015,
65485

	);
	
begin
	
	volume_control_l : process (signal_in, volume_l)
	begin
		signal_x_volume_l <= signal_in * volume_log(to_integer(volume_l)); -- do multiplication signal x volume[0-65535]
	end process volume_control_l;
	
	volume_control_r : process (signal_in, volume_r)
	begin
		signal_x_volume_r <= signal_in * volume_log(to_integer(volume_r)); -- do multiplication signal x volume[0-65535]
	end process volume_control_r;

	-- now we have to divide the multiplicated signal by 256.
	-- We can bitshift this by 8 to the right, but we have
	-- to take care of the sign-bit as we have signed-integers here!
	signal_out_l <= std_logic_vector(signal_x_volume_l)((2*top)+1) & std_logic_vector(signal_x_volume_l)(top+15 downto 16); -- signed-bit & 23 audio-sample-bits
	signal_out_r <= std_logic_vector(signal_x_volume_r)((2*top)+1) & std_logic_vector(signal_x_volume_r)(top+15 downto 16); -- signed-bit & 23 audio-sample-bits
end architecture behavior;
