#!/usr/bin/env perl
#
# @(#) gen-bitcount-table.pl
#

use strict;


sub dump_table(@)
{
	my	(@table) = @_;
	my	@line;


	while(@table)
	{
		@line = splice(@table, 0, 8);

		print "\t", join(", ", map { sprintf("0x%02X", $_) } @line),
			((@table != 0) ? "," : ""), "\n";
	}
}


sub gentable()
{
	my	@table;
	my	$d;
	my	$x;


	@table = ();

	foreach $d ( 0x00 .. 0xFF )
	{
		$x = (($d >> 7) & 0x01)
			+ (($d >> 6) & 0x01)
			+ (($d >> 5) & 0x01)
			+ (($d >> 4) & 0x01)
			+ (($d >> 3) & 0x01)
			+ (($d >> 2) & 0x01)
			+ (($d >> 1) & 0x01)
			+ (($d >> 0) & 0x01);

		push(@table, $x);
	}

	dump_table(@table);
}

#
#

gentable();
