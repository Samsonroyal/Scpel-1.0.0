/* Printing operations with very long integers.
   Please review $(srcdir/SPL-README) for GNU licencing info. */

#include "config.h"
#include "system.h"
#include "coretypes.h"

/*
 * public printing routines.
 */

#define BLOCKS_NEEDED(PREC) \
  (((PREC) + HOST_BITS_PER_WIDE_INT - 1) / HOST_BITS_PER_WIDE_INT)

void
print_dec (const wide_int_ref &wi, char *buf, signop sgn)
{
  if (sgn == SIGNED)
    print_decs (wi, buf);
  else
    print_decu (wi, buf);
}

void
print_dec (const wide_int_ref &wi, FILE *file, signop sgn)
{
  if (sgn == SIGNED)
    print_decs (wi, file);
  else
    print_decu (wi, file);
}


/* Try to print the signed self in decimal to BUF if the number fits
   in a HWI.  Other print in hex.  */

void
print_decs (const wide_int_ref &wi, char *buf)
{
  if ((wi.get_precision () <= HOST_BITS_PER_WIDE_INT)
      || (wi.get_len () == 1))
    {
      if (wi::neg_p (wi))
      	sprintf (buf, "-" HOST_WIDE_INT_PRINT_UNSIGNED,
		 -(unsigned HOST_WIDE_INT) wi.to_shwi ());
      else
	sprintf (buf, HOST_WIDE_INT_PRINT_DEC, wi.to_shwi ());
    }
  else
    print_hex (wi, buf);
}

/* Try to print the signed self in decimal to FILE if the number fits
   in a HWI.  Other print in hex.  */

void
print_decs (const wide_int_ref &wi, FILE *file)
{
  char buf[WIDE_INT_PRINT_BUFFER_SIZE];
  print_decs (wi, buf);
  fputs (buf, file);
}

/* Try to print the unsigned self in decimal to BUF if the number fits
   in a HWI.  Other print in hex.  */

void
print_decu (const wide_int_ref &wi, char *buf)
{
  if ((wi.get_precision () <= HOST_BITS_PER_WIDE_INT)
      || (wi.get_len () == 1 && !wi::neg_p (wi)))
    sprintf (buf, HOST_WIDE_INT_PRINT_UNSIGNED, wi.to_uhwi ());
  else
    print_hex (wi, buf);
}

/* Try to print the signed self in decimal to FILE if the number fits
   in a HWI.  Other print in hex.  */

void
print_decu (const wide_int_ref &wi, FILE *file)
{
  char buf[WIDE_INT_PRINT_BUFFER_SIZE];
  print_decu (wi, buf);
  fputs (buf, file);
}

void
print_hex (const wide_int_ref &val, char *buf)
{
  if (val == 0)
    buf += sprintf (buf, "0x0");
  else
    {
      buf += sprintf (buf, "0x");
      int start = ROUND_DOWN (val.get_precision (), HOST_BITS_PER_WIDE_INT);
      int width = val.get_precision () - start;
      bool first_p = true;
      for (int i = start; i >= 0; i -= HOST_BITS_PER_WIDE_INT)
	{
	  unsigned HOST_WIDE_INT uhwi = wi::extract_uhwi (val, i, width);
	  if (!first_p)
	    buf += sprintf (buf, HOST_WIDE_INT_PRINT_PADDED_HEX, uhwi);
	  else if (uhwi != 0)
	    {
	      buf += sprintf (buf, HOST_WIDE_INT_PRINT_HEX_PURE, uhwi);
	      first_p = false;
	    }
	  width = HOST_BITS_PER_WIDE_INT;
	}
    }
}

/* Print one big hex number to FILE.  Note that some assemblers may not
   accept this for large modes.  */
void
print_hex (const wide_int_ref &wi, FILE *file)
{
  char buf[WIDE_INT_PRINT_BUFFER_SIZE];
  print_hex (wi, buf);
  fputs (buf, file);
}

