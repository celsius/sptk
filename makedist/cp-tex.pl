#!/usr/local/bin/perl

use File::Path;
use Getopt::Long;
$Getopt::Long::ignorecase=undef;
sub execute;

#
# Default
#
$opt_b = "..";
$opt_l = "list.all";
$opt_f = "tex-files.extra";
$opt_d = "SPTKref";
$opt_v = undef;
$opt_n = undef;
$opt_h = undef;

#
# Get Options
#
GetOptions("b|base-dir=s", "l|command-list=s", "f|extra-file-list=s", "d|target-dir=s", "v|verbose", "n", "h");

if ( $opt_h == 1 ) {
	print "\n";
	print "cp-tex.pl [options]\n";
	print "\n";
	print "    options:\n";
	print "        -b s :  base directory\n";
	print "        -l s :  command list\n";
	print "        -f s :  extra file list\n";
	print "        -d s :  target directory name\n";
	print "        -v   :  print commands\n";
	print "        -n   :  print commands but do not execute them (same as 'make -n')\n";
	print "        -h   :  print this message\n";
	print "\n";
	exit;
}


#
# Read Database
#
open(f, $opt_f) || die "file list $opt_f not found";
while ( $x = <f> ) {
	chomp($x);
	if ( $x =~ /[-0-9A-Za-z\/.]/ ) {
		if ( $x =~ /:$/ ) {
			($CMND) = ($x =~ /^(.*):$/);
		}
		else {
			push(@{$hashref->{$CMND}}, $x);
		}
	}
}
close(f);

#
# Copy Files
#
execute("mkdir -p $opt_d") unless ( -d "$opt_d" );
execute("cp $opt_b/doc/ref/cmndref.sty $opt_d");
execute("cp $opt_b/doc/ref/ref.tex $opt_d");

# modify Makefile and main.tex
print "$opt_b/doc/ref/Makefile -> $opt_d/Makefile\n"
	if ( ( $opt_n == 1 ) || ( $opt_v == 1 ) );
if ( $opt_n == undef ) {
	open(MAKEIN, "$opt_b/doc/ref/Makefile") || die "cannot open $opt_b/doc/ref/Makefile";
	open(MAKEOUT, "> $opt_d/Makefile") || die "cannot open $opt_d/Makefile";
	while ( <MAKEIN> ) {
		last if ( /INC/ );
		print MAKEOUT;
	}
	print MAKEOUT "INC  = cmndref.sty";
	while ( <MAKEIN> ) {
		last if ( /^$/ );
	}
}

print "$opt_b/doc/ref/main.tex -> $opt_d/main.tex\n"
	if ( ( $opt_n == 1 ) || ( $opt_v == 1 ) );
if ( $opt_n == undef ) {
	open(MAININ, "$opt_b/doc/ref/main.tex") || die "cannot open $opt_b/doc/ref/main.tex";
	open(MAINOUT, "> $opt_d/main.tex") || die "cannot open $opt_d/main.tex";
	while ( <MAININ> ) {
		last if ( /BEGIN COMMANDS/ );
		print MAINOUT;
	}
	while ( <MAININ> ) {
		last if ( /END COMMANDS/ );
	}
}

open (FILE, $opt_l) || die "command list file $opt_l not found\n";
while ( $x = <FILE> ) {
	chomp($x);
	if (( $x =~ /^#/ ) || ( $x !~ /[-0-9A-Za-z\/.]/ )) {
		next;
	}

	# add Makefile and main.tex
	print MAKEOUT " \\\n\t$x.tex";
	print MAINOUT "\\include{$x}\n";

	# standard files
	execute("cp $opt_b/doc/ref/$x.tex $opt_d");

	# extra files
	for ( $i = 0 ; $i < @{$hashref->{$x}} ; $i++ ) {
		execute("mkdir -p $opt_d/fig") unless ( -d "$opt_d/fig" );
		execute("cp $opt_b/$hashref->{$x}[$i] $opt_d/fig")
			if ( -f "$opt_b/$hashref->{$x}[$i]" );
	}
}
close(FILE);

print MAKEOUT " \\\n\tref.tex";

if ( $opt_n == undef ) {
	print MAKEOUT "\nEPSF = ";
	if ( -d "$opt_d/fig" ) {
		opendir(DIR, "$opt_d/fig");
		$EPSF = 0;
		while ( $file = readdir(DIR) ) {
			if ( -f "$opt_d/fig/$file" ) {
				if ( $EPSF ) {
					print MAKEOUT " \\\n\tfig/$file";
				} else {
					print MAKEOUT "fig/$file";
					$EPSF = 1;
				}
			}
		}
		closedir(DIR);
	}
	print MAKEOUT "\n\n";
	while ( <MAKEIN> ) {
		print MAKEOUT;
	}
	while ( <MAININ> ) {
		print MAINOUT;
	}
}
close(MAKEIN);
close(MAKEOUT);
close(MAININ);
close(MAINOUT);

sub execute {
	$cmnd = $_[0];

	print $cmnd, "\n" if ( ( $opt_n == 1 ) || ( $opt_v == 1) );
	system($cmnd) unless ( $opt_n == 1 );
}

