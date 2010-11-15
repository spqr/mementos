#!/usr/bin/env perl
use strict;
use warnings;

if (scalar(@ARGV) < 2) {
    print STDERR "USAGE: $0 <V_thresh> <Timer_Interval> infile > outfile\n";
    exit 1;
}

my $vthresh = shift;
my $tinterval = shift;
while (<>) {
    # uncomment the V_THRESH line corresponding to the selected voltage
    if (m{^// #define V_THRESH} and m{\b$vthresh volts$}) {
        s{^// }{};
    }

    s/^\/\/==TIMER_INTERVAL==$/#define TIMER_INTERVAL $tinterval/;
    print;
}
