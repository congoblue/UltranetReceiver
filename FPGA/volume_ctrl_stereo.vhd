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
4,
16,
36,
64,
100,
144,
196,
256,
324,
400,
484,
576,
676,
784,
900,
1024,
1156,
1296,
1444,
1600,
1764,
1936,
2116,
2304,
2500,
2704,
2916,
3136,
3364,
3600,
3844,
4096,
4356,
4624,
4900,
5184,
5476,
5776,
6084,
6400,
6724,
7056,
7396,
7744,
8100,
8464,
8836,
9216,
9604,
10000,
10404,
10816,
11236,
11664,
12100,
12544,
12996,
13456,
13924,
14400,
14884,
15376,
15876,
16384,
16900,
17424,
17956,
18496,
19044,
19600,
20164,
20736,
21316,
21904,
22500,
23104,
23716,
24336,
24964,
25600,
26244,
26896,
27556,
28224,
28900,
29584,
30276,
30976,
31684,
32400,
33124,
33856,
34596,
35344,
36100,
36864,
37636,
38416,
39204,
40000,
40804,
41616,
42436,
43264,
44100,
44944,
45796,
46656,
47524,
48400,
49284,
50176,
51076,
51984,
52900,
53824,
54756,
55696,
56644,
57600,
58564,
59536,
60516,
61504,
62500,
63504,
64516


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
