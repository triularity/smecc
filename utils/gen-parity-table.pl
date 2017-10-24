#!/usr/bin/env perl
#
# @(#) genparitytable.pl
#

use strict;

#
#

sub get_parity($)
{
	my	($d) = @_;
	my	$parity;


	$parity = 0;

	while($d != 0)
	{
		if($d & 0x01)
		{
			$parity = 1 - $parity;
		}

		$d >>= 1;
	}

	return $parity;
}


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
	my	$cp0;
	my	$cp1;
	my	$cp2;
	my	$cp3;
	my	$cp4;
	my	$cp5;
	my	$all;
	my	$p;


	@table = ();

	foreach $d ( 0x00 .. 0xFF )
	{
		$cp0 = get_parity($d & 0x55);
		$cp1 = get_parity($d & 0xAA);

		$cp2 = get_parity($d & 0x33);
		$cp3 = get_parity($d & 0xCC);

		$cp4 = get_parity($d & 0x0F);
		$cp5 = get_parity($d & 0xF0);

		$all = get_parity($d);

		$p = ($cp5 << 7)
			| ($cp4 << 6)
			| ($cp3 << 5)
			| ($cp2 << 4)
			| ($cp1 << 3)
			| ($cp0 << 2)
			| ($all << 0);

		push(@table, $p);
	}

	dump_table(@table);
}

#
#

gentable();
