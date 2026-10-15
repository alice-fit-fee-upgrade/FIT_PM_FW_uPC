-- Diagnostic wrapper around the extracted original PM trigger entity.
-- Original output pipeline: fit.vhd tto<=tt/tao<=ta on clk320, then LVDS OBUFDS.
-- No new reset or output mux semantics are introduced here.
-- Not yet synthesized or hardware-qualified; constraints are board-specific.
library ieee;
use ieee.std_logic_1164.all;
library unisim;
use unisim.vcomponents.all;
use work.PM12_pkg.all;

entity pm_trigger_tx is
  port (
    clk320 : in std_logic;
    mt_cou : in std_logic_vector(2 downto 0);
    CH_trigt, CH_triga, CH_trigb : in std_logic_vector(11 downto 0);
    CH_TIME_T : in trig_time;
    CH_ampl0 : in trig_ampl0;
    tcm_req : in std_logic;
    AT_P, AT_N, TT_P, TT_N : out std_logic_vector(1 downto 0)
  );
end entity;

architecture rtl of pm_trigger_tx is
  signal tt, ta, tto, tao : std_logic_vector(1 downto 0);
begin
  original_trigger : entity work.trigger
    port map (clk320=>clk320, mt_cou=>mt_cou, CH_trigt=>CH_trigt,
      CH_triga=>CH_triga, CH_trigb=>CH_trigb, CH_TIME_T=>CH_TIME_T,
      CH_ampl0=>CH_ampl0, tcm_req=>tcm_req, tt=>tt, ta=>ta);
  process(clk320)
  begin
    if rising_edge(clk320) then
      tto <= tt;
      tao <= ta;
    end if;
  end process;
  lanes : for i in 0 to 1 generate
    amplitude_out : OBUFDS
      generic map (SLEW=>"FAST", IOSTANDARD=>"LVDS")
      port map (I=>tao(i), O=>AT_P(i), OB=>AT_N(i));
    time_out : OBUFDS
      generic map (SLEW=>"FAST", IOSTANDARD=>"LVDS")
      port map (I=>tto(i), O=>TT_P(i), OB=>TT_N(i));
  end generate;
end architecture;
