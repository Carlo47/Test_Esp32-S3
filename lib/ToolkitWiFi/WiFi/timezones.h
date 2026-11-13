/**
 * Purpose      Definition of some Time Zones
 * Reference    https://www.sourceware.org/glibc/manual/2.39/html_node/TZ-Variable.html
 * Format       std offset dst [offset],start[/time],end[/time]
 */
#pragma once

namespace RTCUtils
{
  const char MEZ_MESZ[] = "MEZ-1MESZ-2,M3.5.0/02:00:00,M10.5.0/03:00:00"; // Mitteleuropäische Zeit / Sommerzeit
  const char EST_EDT[]  = "EST5EDT4,M3.2.0/02:00:00,M11.1.0/02:00:00";    // Eastern standard time / dayligt saving time
  const char IST_IDT[]  = "IST-2IDT,M3.4.4/26,M10.5.0";                   // Isral standard time / daylight saving time
  const char WGT_WGST[] = "WGT3WGST,M3.5.0/-2,M10.5.0/-1"; // Western Greenland time / daylight saving time
} // namespace RTCUtils