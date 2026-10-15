library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
entity pm12_mcu_spi is
 port(SCKI,SPI_CS,MOSII:in std_logic; MISOI:out std_logic;
      scratch_out,writes_out:out std_logic_vector(15 downto 0));
end;
architecture recovered of pm12_mcu_spi is
 signal spi_bit_count:std_logic_vector(4 downto 0):=(others=>'0');
 signal spi_addr:unsigned(8 downto 0):=(others=>'0');
 signal SPI_DATA,spi_wr_data,timestamp_lo,timestamp_hi:std_logic_vector(15 downto 0):=(others=>'0');
 signal spi_rd,spi_na,spi_wr_rdy,spibuf_wr,spibuf_rd,cnt_rd,rd_lock_spi:std_logic:='0';
 signal scratch:std_logic_vector(15 downto 0):=x"0000";
 signal writes:unsigned(15 downto 0):=(others=>'0');
begin
 scratch_out<=scratch;writes_out<=std_logic_vector(writes);
 rd_lock_spi <= '1' when (spi_bit_count(3 downto 0)=x"F") and (spi_rd='1') and (spi_addr(8)='0') else '0';
 process(SCKI) begin if falling_edge(SCKI) then MISOI<=SPI_DATA(15);end if;end process;
 process(SCKI) begin
  if rising_edge(SCKI) and SPI_CS='0' and spi_bit_count="11111" and spi_rd='0' and spi_addr=16#40# then
   scratch<=SPI_DATA(14 downto 0)&MOSII;writes<=writes+1;
  end if;
 end process;
process (SCKI, SPI_CS)
begin
if (SPI_CS='1') then spi_bit_count<="00000"; cnt_rd<='0'; spibuf_wr<='0'; spibuf_rd<='0'; spi_na<='0'; else

-- MISO falling-edge register is preserved in a separate process.

if (SCKI'event and SCKI='1') then

        if (spi_bit_count="11111") then spi_bit_count<="10000"; spi_na<='1';
          if  (spi_rd='0') then  
            if (spi_addr(7 downto 0) = x"F5") then timestamp_lo <= SPI_DATA(14 downto 0) & MOSII; end if;
            if (spi_addr(7 downto 0) = x"F6") then timestamp_hi <= SPI_DATA(14 downto 0) & MOSII;  end if;
            spi_wr_data<=SPI_DATA(14 downto 0) & MOSII;  
            if (spi_addr(7)='0') then  spi_wr_rdy<='1'; else if (spi_addr(6)='0') then spibuf_wr<='1'; end if; end if;
          end if;
         else 
         spi_bit_count<=std_logic_vector(unsigned(spi_bit_count)+1); 
        end if;
        if (spi_bit_count="10000") then spi_wr_rdy<='0'; spibuf_wr<='0'; if (spi_rd='0') and (spi_na='1') then spi_addr <= spi_addr+1; end if; end if;
        if (spi_bit_count="00000") then spi_rd <= MOSII; end if;
        if (spi_bit_count="01001") then spi_addr <= unsigned(SPI_DATA(7 downto 0) & MOSII); end if;
        if (spi_bit_count="01110") and (spi_rd='1') and (spi_addr(7 downto 0)=x"C0") then cnt_rd<='1'; end if;
        
        if (spi_bit_count(3 downto 0)="1110") and (spi_rd='1') and (spi_addr(7 downto 6)="10") then spibuf_rd<='1'; end if; 
                     
        if (rd_lock_spi='1') then  spibuf_rd<='0'; spi_addr <= spi_addr+1; 

          case to_integer(unsigned(spi_addr(7 downto 0))) is
            when 16#40# => SPI_DATA<=scratch;
            when 16#41# => SPI_DATA<=x"504D";
            when 16#42# => SPI_DATA<=x"5350";
            when 16#43# => SPI_DATA<=x"0001";
            when 16#44# => SPI_DATA<=std_logic_vector(writes);
            when others => SPI_DATA<=x"0000";
          end case;
        else SPI_DATA<=SPI_DATA(14 downto 0) & MOSII; 
        end if;
         
end if;
end if;
end process;
end;
