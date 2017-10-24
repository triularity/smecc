#!/usr/bin/env perl
#
# @(#) gen-bitexpand-table.pl
#

use strict;


sub dump_table(@)
{
	my	(@table) = @_;
	my	@line;


	while(@table)
	{
		@line = splice(@table, 0, 8);

		print "\t", join(", ", map { sprintf("0x%04X", $_) } @line),
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
		$x = (($d & 0x80) << (14-7))
                        | (($d & 0x40) << (12-6))
                        | (($d & 0x20) << (10-5))
                        | (($d & 0x10) << ( 8-4))
                        | (($d & 0x08) << ( 6-3))
                        | (($d & 0x04) << ( 4-2))
                        | (($d & 0x02) << ( 2-1))
                        | (($d & 0x01) << ( 0-0));

		push(@table, $x);
	}

	dump_table(@table);
}

#
#

gentable();
