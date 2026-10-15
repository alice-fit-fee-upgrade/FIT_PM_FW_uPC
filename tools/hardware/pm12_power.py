"""Wait for physical MCU power-good inputs without requesting an ADT transfer."""
import re
import time

def power_good(reply):
    match = re.search(r'HW V2 PORTS A\.\.F=([0-9A-F]{12})(?: |\r|\n)', reply)
    if not match:
        return False
    port_e = bytes.fromhex(match[1])[4]
    # PE1 IN_PWR_OK, PE3 P1V2LDO_PG; PE2 reflects driven EN_PSU.
    return port_e & 0x0e == 0x0e

def wait_power_good(uart, capture, timeout=20.0, stable=1.0):
    """Require >=3 good GPIO snapshots spanning >=1 s; fail closed on timeout.

    @HW snapshots GPIO/SRAM only. It does not initiate temperature SPI.
    An instantaneous check is still required immediately before a sensor read.
    """
    deadline = time.monotonic() + timeout
    since = None
    good_count = 0
    rows = []
    while time.monotonic() < deadline:
        row = capture(uart, '@HW', limit=min(1.0, max(.05, deadline-time.monotonic())))
        rows.append(row)
        now = time.monotonic()
        if power_good(row['reply']):
            if since is None:
                since = now
            good_count += 1
            if good_count >= 3 and now-since >= stable:
                return rows
        else:
            since = None
            good_count = 0
        time.sleep(min(.25, max(0.0, deadline-time.monotonic())))
    raise RuntimeError('PGOOD not stable before sensor access; last HW='+repr(rows[-1:] ))

def require_power_good(uart, capture):
    """Fresh GPIO check immediately before an explicitly requested sensor read."""
    row = capture(uart, '@HW', limit=1.0)
    if not power_good(row['reply']):
        raise RuntimeError('PGOOD dropped before sensor access: '+repr(row))
    return row
